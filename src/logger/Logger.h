
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
#include "ISingleton.h"
#include "ISubSystem.h"
#include "PreInit.h"
#include "Timer.h"
#include <iostream>

namespace Nova
{
class Logger : public ISingleton<Logger>, public ISubSystem
{
  public:
    Logger() : mLogFile(std::cout){};
    ~Logger(){};
    void startUp() override;
    void shutDown() override;

    void setStream(std::ostream *stream);

    template <typename T> Logger &operator<<(const T &object)
    {
        if (!mOutStream)
        {
            // error("No stream set for Logger class");
            std::cout << "No stream set for Logger class";
            return *this;
        }
        else
        {
            (*mOutStream) << object;
            return *this;
        }
    }

    /// shows the current time in seconds since the logger started
    std::string showTime()
    {
        // if (mShowTime)
        return "[" + std::to_string(Timer::getTimeSinceEngineStart() / 1000.0) + "] ";
        // else
        // return "";
    }

    void openFile()
    {
        // mLogFile.open(mLogFileAndPath, std::ofstream::out |
        // std::ofstream::app);
    }
    void closeFile()
    {
        // mLogFile.close();
    }

  protected:
    // Timer mTimer;
    std::ostream *mOutStream;
    // bool mShowTime = true;			//show time elapsed since
    // application started
    std::string mLogFileAndPath;
    std::ostream &mLogFile;
};

#ifdef LOG_ACTIVE
#    ifdef INFO_LOG_LEVEL_ACTIVE
#        ifdef NOVA_LINUX_PLATFORM
#            define LOG_INFO(msg)                                                                  \
                {                                                                                  \
                    Logger::getInstance().openFile();                                              \
                    Logger::getInstance()                                                          \
                        << "\033[1;92m[INFO]" << Logger::getInstance().showTime() << msg           \
                        << "\033[0m\n";                                                            \
                    Logger::getInstance().closeFile();                                             \
                };
#        else
#            define LOG_INFO(msg)                                                                  \
                {                                                                                  \
                    Logger::getInstance().openFile();                                              \
                    Logger::getInstance()                                                          \
                        << "[INFO]" << Logger::getInstance().showTime() << msg << "\n";            \
                    Logger::getInstance().closeFile();                                             \
                };
#        endif
#    else
#        define LOG_INFO(msg)
#    endif // INFO_LOG_LEVEL_ACTIVE
#    ifdef WARNING_LOG_LEVEL_ACTIVE
#        ifdef NOVA_LINUX_PLATFORM
#            define LOG_WARNING(msg)                                                               \
                {                                                                                  \
                    Logger::getInstance().openFile();                                              \
                    Logger::getInstance()                                                          \
                        << "\033[1;93m[WARNING]" << Logger::getInstance().showTime() << msg        \
                        << "\033[0m\n";                                                            \
                    Logger::getInstance().closeFile();                                             \
                };
#        else
#            define LOG_WARNING(msg)                                                               \
                {                                                                                  \
                    Logger::getInstance().openFile();                                              \
                    Logger::getInstance()                                                          \
                        << "[WARNING]" << Logger::getInstance().showTime() << msg << "\n";         \
                    Logger::getInstance().closeFile();                                             \
                };
#        endif
#    else
#        define LOG_WARNING(msg)
#    endif // WARNING_LOG_LEVEL_ACTIVE
#    ifdef DEBUG_LOG_LEVEL_ACTIVE
#        ifdef NOVA_LINUX_PLATFORM
#            define LOG_DEBUG(msg)                                                                 \
                {                                                                                  \
                    Logger::getInstance().openFile();                                              \
                    Logger::getInstance()                                                          \
                        << "\033[1;96m[DEBUG]" << Logger::getInstance().showTime() << msg          \
                        << "\033[0m\n";                                                            \
                    Logger::getInstance().closeFile();                                             \
                };
#        else
#            define LOG_DEBUG(msg)                                                                 \
                {                                                                                  \
                    Logger::getInstance().openFile();                                              \
                    Logger::getInstance()                                                          \
                        << "[DEBUG]" << Logger::getInstance().showTime() << msg << "\n";           \
                    Logger::getInstance().closeFile();                                             \
                };
#        endif
#    else
#        define LOG_DEBUG(msg)
#    endif // DEBUG_LOG_LEVEL_ACTIVE
#    ifdef ERROR_LOG_LEVEL_ACTIVE
#        ifdef NOVA_LINUX_PLATFORM
#            define LOG_ERROR(msg)                                                                 \
                {                                                                                  \
                    Logger::getInstance().openFile();                                              \
                    Logger::getInstance()                                                          \
                        << "\033[1;91m[ERROR]" << Logger::getInstance().showTime() << msg          \
                        << "\033[0m\n";                                                            \
                    Logger::getInstance().closeFile();                                             \
                };
#        else
#            define LOG_ERROR(msg)                                                                 \
                {                                                                                  \
                    Logger::getInstance().openFile();                                              \
                    Logger::getInstance()                                                          \
                        << "[ERROR]" << Logger::getInstance().showTime() << msg << "\n";           \
                    Logger::getInstance().closeFile();                                             \
                };
#        endif
#    else
#        define LOG_ERROR(msg)
#    endif // ERROR_LOG_LEVEL_ACTIVE
#else
#    define LOG_INFO(msg)
#    define LOG_WARNING(msg)
#    define LOG_DEBUG(msg)
#    define LOG_ERROR(msg)
#endif // LOG_ACTIVE

//
//    end = std::chrono::system_clock::now();
//
//    std::chrono::duration<double> elapsed_seconds = end-start;
//    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
//
//    std::cout << "finished computation at " << std::ctime(&end_time)
//              << "elapsed time: " << elapsed_seconds.count() << "s\n";
} // namespace Nova
