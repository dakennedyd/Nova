
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

#pragma once
// #include "AL/al.h"
// #include "AL/alc.h"
// #include "AL/alext.h"
#include "ISingleton.h"
#include "ISubSystem.h"
#include "SoundUtil.h"
//#include "logger/Logger.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "math/Vector.h"
#include <memory>

namespace Nova
{

class Audio final : public ISingleton<Audio>, public ISubSystem
{
    friend SoundBuffer;

  public:
    Audio() = default;
    ~Audio() = default;
    void startUp() override;
    void shutDown() override;
    void setListenerData(const Vec3 &position, const Vec3 &velocity = Vec3(0.0f),
                         const Vec3 &forwardVec = Vec3(0.0f, 0.0f, -1.0f),
                         const Vec3 &upVec = Vec3(0.0f, 1.0f, 0.0f));
    void setListenerPosition(const Vec3 &position);

    void createSoundEmitter(uint64_t entityID);
    void playSound(std::shared_ptr<SoundBuffer> soundBuffer, uint64_t soundSourceID);

  private:
    ALCdevice *mAudioDevice;
    ALCcontext *mAudioContext;
    ALfloat mListenerOrientation[6];
};
} // namespace Nova