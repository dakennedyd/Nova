#ifndef _AL_EFFECT_H_
#define _AL_EFFECT_H_

#include "alMain.h"


struct ALeffect;

enum {
    EAXREVERB_EFFECT = 0,
    REVERB_EFFECT,
    AUTOWAH_EFFECT,
    CHORUS_EFFECT,
    COMPRESSOR_EFFECT,
    DISTORTION_EFFECT,
    ECHO_EFFECT,
    EQUALIZER_EFFECT,
    FLANGER_EFFECT,
    FSHIFTER_EFFECT,
    MODULATOR_EFFECT,
    PSHIFTER_EFFECT,
    DEDICATED_EFFECT,

    MAX_EFFECTS
};
extern ALboolean DisabledEffects[MAX_EFFECTS];

extern ALfloat ReverbBoost;

struct EffectList {
    const char name[16];
    int type;
    ALenum val;
};
extern const EffectList gEffectList[14];

struct ALeffectVtable {
    void (*const setParami)(ALeffect *effect, ALCcontext *context, ALenum param, ALint val);
    void (*const setParamiv)(ALeffect *effect, ALCcontext *context, ALenum param, const ALint *vals);
    void (*const setParamf)(ALeffect *effect, ALCcontext *context, ALenum param, ALfloat val);
    void (*const setParamfv)(ALeffect *effect, ALCcontext *context, ALenum param, const ALfloat *vals);

    void (*const getParami)(const ALeffect *effect, ALCcontext *context, ALenum param, ALint *val);
    void (*const getParamiv)(const ALeffect *effect, ALCcontext *context, ALenum param, ALint *vals);
    void (*const getParamf)(const ALeffect *effect, ALCcontext *context, ALenum param, ALfloat *val);
    void (*const getParamfv)(const ALeffect *effect, ALCcontext *context, ALenum param, ALfloat *vals);
};

#define DEFINE_ALEFFECT_VTABLE(T)           \
const ALeffectVtable T##_vtable = {         \
    T##_setParami, T##_setParamiv,          \
    T##_setParamf, T##_setParamfv,          \
    T##_getParami, T##_getParamiv,          \
    T##_getParamf, T##_getParamfv,          \
}

extern const ALeffectVtable ALeaxreverb_vtable;
extern const ALeffectVtable ALreverb_vtable;
extern const ALeffectVtable ALautowah_vtable;
extern const ALeffectVtable ALchorus_vtable;
extern const ALeffectVtable ALcompressor_vtable;
extern const ALeffectVtable ALdistortion_vtable;
extern const ALeffectVtable ALecho_vtable;
extern const ALeffectVtable ALequalizer_vtable;
extern const ALeffectVtable ALflanger_vtable;
extern const ALeffectVtable ALfshifter_vtable;
extern const ALeffectVtable ALmodulator_vtable;
extern const ALeffectVtable ALnull_vtable;
extern const ALeffectVtable ALpshifter_vtable;
extern const ALeffectVtable ALdedicated_vtable;


union ALeffectProps {
    struct {
        // Shared Reverb Properties
        ALfloat Density;
        ALfloat Diffusion;
        ALfloat Gain;
        ALfloat GainHF;
        ALfloat DecayTime;
        ALfloat DecayHFRatio;
        ALfloat ReflectionsGain;
        ALfloat ReflectionsDelay;
        ALfloat LateReverbGain;
        ALfloat LateReverbDelay;
        ALfloat AirAbsorptionGainHF;
        ALfloat RoomRolloffFactor;
        ALboolean DecayHFLimit;

        // Additional EAX Reverb Properties
        ALfloat GainLF;
        ALfloat DecayLFRatio;
        ALfloat ReflectionsPan[3];
        ALfloat LateReverbPan[3];
        ALfloat EchoTime;
        ALfloat EchoDepth;
        ALfloat ModulationTime;
        ALfloat ModulationDepth;
        ALfloat HFReference;
        ALfloat LFReference;
    } Reverb;

    struct {
        ALfloat AttackTime;
        ALfloat ReleaseTime;
        ALfloat Resonance;
        ALfloat PeakGain;
    } Autowah;

    struct {
        ALint Waveform;
        ALint Phase;
        ALfloat Rate;
        ALfloat Depth;
        ALfloat Feedback;
        ALfloat Delay;
    } Chorus; /* Also Flanger */

    struct {
        ALboolean OnOff;
    } Compressor;

    struct {
        ALfloat Edge;
        ALfloat Gain;
        ALfloat LowpassCutoff;
        ALfloat EQCenter;
        ALfloat EQBandwidth;
    } Distortion;

    struct {
        ALfloat Delay;
        ALfloat LRDelay;

        ALfloat Damping;
        ALfloat Feedback;

        ALfloat Spread;
    } Echo;

    struct {
        ALfloat LowCutoff;
        ALfloat LowGain;
        ALfloat Mid1Center;
        ALfloat Mid1Gain;
        ALfloat Mid1Width;
        ALfloat Mid2Center;
        ALfloat Mid2Gain;
        ALfloat Mid2Width;
        ALfloat HighCutoff;
        ALfloat HighGain;
    } Equalizer;

    struct {
        ALfloat Frequency;
        ALint LeftDirection;
        ALint RightDirection;
    } Fshifter;

    struct {
        ALfloat Frequency;
        ALfloat HighPassCutoff;
        ALint Waveform;
    } Modulator;

    struct {
        ALint CoarseTune;
        ALint FineTune;
    } Pshifter;

    struct {
        ALfloat Gain;
    } Dedicated;
};

struct ALeffect {
    // Effect type (AL_EFFECT_NULL, ...)
    ALenum type{AL_EFFECT_NULL};

    ALeffectProps Props{};

    const ALeffectVtable *vtab{nullptr};

    /* Self ID */
    ALuint id{0u};
};
#define ALeffect_setParami(o, c, p, v)   ((o)->vtab->setParami(o, c, p, v))
#define ALeffect_setParamf(o, c, p, v)   ((o)->vtab->setParamf(o, c, p, v))
#define ALeffect_setParamiv(o, c, p, v)  ((o)->vtab->setParamiv(o, c, p, v))
#define ALeffect_setParamfv(o, c, p, v)  ((o)->vtab->setParamfv(o, c, p, v))
#define ALeffect_getParami(o, c, p, v)   ((o)->vtab->getParami(o, c, p, v))
#define ALeffect_getParamf(o, c, p, v)   ((o)->vtab->getParamf(o, c, p, v))
#define ALeffect_getParamiv(o, c, p, v)  ((o)->vtab->getParamiv(o, c, p, v))
#define ALeffect_getParamfv(o, c, p, v)  ((o)->vtab->getParamfv(o, c, p, v))

inline ALboolean IsReverbEffect(ALenum type)
{ return type == AL_EFFECT_REVERB || type == AL_EFFECT_EAXREVERB; }

void InitEffect(ALeffect *effect);

void LoadReverbPreset(const char *name, ALeffect *effect);

#endif
