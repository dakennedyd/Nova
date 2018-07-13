#pragma once
#include <chrono>
#include <vector>
#include <algorithm>
//temp until engine loads its configuration from a file
namespace Nova {
	static const int NOVA_VERSION_MAJOR = 0;
	static const int NOVA_VERSION_MINOR = 1;
	static const std::string NOVA_VERSION_STAGE("PRE-ALPHA");
	static const std::string NOVA_DESCRIPTION_STRING("NOVA " + std::to_string(NOVA_VERSION_MAJOR) + "." +
		std::to_string(NOVA_VERSION_MINOR) + " " + NOVA_VERSION_STAGE + " ");

	static const std::string DEFAULT_FILE_PATH("");
	static const std::string RESOURCES_PATH("Resources/");
	static const std::string TEXTURES_PATH(RESOURCES_PATH + "textures/");

	static const std::string MODELS_PATH(RESOURCES_PATH + "models/");
	static const std::string LOGGER_OUTPUT_FILE_NAME(DEFAULT_FILE_PATH + "nova_log.txt");
	static const std::string CONFIGURATION_FILE_NAME(DEFAULT_FILE_PATH + "configuration.ini");
	static const std::string RESOURCE_INDEX_FILE_NAME(RESOURCES_PATH + "resource_index.xml");
	static const std::string INVALID_ENTITY_STRING{"NOVA_INVALID_ENTITY"};

	static const unsigned SIMULATION_TIME_STEP = static_cast<unsigned>(1.0 / 100.0 * 1000.0);//100 times a second

	static const std::chrono::time_point<std::chrono::high_resolution_clock> ENGINE_EPOCH = std::chrono::high_resolution_clock::now();

	/*takes an std::vector<string> and converts it to a std::vector<float>*/
	 static std::vector<float> vectorStringToFloat(const std::vector<std::string>& stringVector) {
		std::vector<float> floatVector(stringVector.size());
		std::transform(stringVector.begin(), stringVector.end(), floatVector.begin(), [](const std::string& val)
		{
			return stof(val);
		});
		return floatVector;
	}

	/** replace all ocurrences of string2 in string1 with string3 */
	void replaceAllInPlace(std::string &string1, const std::string &string2, const std::string &string3);

	/** parse tokens in a string and return them as a vector */
	std::vector<std::string> split(const std::string &str, const std::string &delimiter = " ");

	/**
	trim string from start (in place)
	*/
	 static void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	/**
	trim string from end (in place)
	*/
	 static void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	/** 
	trim string from both ends (in place)
	*/
	 static void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}
	
	/*template <bool>
	struct if_constexpr;

	template <>
	struct if_constexpr<true>
	{
		struct elser
		{
			template <class F, class... Args>
			void _else(F&&, Args&&...) {}

			template <bool C, class F, class... Args>
			auto _else_if(F&&, Args&&...) { return elser(); }
		};

		template <class F, class... Args>
		auto operator()(F&& callback, Args&&... args)
		{
			callback(std::forward<Args>(args)...);
			return elser();
		}
	};

	template <>
	struct if_constexpr<false>
	{
		struct elser
		{
			template <bool C, class F, class... Args>
			auto _else_if(F&& callback, Args&&... args)
			{
				return if_constexpr<C>()(std::forward<F>(callback), std::forward<Args>(args)...);
			}

			template <class F, class... Args>
			void _else(F&& callback, Args&&... args)
			{
				callback(std::forward<Args>(args)...);
			}
		};

		template <class F, class... Args>
		auto operator()(F&& callback, Args&&... args)
		{
			return elser();
		}
	};*/
}

