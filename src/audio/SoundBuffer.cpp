
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

#include "SoundBuffer.h"
//#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"
#include "SoundUtil.h"
#include "logger/Logger.h"
#include "stb/stb_vorbis.c"

namespace Nova
{
SoundBuffer::SoundBuffer(const std::string &fileAndPath)
{
    mFileNameAndPath = fileAndPath;
    int *errorNum = nullptr;
    mVorbisStream = nullptr;
    mVorbisStream = stb_vorbis_open_filename(mFileNameAndPath.c_str(), errorNum, NULL);
    if (errorNum != NULL || !mVorbisStream) LOG_ERROR("Can't open vorbis file");

    // stb_vorbis_info vorbisInfo = stb_vorbis_get_info(mVorbisStream);

    int vorbisReturnCode = -1;
    vorbisReturnCode = stb_vorbis_decode_filename(mFileNameAndPath.c_str(), &mNumChannels,
                                                  &mSampleRate, &mSoundData);
    if (vorbisReturnCode == -1)
    {
        LOG_ERROR("Can't decode vorbis file:" << mFileNameAndPath);
    }
    delete mSoundData; // note:should i delete this data??
    if (mNumChannels == 2)
    {
        mSoundFormat = AL_FORMAT_STEREO16;
    }
    else
    {
        mSoundFormat = AL_FORMAT_MONO16;
    }

    // create openAL buffer
    alGenBuffers(1, &mOpenALBufferID);
    CHECK_OPENAL_ERRORS();

    // int const __length_samples = (::stb_vorbis_stream_length_in_samples(__file) *
    // __info.channels);
    mBufferSize = stb_vorbis_stream_length_in_samples(mVorbisStream);
    alBufferData(mOpenALBufferID, mSoundFormat, mSoundData, mBufferSize * 4, mSampleRate);
    CHECK_OPENAL_ERRORS();

    stb_vorbis_close(mVorbisStream);
}
SoundBuffer::~SoundBuffer() {}
} // namespace Nova