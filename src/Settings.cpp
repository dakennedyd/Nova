#include <string>
#include <stdexcept>
#include <fstream>
#include <algorithm>
#include "Init.h"
#include "Settings.h"
#include "Error.h"
#include "Settings.h"
#include "logger/Logger.h"

namespace Nova {	
	int convertStringToInt(const std::string &str)
	{
		int number = 0;
		try { number = std::stoi(str); }
		catch (const std::invalid_argument e) { error(e.what()); }//TODO: get rid of exceptions!
		catch (const std::out_of_range e) { error(e.what()); }
		return number;
	}
	//std::unordered_map<std::string, PropertyValue> Settings::mSettingsData;
	void Settings::add(const std::string & section, const std::string & property, const std::string & value)
	{
		mSettingsData[section].data[property] = value;
	}

	const std::string & Settings::getString(const std::string & section, const std::string & property)
	{
		const auto &sectionBlock = mSettingsData.find(section);
		if (sectionBlock != mSettingsData.end()) //does section exist?
		{
			const auto & keyValue = sectionBlock->second.data.find(property);
			if (keyValue != sectionBlock->second.data.end()) //does property exist?
			{
				return keyValue->second; //returns the value
			}
			else {
				//LOG_WARNING("Tried to get property \"" << property << "\" from configuration file:" << mFileName <<". Property does not exist");
				return mNoDataStr;
			}
		}
		else {
			//LOG_WARNING("Tried to get section \"" << section << "\" from configuration file:" << mFileName <<". Section does not exist");
			return mNoDataStr;
		}

	}

	const int Settings::getInteger(const std::string & section, const std::string & property)
	{
		return convertStringToInt(getString(section, property));		
	}

	const bool Settings::getBoolean(const std::string & section, const std::string & property)
	{		
		if (getString(section, property) == "yes" ||
			getString(section, property) == "true" ||
			getString(section, property) == "1")
		{
			return true;
		}
		else {
			return false;
		}		
	}

	void Settings::loadConfFile()
	{
		std::string line, currentSection;
		std::ifstream infile(mFileAndPath);
		LOG_INFO("Loading Configuration file:" << mFileAndPath);
		if (!infile.is_open())
		{			
			LOG_WARNING("Can't open configuration file:" << mFileName << " creating a new one");
			saveConfFile();
			infile.open(mFileAndPath, std::ofstream::in);
		}				
		//clear();
		while (std::getline(infile, line))
		{
			trim(line);
			if (line.substr(0, 1) == mCommentMarker) continue; //skip comments
			if (line.empty()) continue; //skip empty strings
			if (line.substr(0, 1) == mSectionDelimiterStart && line.substr(line.length()-1, line.length()) == mSectionDelimiterEnd)
			{
				currentSection = line.substr(1, line.length()-2);
				continue;
			}
			size_t position = line.find(mDelimiter);
			if (position == std::string::npos)
			{
				LOG_WARNING("Malformed expression \"" << line << "\" in " << mFileName << " skipping");
				continue;
			}
			std::transform(line.begin(), line.end(), line.begin(), ::tolower);
			std::string property = line.substr(0, position);
			trim(property);
			std::string value = line.substr(position + mDelimiter.length(), std::string::npos);
			trim(value);

			//LOG_DEBUG(currentSection << "->" << property << "=" << value);
			if (checkLimits(currentSection, property, value))
			{
				add(currentSection, property, value);
			}
			else {
				LOG_WARNING("value:" << value << "from:" << currentSection << ":" << property << " is invalid. using default");
			}
		}		
	}
	void Settings::saveConfFile()
	{
		std::ofstream file;		
		file.open(mFileAndPath, std::ofstream::out | std::ofstream::trunc);
		file << "#AUTO-GENERATED CONFIGURATION FILE BY "<< NOVA_DESCRIPTION_STRING <<" DO NOT EDIT!. " << std::endl;
		for (auto & sectionData : mSettingsData)
		{
			file << std::endl << mSectionDelimiterStart << sectionData.first << mSectionDelimiterEnd << std::endl;
			for (auto & propertyValue : sectionData.second.data)
			{
				file << propertyValue.first << mDelimiter << propertyValue.second << std::endl;
			}
		}
		file.close();
	}
	void Settings::clear()
	{
		mSettingsData.clear();
	}
	
	void EngineSettings::startUp()
	{
		mFileName = CONFIGURATION_FILE_NAME;
		mFileAndPath = DEFAULT_FILE_PATH + mFileName;

		//sets default values, this values are overridden
		//by CONFIGURATION_FILE_NAME if appropriate
		add("Video", "width", "800");
		add("Video", "height", "600");
		add("Video", "fullscreen", "no");
		add("Video", "fps", "30");

		add("Input", "mouse_sensitivity", "30");
		
		loadConfFile();
	}
	void EngineSettings::shutDown()
	{
		saveConfFile();
	}
	
	bool EngineSettings::checkLimits(const std::string & section, const std::string & property, const std::string & value)
	{		
		//TODO: limits should be stored in a hashmap for performance reasons


		if (section == "Video" && (property == "width" || property == "height"))
		{			
			int val = convertStringToInt(value);
			if (val < 100 || val > 16000) return false;
		}
		if (section == "Video" && property == "fps")
		{
			int val = convertStringToInt(value);
			if (val < 1 || val > 100) return false;
		}
		return true;
	}
}