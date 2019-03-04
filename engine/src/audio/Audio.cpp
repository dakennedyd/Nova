
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

#include "Audio.h"
#include "ECS/DefaultComponents.h"
#include "SoundBuffer.h"
#include "logger/Logger.h"
//#include "stb/stb_vorbis.c"

namespace Nova
{

void Audio::startUp()
{
    LOG_INFO("Initiating Audio subsystem");
    ALboolean enumeration;
    enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_TRUE) // enumeration supported
    {
        // const ALCchar *devices = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        // // CHECK_OPENAL_ERRORS();
        // const ALCchar *device = devices, *next = devices + 1;
        // size_t len = 0;

        // LOG_INFO("Devices list:");
        // while (device && *device != '\0' && next && *next != '\0')
        // {
        //     LOG_INFO(device);
        //     len = strlen(device);
        //     device += (len + 1);
        //     next += (len + 2);
        // }
    }
    else // enumeration not supported
    {
        LOG_WARNING("OpenAL:device enumeration not supported! using default one");
    }

    // device intialization
    mAudioDevice = alcOpenDevice(NULL); // returns the default device
    if (!mAudioDevice)
    {
        LOG_WARNING("can't open audio device");
    }

    // context initialization
    // ALCint attribs[] = {ALC_HRTF_SOFT, ALC_TRUE, 0};
    // mAudioContext = alcCreateContext(mAudioDevice, attribs);
    mAudioContext = alcCreateContext(mAudioDevice, NULL);
    if (!alcMakeContextCurrent(mAudioContext))
    {
        LOG_ERROR("can't create OpenAL context!")
    }

    // print the default device name
    const ALchar *defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    CHECK_OPENAL_ERRORS();
    LOG_INFO("Default audio device:" << std::string{defaultDeviceName});

    alDistanceModel(AL_EXPONENT_DISTANCE);
    CHECK_OPENAL_ERRORS();

    // creates sound sources pool
    for (int i = 0; i < NUM_SOUND_SOURCES; i++)
    {
        ALuint id;
        alGenSources(1, &id);
        CHECK_OPENAL_ERRORS();
        mAvailableSoundSources.push_back(id);
    }
}

void Audio::shutDown()
{
    for (auto id : mPlayingSoundSources)
    {
        alSourceStop(id); // stops playing sound
        CHECK_OPENAL_ERRORS();
        alSourcei(id, AL_BUFFER, 0); // unbind
        CHECK_OPENAL_ERRORS();
        alDeleteSources(1, &id);
        CHECK_OPENAL_ERRORS();
    }
    for (auto id : mAvailableSoundSources)
    {
        alSourceStop(id); // stops playing sound ?????????
        CHECK_OPENAL_ERRORS();

        alSourcei(id, AL_BUFFER, 0); // unbind
        CHECK_OPENAL_ERRORS();
        alDeleteSources(1, &id);
        CHECK_OPENAL_ERRORS();
    }
    for (auto id : mSoundBuffers)
    {
        alDeleteBuffers(1, &id);
    }

    alcMakeContextCurrent(NULL);
    alcDestroyContext(mAudioContext);

    alcCloseDevice(mAudioDevice);

    // auto device = alcGetContextsDevice(mAudioContext);
    // alcCloseDevice(device);
}

inline ALuint Audio::getAvailableSoundSource()
{
    if (mAvailableSoundSources.empty())
    {
        return 0;
    }
    auto id = mAvailableSoundSources.back();
    mAvailableSoundSources.pop_back();
    mPlayingSoundSources.push_back(id);
    return id;
}

void Audio::setListenerData(const Vec3 &position, const Vec3 &velocity, const Vec3 &forwardVec,
                            const Vec3 &upVec)
{
    alListener3f(AL_POSITION, position.getX(), position.getY(), position.getZ());
    CHECK_OPENAL_ERRORS();

    alListener3f(AL_VELOCITY, velocity.getX(), velocity.getY(), velocity.getZ());
    CHECK_OPENAL_ERRORS();

    mListenerOrientation[0] = forwardVec.getX();
    mListenerOrientation[1] = forwardVec.getY();
    mListenerOrientation[2] = forwardVec.getZ();
    mListenerOrientation[3] = upVec.getX();
    mListenerOrientation[4] = upVec.getY();
    mListenerOrientation[5] = upVec.getZ();
    alListenerfv(AL_ORIENTATION, mListenerOrientation);
    CHECK_OPENAL_ERRORS();
}

void Audio::updateListenerData()
{
    // WARNING: mListenerEntity must not be NULL!!
    alListenerfv(AL_POSITION, mListenerEntity->getTransformStruct().finalTranslation.getDataPtr());
    CHECK_OPENAL_ERRORS();
    // mListenerEntity.getTransformStruct().finalTranslation.debugPrint();

    mListenerOrientation[0] = mListenerEntity->getTransformStruct().forward.getX();
    mListenerOrientation[1] = -mListenerEntity->getTransformStruct().forward.getY();
    mListenerOrientation[2] = mListenerEntity->getTransformStruct().forward.getZ();
    mListenerOrientation[3] = mListenerEntity->getTransformStruct().up.getX();
    mListenerOrientation[4] = -mListenerEntity->getTransformStruct().up.getY();
    mListenerOrientation[5] = mListenerEntity->getTransformStruct().up.getZ();
    alListenerfv(AL_ORIENTATION, mListenerOrientation);
    CHECK_OPENAL_ERRORS();
}

void Audio::updateSoundSource(Entity &entity)
{
    auto &sc = entity.getComponent<SoundComponent>();
    if (sc.soundSourceID == 0) return;
    alSourcefv(sc.soundSourceID, AL_POSITION,
               entity.getTransformStruct().finalTranslation.getDataPtr());
    // CHECK_OPENAL_ERRORS();
}

void Audio::setListenerPosition(const Vec3 &position)
{
    alListener3f(AL_POSITION, position.getX(), position.getY(), position.getZ());
    CHECK_OPENAL_ERRORS();
}

void Audio::playSound(std::shared_ptr<SoundBuffer> soundBuffer, Entity &entity)
{
    if (!soundBuffer) LOG_ERROR("null pointer to sound buffer object")
        {
            if (!entity.containsComponent<SoundComponent>()) // sound source is created when
                                                             // component is added
            {
                LOG_ERROR("Entity doesn't contain a sound component!");
            }
            else
            {
                // auto &ss = getSoundSource(entity.getID());

                // auto sourceID = ss.mOpenALSourceID;
                // // bind source to buffer
                // alSourcei(sourceID, AL_BUFFER, soundBuffer->getOpenALID());
                // CHECK_OPENAL_ERRORS();
                // ss.mBindedBuffer = soundBuffer->getOpenALID(); // keeps track if binded buffer

                // // alSourceQueueBuffers(self->source, 2, self->buffers);
                // alSourcePlay(sourceID);
                // CHECK_OPENAL_ERRORS();

                auto soundSourceID = getAvailableSoundSource();
                if (soundSourceID == 0)
                {
                    LOG_WARNING("can't play sound available sound sources exhausted!");
                }
                else
                {
                    // alSourceStop(soundSourceID);

                    // bind source to buffer
                    alSourcei(soundSourceID, AL_BUFFER, soundBuffer->getOpenALID());
                    CHECK_OPENAL_ERRORS();

                    auto &sc = entity.getComponent<SoundComponent>();
                    sc.soundSourceID = soundSourceID;
                    alSourcef(soundSourceID, AL_PITCH, sc.pitch);
                    // CHECK_OPENAL_ERRORS();
                    alSourcef(soundSourceID, AL_GAIN, sc.gain);
                    // CHECK_OPENAL_ERRORS();
                    alSource3f(soundSourceID, AL_POSITION,
                               entity.getTransformStruct().finalTranslation.getDataPtr()[0],
                               entity.getTransformStruct().finalTranslation.getDataPtr()[1],
                               entity.getTransformStruct().finalTranslation.getDataPtr()[2]);
                    // CHECK_OPENAL_ERRORS();
                    // alSource3f(soundSourceID, AL_VELOCITY, 0, 0, 0);
                    // CHECK_OPENAL_ERRORS();
                    alSourcei(soundSourceID, AL_LOOPING, sc.looped ? AL_TRUE : AL_FALSE);
                    // CHECK_OPENAL_ERRORS();

                    alSourcePlay(soundSourceID);
                    CHECK_OPENAL_ERRORS();
                }
            }
        }
}

void Audio::stopSound(Entity &entity)
{
    if (!entity.containsComponent<SoundComponent>()) // sound source is created when
                                                     // component is added
    {
        LOG_ERROR("Entity doesn't contain a sound component!");
    }
    else
    {
        auto &sc = entity.getComponent<SoundComponent>();
        alSourceStop(sc.soundSourceID);
        CHECK_OPENAL_ERRORS();
    }
}

void Audio::cleanUpSources()
{

    /*
    Possible values of state
    AL_INITIAL
    AL_STOPPED
    AL_PLAYING
    AL_PAUSED
    */
    mPlayingSoundSources.erase(std::remove_if(mPlayingSoundSources.begin(),
                                              mPlayingSoundSources.end(),
                                              [&](ALuint id) {
                                                  ALint state;
                                                  alGetSourcei(id, AL_SOURCE_STATE, &state);
                                                  CHECK_OPENAL_ERRORS();
                                                  if (state == AL_STOPPED)
                                                  {
                                                      mAvailableSoundSources.push_back(id);
                                                      return true;
                                                  }
                                                  else
                                                  {
                                                      return false;
                                                  }
                                              }),
                               mPlayingSoundSources.end());

    // erase remove idiom
    // mPlayingSoundSources.erase(std::remove(mPlayingSoundSources.begin(),
    // mPlayingSoundSources.end(), it),mPlayingSoundSources.end());

    // mAvailableSoundSources.insert(mAvailableSoundSources.end(), stoppedAources.begin(),
    //                               stoppedAources.end());
}

} // namespace Nova