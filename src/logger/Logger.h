#pragma once
#include <iostream>
#include "ISingleton.h"
#include "ISubSystem.h"
#include "Timer.h"
#include "PreInit.h"

namespace Nova {
	class Logger : public ISingleton<Logger>, public ISubSystem
	{
	public:
		Logger() : mLogFile(std::cout){};
		~Logger() {};
		void startUp() override;
		void shutDown() override;
		
		void setStream(std::ostream* stream);
		
		template<typename T>
		Logger& operator << (const T& object)
		{
			if (!mOutStream)
			{ 
				//error("No stream set for Logger class");
				std::cout << "No stream set for Logger class";
				return *this; 
			}
			else {
				(*mOutStream) << object;
				return *this;
			}
		}			

		/*
		* shows the current time in seconds since the logger started
		*/
		std::string showTime()
		{
			//if (mShowTime)
				return "[" + std::to_string(Timer::getTimeSinceEngineStart() / 1000.0) + "] ";
			//else
				//return "";
		}
		
		void openFile()
		{
			//mLogFile.open(mLogFileAndPath, std::ofstream::out | std::ofstream::app);
		}
		void closeFile()
		{
			//mLogFile.close();
		}
	protected:						
		//Timer mTimer;
		std::ostream *mOutStream;
		//bool mShowTime = true;			//show time elapsed since application started
		std::string mLogFileAndPath;
		std::ostream & mLogFile;
	};

#ifdef LOG_ACTIVE	
	#ifdef INFO_LOG_LEVEL_ACTIVE
		#define LOG_INFO(msg) {Logger::getInstance().openFile();\
		Logger::getInstance() << "[INFO]" << Logger::getInstance().showTime() << msg << "\n";\
		Logger::getInstance().closeFile();};
	#else
		#define LOG_INFO(msg)
	#endif //INFO_LOG_LEVEL_ACTIVE
	#ifdef WARNING_LOG_LEVEL_ACTIVE
		#define LOG_WARNING(msg) {Logger::getInstance().openFile();\
		Logger::getInstance() << "[WARNING]" << Logger::getInstance().showTime() << msg << "\n";\
		Logger::getInstance().closeFile();};
	#else
		#define LOG_WARNING(msg)
	#endif //WARNING_LOG_LEVEL_ACTIVE
	#ifdef DEBUG_LOG_LEVEL_ACTIVE
		#define LOG_DEBUG(msg) {Logger::getInstance().openFile();\
		Logger::getInstance() << "[DEBUG]" << Logger::getInstance().showTime() << msg << "\n";\
		Logger::getInstance().closeFile();};
	#else
		#define LOG_DEBUG(msg)
	#endif //DEBUG_LOG_LEVEL_ACTIVE
	#ifdef ERROR_LOG_LEVEL_ACTIVE
		#define LOG_ERROR(msg) {Logger::getInstance().openFile();\
		Logger::getInstance() << "[ERROR]" << Logger::getInstance().showTime() << msg << "\n";\
		Logger::getInstance().closeFile();};
	#else
		#define LOG_ERROR(msg)
	#endif //ERROR_LOG_LEVEL_ACTIVE
#else
	#define LOG_INFO(msg)	
	#define LOG_WARNING(msg)
	#define LOG_DEBUG(msg)
	#define LOG_ERROR(msg)
#endif // LOG_ACTIVE

	

	//
	//    end = std::chrono::system_clock::now();
	//
	//    std::chrono::duration<double> elapsed_seconds = end-start;
	//    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	//
	//    std::cout << "finished computation at " << std::ctime(&end_time)
	//              << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
