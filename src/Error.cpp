#include <fstream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include "Error.h"
#include "Timer.h"
#include "logger/Logger.h"

namespace Nova {	
    void error(const std::string &msg)
	{
		LOG_ERROR(msg);
		//Application::getInstance().shutDown();
		std::exit(EXIT_FAILURE);
	}
}