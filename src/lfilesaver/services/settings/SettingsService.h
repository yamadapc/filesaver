//
// Created by Pedro Tacla Yamada on 24/5/20.
//


#pragma once

#include <boost/filesystem.hpp>
#include <fstream>
#include <optional>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace filesaver::services::settings
{

class SettingsService
{
public:
    explicit SettingsService (boost::filesystem::path settingsPath);

    bool loadSettings ();

    bool saveSettings ();

    template <typename T> std::optional<T> get (const std::string& key)
    {
        try
        {
            auto value = m_store[key];
            return value.as<T> ();
        }
        catch (const YAML::BadSubscript&)
        {
            return {};
        }
        catch (const YAML::BadConversion&)
        {
            return {};
        }
    }

    template <typename T> void set (const std::string& key, T value)
    {
        m_store[key] = value;
    }

    static SettingsService defaultForMac ();

private:
    YAML::Node m_store;
    boost::filesystem::path m_settingsPath;
};

} // namespace filesaver::services::settings
