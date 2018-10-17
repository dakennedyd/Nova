
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
    // CHECK_OPENAL_ERRORS();
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
    mAudioContext = alcCreateContext(mAudioDevice, NULL);
    if (!alcMakeContextCurrent(mAudioContext))
    {
        LOG_ERROR("can't create OpenAL context!")
    }

    // print the default device name
    const ALchar *defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    CHECK_OPENAL_ERRORS();
    LOG_INFO("Default audio device:" << std::string{defaultDeviceName});
}

void Audio::shutDown()
{
    // alDeleteSources(1, &source);
    // alDeleteBuffers(1, &buffer);
    // device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(mAudioContext);
    alcCloseDevice(mAudioDevice);
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

void Audio::setListenerPosition(const Vec3 &position)
{
    alListener3f(AL_POSITION, position.getX(), position.getY(), position.getZ());
    CHECK_OPENAL_ERRORS();
}

void Audio::playSound(std::shared_ptr<SoundBuffer> soundBuffer, uint64_t soundSourceID)
{
    if (!soundBuffer) LOG_ERROR("null pointer to sound buffer object");

    // bind source to buffer
    alSourcei(soundSourceID, AL_BUFFER, soundBuffer->getOpenALID());
    CHECK_OPENAL_ERRORS();

    // alSourceQueueBuffers(self->source, 2, self->buffers);
    alSourcePlay(soundSourceID);
    CHECK_OPENAL_ERRORS();
}
} // namespace Nova