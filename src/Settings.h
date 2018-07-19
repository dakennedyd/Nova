
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
#include "Init.h"
#include <unordered_map>

namespace Nova
{
/*this class is created just so that MSVC does not trow a warning*/
struct PropertyValue
{
    std::unordered_map<std::string, std::string> data;
};

/*For storing and retrieving configuration data from ini files*/
class Settings
{
  public:
    Settings() : mFileName(CONFIGURATION_FILE_NAME){};
    /*defines the configuration file*/
    Settings(const std::string &fileName) : mFileName(DEFAULT_FILE_PATH + fileName){};
    virtual ~Settings() = default;

    void add(const std::string &section, const std::string &property, const std::string &value);
    const std::string &getString(const std::string &section, const std::string &property);
    const int getInteger(const std::string &section, const std::string &property);
    const bool getBoolean(const std::string &section, const std::string &property);
    // void remove(const std::string & section, const std::string & property);

    // loads a configuration file if it doesn't exist it creates a new one
    void loadConfFile();
    void saveConfFile();

    // function to check the limits of values loaded from config file
    virtual bool checkLimits(const std::string &section, const std::string &property,
                             const std::string &value)
    {
        return true;
    };

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

class EngineSettings : public Settings, public ISingleton<EngineSettings>, public ISubSystem
{
  public:
    EngineSettings() = default;
    ~EngineSettings() = default;
    void startUp() override;
    void shutDown() override;
    bool checkLimits(const std::string &section, const std::string &property,
                     const std::string &value) override;

  protected:
};
} // namespace Nova
