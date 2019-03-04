
#include "config.h"

#include <cmath>
#include <array>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

#include "bformatdec.h"
#include "ambdec.h"
#include "filters/splitter.h"
#include "alu.h"

#include "threads.h"
#include "almalloc.h"


namespace {

using namespace std::placeholders;


constexpr ALfloat Ambi3DDecoderHFScale[MAX_AMBI_ORDER+1] = {
    1.00000000e+00f, 1.00000000e+00f
};
constexpr ALfloat Ambi3DDecoderHFScale2O[MAX_AMBI_ORDER+1] = {
    7.45355990e-01f, 1.00000000e+00f
};
constexpr ALfloat Ambi3DDecoderHFScale3O[MAX_AMBI_ORDER+1] = {
    5.89792205e-01f, 8.79693856e-01f
};

inline auto GetDecoderHFScales(ALsizei order) noexcept -> const ALfloat(&)[MAX_AMBI_ORDER+1]
{
    if(order >= 3) return Ambi3DDecoderHFScale3O;
    if(order == 2) return Ambi3DDecoderHFScale2O;
    return Ambi3DDecoderHFScale;
}

inline auto GetAmbiScales(AmbDecScale scaletype) noexcept -> const std::array<float,MAX_AMBI_CHANNELS>&
{
    if(scaletype == AmbDecScale::FuMa) return AmbiScale::FromFuMa;
    if(scaletype == AmbDecScale::SN3D) return AmbiScale::FromSN3D;
    return AmbiScale::FromN3D;
}

} // namespace


void BFormatDec::reset(const AmbDecConf *conf, const bool allow_2band, const ALsizei inchans,
    const ALuint srate, const ALsizei (&chanmap)[MAX_OUTPUT_CHANNELS])
{
    mSamples.clear();
    mSamplesHF = nullptr;
    mSamplesLF = nullptr;

    mMatrix = MatrixU{};
    mDualBand = allow_2band && (conf->FreqBands == 2);
    if(!mDualBand)
        mSamples.resize(2);
    else
    {
        ASSUME(inchans > 0);
        mSamples.resize(inchans * 2);
        mSamplesHF = mSamples.data();
        mSamplesLF = mSamplesHF + inchans;
    }
    mNumChannels = inchans;

    mEnabled = std::accumulate(std::begin(chanmap), std::begin(chanmap)+conf->Speakers.size(), 0u,
        [](ALuint mask, const ALsizei &chan) noexcept -> ALuint
        { return mask | (1 << chan); }
    );

    const ALfloat xover_norm{conf->XOverFreq / static_cast<float>(srate)};

    const bool periphonic{(conf->ChanMask&AMBI_PERIPHONIC_MASK) != 0};
    const std::array<float,MAX_AMBI_CHANNELS> &coeff_scale = GetAmbiScales(conf->CoeffScale);
    const size_t coeff_count{periphonic ? MAX_AMBI_CHANNELS : MAX_AMBI2D_CHANNELS};

    if(!mDualBand)
    {
        for(size_t i{0u};i < conf->Speakers.size();i++)
        {
            ALfloat (&mtx)[MAX_AMBI_CHANNELS] = mMatrix.Single[chanmap[i]];
            for(size_t j{0},k{0};j < coeff_count;j++)
            {
                const size_t l{periphonic ? j : AmbiIndex::From2D[j]};
                if(!(conf->ChanMask&(1u<<l))) continue;
                mtx[j] = conf->HFMatrix[i][k] / coeff_scale[l] *
                    ((l>=9) ? conf->HFOrderGain[3] :
                    (l>=4) ? conf->HFOrderGain[2] :
                    (l>=1) ? conf->HFOrderGain[1] : conf->HFOrderGain[0]);
                ++k;
            }
        }
    }
    else
    {
        mXOver[0].init(xover_norm);
        std::fill(std::begin(mXOver)+1, std::end(mXOver), mXOver[0]);

        const float ratio{std::pow(10.0f, conf->XOverRatio / 40.0f)};
        for(size_t i{0u};i < conf->Speakers.size();i++)
        {
            ALfloat (&mtx)[sNumBands][MAX_AMBI_CHANNELS] = mMatrix.Dual[chanmap[i]];
            for(size_t j{0},k{0};j < coeff_count;j++)
            {
                const size_t l{periphonic ? j : AmbiIndex::From2D[j]};
                if(!(conf->ChanMask&(1u<<l))) continue;
                mtx[sHFBand][j] = conf->HFMatrix[i][k] / coeff_scale[l] *
                    ((l>=9) ? conf->HFOrderGain[3] :
                    (l>=4) ? conf->HFOrderGain[2] :
                    (l>=1) ? conf->HFOrderGain[1] : conf->HFOrderGain[0]) * ratio;
                mtx[sLFBand][j] = conf->LFMatrix[i][k] / coeff_scale[l] *
                    ((l>=9) ? conf->LFOrderGain[3] :
                    (l>=4) ? conf->LFOrderGain[2] :
                    (l>=1) ? conf->LFOrderGain[1] : conf->LFOrderGain[0]) / ratio;
                ++k;
            }
        }
    }
}

void BFormatDec::reset(const ALsizei inchans, const ALsizei chancount,
    const ChannelDec (&chancoeffs)[MAX_OUTPUT_CHANNELS],
    const ALsizei (&chanmap)[MAX_OUTPUT_CHANNELS])
{
    mSamples.clear();
    mSamplesHF = nullptr;
    mSamplesLF = nullptr;

    mMatrix = MatrixU{};
    mDualBand = false;
    mSamples.resize(2);
    mNumChannels = inchans;

    ASSUME(chancount > 0);
    mEnabled = std::accumulate(std::begin(chanmap), std::begin(chanmap)+chancount, 0u,
        [](ALuint mask, const ALsizei &chan) noexcept -> ALuint
        { return mask | (1 << chan); }
    );

    auto set_coeffs = [this,inchans,&chancoeffs](const ALsizei chanidx) noexcept -> void
    {
        ASSUME(chanidx >= 0);
        const ALfloat (&coeffs)[MAX_AMBI_CHANNELS] = chancoeffs[chanidx];
        ALfloat (&mtx)[MAX_AMBI_CHANNELS] = mMatrix.Single[chanidx];

        ASSUME(inchans > 0);
        std::copy_n(std::begin(coeffs), inchans, std::begin(mtx));
    };
    std::for_each(chanmap, chanmap+chancount, set_coeffs);
}


void BFormatDec::process(ALfloat (*OutBuffer)[BUFFERSIZE], const ALsizei OutChannels, const ALfloat (*InSamples)[BUFFERSIZE], const ALsizei SamplesToDo)
{
    ASSUME(OutChannels > 0);
    ASSUME(mNumChannels > 0);

    if(mDualBand)
    {
        for(ALsizei i{0};i < mNumChannels;i++)
            mXOver[i].process(mSamplesHF[i].data(), mSamplesLF[i].data(), InSamples[i],
                              SamplesToDo);

        for(ALsizei chan{0};chan < OutChannels;chan++)
        {
            if(UNLIKELY(!(mEnabled&(1<<chan))))
                continue;

            MixRowSamples(OutBuffer[chan], mMatrix.Dual[chan][sHFBand],
                &reinterpret_cast<ALfloat(&)[BUFFERSIZE]>(mSamplesHF[0]),
                mNumChannels, 0, SamplesToDo);
            MixRowSamples(OutBuffer[chan], mMatrix.Dual[chan][sLFBand],
                &reinterpret_cast<ALfloat(&)[BUFFERSIZE]>(mSamplesLF[0]),
                mNumChannels, 0, SamplesToDo);
        }
    }
    else
    {
        for(ALsizei chan{0};chan < OutChannels;chan++)
        {
            if(UNLIKELY(!(mEnabled&(1<<chan))))
                continue;

            MixRowSamples(OutBuffer[chan], mMatrix.Single[chan], InSamples,
                          mNumChannels, 0, SamplesToDo);
        }
    }
}


std::array<ALfloat,MAX_AMBI_ORDER+1> BFormatDec::GetHFOrderScales(const ALsizei in_order, const ALsizei out_order) noexcept
{
    std::array<ALfloat,MAX_AMBI_ORDER+1> ret{};

    assert(out_order >= in_order);
    ASSUME(out_order >= in_order);

    const ALfloat (&target)[MAX_AMBI_ORDER+1] = GetDecoderHFScales(out_order);
    const ALfloat (&input)[MAX_AMBI_ORDER+1] = GetDecoderHFScales(in_order);

    for(ALsizei i{0};i < in_order+1;++i)
        ret[i] = input[i] / target[i];

    return ret;
}
