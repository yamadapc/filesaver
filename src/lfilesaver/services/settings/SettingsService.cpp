//
// Created by Pedro Tacla Yamada on 24/5/20.
//

#include "SettingsService.h"

namespace filesaver::services::settings
{

SettingsService::SettingsService (boost::filesystem::path settingsPath) : m_settingsPath (settingsPath)
{
}

bool SettingsService::loadSettings ()
{
    if (boost::filesystem::exists (m_settingsPath))
    {
        try
        {
            m_store = YAML::LoadFile (m_settingsPath.string ());
            return true;
        }
        catch (const YAML::BadFile&)
        {
            return false;
        }
        catch (const YAML::ParserException&)
        {
            return false;
        }
    }

    return false;
}

bool SettingsService::saveSettings ()
{
    std::ofstream settingsFile{m_settingsPath.string ()};
    YAML::Emitter emitter (settingsFile);
    emitter << m_store;
    return true;
}

SettingsService SettingsService::defaultForMac ()
{
    auto *homeDirStr = getenv ("HOME");
    boost::filesystem::path homeDirPath{homeDirStr};
    boost::filesystem::path applicationSupportDir{homeDirPath};
    applicationSupportDir.append ("Library").append ("Application Support").append ("filesaver");
    boost::filesystem::path settingsPath{applicationSupportDir};
    settingsPath.append ("default-settings.yaml");
    boost::filesystem::create_directories (applicationSupportDir);

    return SettingsService (settingsPath);
}

} // namespace filesaver::services::settings
