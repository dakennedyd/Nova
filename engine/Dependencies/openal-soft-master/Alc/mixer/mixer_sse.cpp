#include "config.h"

#include <xmmintrin.h>

#include <limits>

#include "AL/al.h"
#include "AL/alc.h"
#include "alMain.h"
#include "alu.h"

#include "alSource.h"
#include "alAuxEffectSlot.h"
#include "defs.h"
#include "hrtfbase.h"


template<>
const ALfloat *Resample_<BSincTag,SSETag>(const InterpState *state, const ALfloat *RESTRICT src,
    ALsizei frac, ALint increment, ALfloat *RESTRICT dst, ALsizei dstlen)
{
    const ALfloat *const filter{state->bsinc.filter};
    const __m128 sf4{_mm_set1_ps(state->bsinc.sf)};
    const ALsizei m{state->bsinc.m};

    ASSUME(m > 0);
    ASSUME(dstlen > 0);
    ASSUME(increment > 0);
    ASSUME(frac >= 0);

    src -= state->bsinc.l;
    for(ALsizei i{0};i < dstlen;i++)
    {
        // Calculate the phase index and factor.
#define FRAC_PHASE_BITDIFF (FRACTIONBITS-BSINC_PHASE_BITS)
        const ALsizei pi{frac >> FRAC_PHASE_BITDIFF};
        const ALfloat pf{(frac & ((1<<FRAC_PHASE_BITDIFF)-1)) * (1.0f/(1<<FRAC_PHASE_BITDIFF))};
#undef FRAC_PHASE_BITDIFF

        ALsizei offset{m*pi*4};
        const __m128 *fil{reinterpret_cast<const __m128*>(filter + offset)}; offset += m;
        const __m128 *scd{reinterpret_cast<const __m128*>(filter + offset)}; offset += m;
        const __m128 *phd{reinterpret_cast<const __m128*>(filter + offset)}; offset += m;
        const __m128 *spd{reinterpret_cast<const __m128*>(filter + offset)};

        // Apply the scale and phase interpolated filter.
        __m128 r4{_mm_setzero_ps()};
        {
            const ALsizei count{m >> 2};
            const __m128 pf4{_mm_set1_ps(pf)};

            ASSUME(count > 0);

#define MLA4(x, y, z) _mm_add_ps(x, _mm_mul_ps(y, z))
            for(ALsizei j{0};j < count;j++)
            {
                /* f = ((fil + sf*scd) + pf*(phd + sf*spd)) */
                const __m128 f4 = MLA4(
                    MLA4(fil[j], sf4, scd[j]),
                    pf4, MLA4(phd[j], sf4, spd[j])
                );
                /* r += f*src */
                r4 = MLA4(r4, f4, _mm_loadu_ps(&src[j*4]));
            }
#undef MLA4
        }
        r4 = _mm_add_ps(r4, _mm_shuffle_ps(r4, r4, _MM_SHUFFLE(0, 1, 2, 3)));
        r4 = _mm_add_ps(r4, _mm_movehl_ps(r4, r4));
        dst[i] = _mm_cvtss_f32(r4);

        frac += increment;
        src  += frac>>FRACTIONBITS;
        frac &= FRACTIONMASK;
    }
    return dst;
}


static inline void ApplyCoeffs(ALsizei Offset, HrirArray<ALfloat> &Values, const ALsizei IrSize,
    const HrirArray<ALfloat> &Coeffs, const ALfloat left, const ALfloat right)
{
    const __m128 lrlr{_mm_setr_ps(left, right, left, right)};

    ASSUME(IrSize >= 2);
    ASSUME(&Values != &Coeffs);

    ASSUME(Offset >= 0 && Offset < HRIR_LENGTH);
    if((Offset&1))
    {
        ALsizei count{mini(IrSize-1, HRIR_LENGTH - Offset)};
        ASSUME(count >= 1);

        __m128 imp0, imp1;
        __m128 coeffs{_mm_load_ps(&Coeffs[0][0])};
        __m128 vals{_mm_loadl_pi(_mm_setzero_ps(), reinterpret_cast<__m64*>(&Values[Offset][0]))};
        imp0 = _mm_mul_ps(lrlr, coeffs);
        vals = _mm_add_ps(imp0, vals);
        _mm_storel_pi(reinterpret_cast<__m64*>(&Values[Offset][0]), vals);
        ++Offset;
        for(ALsizei i{1};;)
        {
            for(;i < count;i += 2)
            {
                coeffs = _mm_load_ps(&Coeffs[i+1][0]);
                vals = _mm_load_ps(&Values[Offset][0]);
                imp1 = _mm_mul_ps(lrlr, coeffs);
                imp0 = _mm_shuffle_ps(imp0, imp1, _MM_SHUFFLE(1, 0, 3, 2));
                vals = _mm_add_ps(imp0, vals);
                _mm_store_ps(&Values[Offset][0], vals);
                imp0 = imp1;
                Offset += 2;
            }
            Offset &= HRIR_MASK;
            if(i >= IrSize-1)
                break;
            count = IrSize-1;
        }
        vals = _mm_loadl_pi(vals, reinterpret_cast<__m64*>(&Values[Offset][0]));
        imp0 = _mm_movehl_ps(imp0, imp0);
        vals = _mm_add_ps(imp0, vals);
        _mm_storel_pi(reinterpret_cast<__m64*>(&Values[Offset][0]), vals);
    }
    else
    {
        ALsizei count{mini(IrSize, HRIR_LENGTH - Offset)};
        ASSUME(count >= 2);

        for(ALsizei i{0};;)
        {
            for(;i < count;i += 2)
            {
                __m128 coeffs{_mm_load_ps(&Coeffs[i][0])};
                __m128 vals{_mm_load_ps(&Values[Offset][0])};
                vals = _mm_add_ps(vals, _mm_mul_ps(lrlr, coeffs));
                _mm_store_ps(&Values[Offset][0], vals);
                Offset += 2;
            }
            if(i >= IrSize)
                break;
            Offset = 0;
            count = IrSize;
        }
    }
}

template<>
void MixHrtf_<SSETag>(ALfloat *RESTRICT LeftOut, ALfloat *RESTRICT RightOut, const ALfloat *data,
    ALsizei Offset, const ALsizei OutPos, const ALsizei IrSize, MixHrtfParams *hrtfparams,
    HrtfState *hrtfstate, const ALsizei BufferSize)
{
    MixHrtfBase<ApplyCoeffs>(LeftOut, RightOut, data, Offset, OutPos, IrSize, hrtfparams,
        hrtfstate, BufferSize);
}

template<>
void MixHrtfBlend_<SSETag>(ALfloat *RESTRICT LeftOut, ALfloat *RESTRICT RightOut,
    const ALfloat *data, ALsizei Offset, const ALsizei OutPos, const ALsizei IrSize,
    const HrtfParams *oldparams, MixHrtfParams *newparams, HrtfState *hrtfstate,
    const ALsizei BufferSize)
{
    MixHrtfBlendBase<ApplyCoeffs>(LeftOut, RightOut, data, Offset, OutPos, IrSize, oldparams,
        newparams, hrtfstate, BufferSize);
}

template<>
void MixDirectHrtf_<SSETag>(ALfloat *RESTRICT LeftOut, ALfloat *RESTRICT RightOut,
    const ALfloat (*data)[BUFFERSIZE], DirectHrtfState *State, const ALsizei NumChans,
    const ALsizei BufferSize)
{ MixDirectHrtfBase<ApplyCoeffs>(LeftOut, RightOut, data, State, NumChans, BufferSize); }


template<>
void Mix_<SSETag>(const ALfloat *data, const ALsizei OutChans, ALfloat (*OutBuffer)[BUFFERSIZE],
    ALfloat *CurrentGains, const ALfloat *TargetGains, const ALsizei Counter, const ALsizei OutPos,
    const ALsizei BufferSize)
{
    ASSUME(OutChans > 0);
    ASSUME(BufferSize > 0);

    const ALfloat delta{(Counter > 0) ? 1.0f / static_cast<ALfloat>(Counter) : 0.0f};
    for(ALsizei c{0};c < OutChans;c++)
    {
        ALfloat *RESTRICT dst{al::assume_aligned<16>(&OutBuffer[c][OutPos])};
        ALsizei pos{0};
        ALfloat gain{CurrentGains[c]};
        const ALfloat diff{TargetGains[c] - gain};

        if(std::fabs(diff) > std::numeric_limits<float>::epsilon())
        {
            ALsizei minsize{mini(BufferSize, Counter)};
            const ALfloat step{diff * delta};
            ALfloat step_count{0.0f};
            /* Mix with applying gain steps in aligned multiples of 4. */
            if(LIKELY(minsize > 3))
            {
                const __m128 four4{_mm_set1_ps(4.0f)};
                const __m128 step4{_mm_set1_ps(step)};
                const __m128 gain4{_mm_set1_ps(gain)};
                __m128 step_count4{_mm_setr_ps(0.0f, 1.0f, 2.0f, 3.0f)};
                ALsizei todo{minsize >> 2};
                do {
                    const __m128 val4{_mm_load_ps(&data[pos])};
                    __m128 dry4{_mm_load_ps(&dst[pos])};
#define MLA4(x, y, z) _mm_add_ps(x, _mm_mul_ps(y, z))
                    /* dry += val * (gain + step*step_count) */
                    dry4 = MLA4(dry4, val4, MLA4(gain4, step4, step_count4));
#undef MLA4
                    _mm_store_ps(&dst[pos], dry4);
                    step_count4 = _mm_add_ps(step_count4, four4);
                    pos += 4;
                } while(--todo);
                /* NOTE: step_count4 now represents the next four counts after
                 * the last four mixed samples, so the lowest element
                 * represents the next step count to apply.
                 */
                step_count = _mm_cvtss_f32(step_count4);
            }
            /* Mix with applying left over gain steps that aren't aligned multiples of 4. */
            for(;pos < minsize;pos++)
            {
                dst[pos] += data[pos]*(gain + step*step_count);
                step_count += 1.0f;
            }
            if(pos == Counter)
                gain = TargetGains[c];
            else
                gain += step*step_count;
            CurrentGains[c] = gain;

            /* Mix until pos is aligned with 4 or the mix is done. */
            minsize = mini(BufferSize, (pos+3)&~3);
            for(;pos < minsize;pos++)
                dst[pos] += data[pos]*gain;
        }

        if(!(std::fabs(gain) > GAIN_SILENCE_THRESHOLD))
            continue;
        if(LIKELY(BufferSize-pos > 3))
        {
            ALsizei todo{(BufferSize-pos) >> 2};
            const __m128 gain4{_mm_set1_ps(gain)};
            do {
                const __m128 val4{_mm_load_ps(&data[pos])};
                __m128 dry4{_mm_load_ps(&dst[pos])};
                dry4 = _mm_add_ps(dry4, _mm_mul_ps(val4, gain4));
                _mm_store_ps(&dst[pos], dry4);
                pos += 4;
            } while(--todo);
        }
        for(;pos < BufferSize;pos++)
            dst[pos] += data[pos]*gain;
    }
}

template<>
void MixRow_<SSETag>(ALfloat *OutBuffer, const ALfloat *Gains, const ALfloat (*data)[BUFFERSIZE],
    const ALsizei InChans, const ALsizei InPos, const ALsizei BufferSize)
{
    ASSUME(InChans > 0);
    ASSUME(BufferSize > 0);

    for(ALsizei c{0};c < InChans;c++)
    {
        const ALfloat *RESTRICT src{al::assume_aligned<16>(&data[c][InPos])};
        const ALfloat gain{Gains[c]};
        if(!(std::fabs(gain) > GAIN_SILENCE_THRESHOLD))
            continue;

        ALsizei pos{0};
        if(LIKELY(BufferSize > 3))
        {
            ALsizei todo{BufferSize >> 2};
            const __m128 gain4 = _mm_set1_ps(gain);
            do {
                const __m128 val4{_mm_load_ps(&src[pos])};
                __m128 dry4{_mm_load_ps(&OutBuffer[pos])};
                dry4 = _mm_add_ps(dry4, _mm_mul_ps(val4, gain4));
                _mm_store_ps(&OutBuffer[pos], dry4);
                pos += 4;
            } while(--todo);
        }
        for(;pos < BufferSize;pos++)
            OutBuffer[pos] += src[pos]*gain;
    }
}
