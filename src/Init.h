
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
#include "linux/FileSystem.h"
#include <chrono>
#include <string>

namespace Nova
{
static const int NOVA_VERSION_MAJOR = 0;
static const int NOVA_VERSION_MINOR = 2;
static const int NOVA_VERSION_REVISION = 0;
static const std::string NOVA_VERSION_STAGE("PRE-ALPHA");
static const std::string NOVA_DESCRIPTION_STRING("NOVA " + std::to_string(NOVA_VERSION_MAJOR) +
                                                 "." + std::to_string(NOVA_VERSION_MINOR) + "." +
                                                 std::to_string(NOVA_VERSION_REVISION) + " " +
                                                 NOVA_VERSION_STAGE + " ");

static const std::string PATH_TO_BINARY(FileSystem::getExecutablePath());
static const std::string PATH_TO_ENGINE_BINARY(FileSystem::getExecutablePath() + "engine/");
static const std::string RESOURCES_PATH("Resources/");
static const std::string TEXTURES_PATH(RESOURCES_PATH + "textures/");
static const std::string MODELS_PATH(RESOURCES_PATH + "models/");
static const std::string SOUNDS_PATH(RESOURCES_PATH + "sounds/");

static const std::string LOGGER_OUTPUT_FILE_NAME("nova_log.txt");
static const std::string CONFIGURATION_FILE_NAME("configuration.ini");
static const std::string RESOURCE_INDEX_FILE_NAME(RESOURCES_PATH + "resource_index.xml");
static const std::string INVALID_ENTITY_STRING{"NOVA_INVALID_ENTITY"};

static const unsigned SIMULATION_TIME_STEP =
    static_cast<unsigned>(1.0 / 100.0 * 1000.0); // 100 times a second

static const std::chrono::time_point<std::chrono::high_resolution_clock> ENGINE_EPOCH =
    std::chrono::high_resolution_clock::now();

} // namespace Nova
