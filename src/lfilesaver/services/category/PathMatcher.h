//
// Created by Pedro Tacla Yamada on 7/8/20.
//

#ifndef FILESAVER_PATHMATCHER_H
#define FILESAVER_PATHMATCHER_H

#include <boost/filesystem.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_set>

namespace filesaver::services
{

struct PathMatch
{
    bool matches;
    bool isRecursive;
};

class PathMatcher
{
public:
    virtual ~PathMatcher () = default;
    virtual PathMatch matches (const boost::filesystem::path& target) = 0;
};

class ExtensionMatcher : public PathMatcher
{
public:
    ExtensionMatcher (const std::unordered_set<std::string>& mExtensions, bool mIsRecursive);

    PathMatch matches (const boost::filesystem::path& target) override;

private:
    std::unordered_set<std::string> m_extensions;
    bool m_isRecursive;
};

class ComponentMatcher : public PathMatcher
{
public:
    ComponentMatcher (std::unordered_set<std::string>  mComponents, bool mIsRecursive);

    PathMatch matches (const boost::filesystem::path& target) override;

private:
    std::unordered_set<std::string> m_components;
    bool m_isRecursive;
};

class FileCategory
{
public:
    FileCategory (std::string name, std::string description, std::string tag);
    virtual ~FileCategory () = default;

    [[nodiscard]] virtual PathMatcher& getMatcher () = 0;

    [[nodiscard]] const std::string& getName () const;
    [[nodiscard]] const std::string& getDescription () const;
    [[nodiscard]] const std::string& getTag () const;

private:
    std::string m_name;
    std::string m_description;
    std::string m_tag;
};

class NodeModulesFileCategory : public FileCategory
{
public:
    NodeModulesFileCategory ();

    ~NodeModulesFileCategory () = default;

    PathMatcher& getMatcher () override;

private:
    ComponentMatcher m_pathMatcher{{"node_modules"}, true};
};

} // namespace filesaver::services

#endif // FILESAVER_PATHMATCHER_H
