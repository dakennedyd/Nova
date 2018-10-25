
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
//#include "SoundBuffer.h"
#include "ECS/Entity.h"
#include "math/Vector.h"
#include <deque>
#include <memory>
#include <vector>

namespace Nova
{
class SoundBuffer;
class Audio final : public ISingleton<Audio>, public ISubSystem
{
    // friend SoundBuffer;
    // friend class SoundSource;
    friend class SoundSystem;
    friend class Application;
    friend class SoundBuffer;

  public:
    Audio() = default;
    ~Audio() = default;
    void startUp() override;
    void shutDown() override;

    /**
     * @brief Sets the audio listener's parameters like it's position, velocity and orientation
     *
     * @param position
     * @param velocity
     * @param forwardVec
     * @param upVec
     */
    void setListenerData(const Vec3 &position, const Vec3 &velocity = Vec3(0.0f),
                         const Vec3 &forwardVec = Vec3(0.0f, 0.0f, -1.0f),
                         const Vec3 &upVec = Vec3(0.0f, 1.0f, 0.0f));

    /**
     * @brief assigns an entity as the current listener
     *
     * @param entity
     */
    void assignListener(Entity &entity) { mListenerEntity = &entity; };
    void setListenerPosition(const Vec3 &position);

    void playSound(std::shared_ptr<SoundBuffer> soundBuffer, Entity &entity);

  private:
    void setSoundSourceData(const ALuint soundSourceID, const bool looped, const float pitch,
                            const float gain, const float position[3], const float velocity[3]);
    ALuint getAvailableSoundSource();

    void registerSoundBuffer(const ALuint id) { mSoundBuffers.push_back(id); }
    void updateListenerData();
    void updateSoundSource(Entity &entity);

    // moves the IDs of sound sources that are not playing anymore to the avalilable sources pile
    void cleanUpSources();
    ALCdevice *mAudioDevice;
    ALCcontext *mAudioContext;
    ALfloat mListenerOrientation[6];

    std::deque<ALuint> mAvailableSoundSources;
    std::vector<ALuint> mPlayingSoundSources;
    std::vector<ALuint> mSoundBuffers;

    Entity *mListenerEntity;
};
} // namespace Nova