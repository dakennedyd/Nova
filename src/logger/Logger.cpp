#include "logger/Logger.h"
#include "Init.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>

namespace Nova
{
void Logger::startUp()
{
    mLogFileAndPath = LOGGER_OUTPUT_FILE_NAME;
    mOutStream = &mLogFile;
    // mOutStream = &std::cout;
    std::chrono::time_point<std::chrono::system_clock> now(std::chrono::system_clock::now());
    std::time_t startUpTime = std::chrono::system_clock::to_time_t(now);
    std::string timeAndDateString(std::ctime(&startUpTime));
    // mLogFile.open(mLogFileAndPath, std::ofstream::out |
    // std::ofstream::trunc);
    (*mOutStream) << NOVA_DESCRIPTION_STRING;
    (*mOutStream) << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
                  << " Log started " << timeAndDateString;
    // mLogFile.close();
}

void Logger::shutDown()
{
    LOG_INFO("Logger Shutdown");
    // mLogFile.close();
}

void Logger::setStream(std::ostream *stream) { mOutStream = stream; }

} // namespace Nova
