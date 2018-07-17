#pragma once
#include <chrono>
#include <string>

namespace Nova
{
static const int NOVA_VERSION_MAJOR = 0;
static const int NOVA_VERSION_MINOR = 1;
static const std::string NOVA_VERSION_STAGE("PRE-ALPHA");
static const std::string NOVA_DESCRIPTION_STRING("NOVA " + std::to_string(NOVA_VERSION_MAJOR) +
                                                 "." + std::to_string(NOVA_VERSION_MINOR) + " " +
                                                 NOVA_VERSION_STAGE + " ");

static const std::string DEFAULT_FILE_PATH("");
static const std::string RESOURCES_PATH("Resources/");
static const std::string TEXTURES_PATH(RESOURCES_PATH + "textures/");

static const std::string MODELS_PATH(RESOURCES_PATH + "models/");
static const std::string LOGGER_OUTPUT_FILE_NAME(DEFAULT_FILE_PATH + "nova_log.txt");
static const std::string CONFIGURATION_FILE_NAME(DEFAULT_FILE_PATH + "configuration.ini");
static const std::string RESOURCE_INDEX_FILE_NAME(RESOURCES_PATH + "resource_index.xml");
static const std::string INVALID_ENTITY_STRING{"NOVA_INVALID_ENTITY"};

static const unsigned SIMULATION_TIME_STEP =
    static_cast<unsigned>(1.0 / 100.0 * 1000.0); // 100 times a second

static const std::chrono::time_point<std::chrono::high_resolution_clock> ENGINE_EPOCH =
    std::chrono::high_resolution_clock::now();

} // namespace Nova
