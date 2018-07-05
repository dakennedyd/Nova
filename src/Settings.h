#pragma once
#include <unordered_map>
#include "ISingleton.h"
#include "ISubSystem.h"

namespace Nova {
	/*this class is created just so that MSVC does not trow a warning*/
	struct PropertyValue
	{
		std::unordered_map<std::string, std::string> data;
	};

	/*For storing and retrieving configuration data from ini files*/
	class Settings
	{
	public:
		Settings() :mFileName(CONFIGURATION_FILE_NAME) {};
		/*defines the configuration file*/
		Settings(const std::string & fileName) :mFileName(DEFAULT_FILE_PATH + fileName) {};
		~Settings() = default;

		void add(const std::string & section, const std::string & property, const std::string & value);
		const std::string & getString(const std::string & section, const std::string & property);
		const int getInteger(const std::string & section, const std::string & property);
		const bool getBoolean(const std::string & section, const std::string & property);
		//void remove(const std::string & section, const std::string & property);
		void loadConfFile();	//loads a configuration file if it doesn't exist it creates a new one
		void saveConfFile();

		//function to check the limits of values loaded from config file		 
		virtual bool checkLimits(const std::string & section, const std::string & property, const std::string & value) { return true; };
	protected:
		void clear();				
		std::unordered_map<std::string, PropertyValue> mSettingsData;
		std::string mFileName;
		std::string mFileAndPath = DEFAULT_FILE_PATH + mFileName;
		const std::string mDelimiter = "=";
		const std::string mCommentMarker = "#";
		const std::string mSectionDelimiterStart = "[";
		const std::string mSectionDelimiterEnd = "]";
		const std::string mNoDataStr = "NO DATA";
	};

	class EngineSettings :public Settings, public ISingleton<EngineSettings>, public ISubSystem
	{
	public:
		EngineSettings() = default;
		~EngineSettings() = default;
		void startUp() override;
		void shutDown() override;
		bool checkLimits(const std::string & section, const std::string & property, const std::string & value) override;
	protected:

	};
}
