//
// Created by Pedro Tacla Yamada on 2019-09-10.
//

#ifndef FILESAVER_SETTINGS_H
#define FILESAVER_SETTINGS_H

#include <optional>
#include <string>
#include <vector>

namespace {

class Settings {
public:
  unsigned int numberOfWorkerThreads;
  std::vector<std::string> targets;

  struct SettingItem {
  public:
    std::string id;
    std::string name;
    std::string description;
  };

  struct CommandLineSetting : SettingItem {
    std::string commandLineFlag;
    std::optional<std::string> commandLineDescription;
  };

  enum class StorageType {};
};

} // namespace

#endif // FILESAVER_SETTINGS_H
