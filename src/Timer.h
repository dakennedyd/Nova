#pragma once
//#include <cstddef>
//#include <cstdint>
//#include <sys/types.h>
//#include <cstdlib>
//#include <chrono>
#include "Init.h"

namespace Nova {
	class Timer
	{
	public:
		Timer() { reset(); };
		~Timer() = default;
		inline void reset()
		{
			mEpoch = std::chrono::high_resolution_clock::now();
		}
		inline std::int64_t getMicro()
		{
			std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::microseconds>(now - mEpoch).count();			
		}
		inline std::int64_t getMillis()
		{
			std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
			//std::chrono::duration<int, std::milli> r = now - mEpoch;
			return std::chrono::duration_cast<std::chrono::milliseconds>(now - mEpoch).count();
		}
		inline std::int64_t getSecs()
		{
			std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::seconds>(now - mEpoch).count();
		}

		/*gets the time since the engine started in milliseconds*/
		static std::int64_t getTimeSinceEngineStart()
		{
			std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(now - ENGINE_EPOCH).count();
		};		
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> mEpoch;
	};	
}
