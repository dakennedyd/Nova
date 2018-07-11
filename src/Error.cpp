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
		//LOG_ERROR(msg);
		std::cout << "**FATAL ERROR** " << msg << "\n";
		int n = 0; std::cin >> n; //just introduces a pause to read the error message
		//Application::getInstance().shutDown();
		std::exit(EXIT_FAILURE);
	}
}