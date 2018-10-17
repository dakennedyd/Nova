
// The MIT License (MIT)

// Copyright (c) 2018 David Kennedy

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

#include "SoundSource.h"
#include "SoundUtil.h"

namespace Nova
{
SoundSource::SoundSource(Entity &entity, bool looped, const float gain, const float pitch)
    : mLooped(looped), mGain(gain), mPitch(pitch)
{
    mPosition[0] = &entity.getTransformStruct().finalTranslation.getDataPtr()[0];
    mPosition[1] = &entity.getTransformStruct().finalTranslation.getDataPtr()[1];
    mPosition[2] = &entity.getTransformStruct().finalTranslation.getDataPtr()[2];
    // mID = entity.getID();

    alGenSources(1, &mOpenALSourceID);
    CHECK_OPENAL_ERRORS();
    alSourcef(mOpenALSourceID, AL_PITCH, mPitch);
    CHECK_OPENAL_ERRORS();
    alSourcef(mOpenALSourceID, AL_GAIN, mGain);
    CHECK_OPENAL_ERRORS();
    alSource3f(mOpenALSourceID, AL_POSITION, *mPosition[0], *mPosition[1], *mPosition[2]);
    CHECK_OPENAL_ERRORS();
    // alSource3f(mOpenALSourceID, AL_VELOCITY, 0, 0, 0);
    // CHECK_OPENAL_ERRORS();
    int l;
    if (mLooped) l = 1;
    if (!mLooped) l = 0;
    alSourcei(mOpenALSourceID, AL_LOOPING, l);
    CHECK_OPENAL_ERRORS();
}
SoundSource::~SoundSource() {}
} // namespace Nova