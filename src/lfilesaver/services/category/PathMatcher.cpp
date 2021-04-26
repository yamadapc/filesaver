//
// Created by Pedro Tacla Yamada on 7/8/20.
//

#include "PathMatcher.h"

namespace filesaver::services
{

ExtensionMatcher::ExtensionMatcher (const std::unordered_set<std::string>& mExtensions, bool mIsRecursive)
    : m_extensions (mExtensions), m_isRecursive (mIsRecursive)
{
}

PathMatch ExtensionMatcher::matches (const boost::filesystem::path& target)
{
    auto matches = m_extensions.find (target.extension ().string ()) != m_extensions.end ();
    return {matches, m_isRecursive};
}

ComponentMatcher::ComponentMatcher (std::unordered_set<std::string> mComponents, bool mIsRecursive)
    : m_components (std::move (mComponents)), m_isRecursive (mIsRecursive)
{
}

PathMatch ComponentMatcher::matches (const boost::filesystem::path& target)
{
    auto matches = m_components.find (target.filename ().string ()) != m_components.end ();

    if (matches && m_isRecursive)
    {
        auto currentPath = target.parent_path ();
        while (!currentPath.empty ())
        {
            if (m_components.find (currentPath.filename ().string ()) != m_components.end ())
            {
                return {false, m_isRecursive};
            }
            currentPath = currentPath.remove_filename ();
        }
    }

    return {matches, m_isRecursive};
}

FileCategory::FileCategory (std::string name, std::string description, std::string tag)
    : m_name (std::move (name)), m_description (std::move (description)), m_tag (std::move (tag))
{
}

const std::string& FileCategory::getName () const
{
    return m_name;
}

const std::string& FileCategory::getDescription () const
{
    return m_description;
}

const std::string& FileCategory::getTag () const
{
    return m_tag;
}

ComponentsCategory::ComponentsCategory (std::string name,
                                        std::string description,
                                        std::string tag,
                                        std::unordered_set<std::string> components,
                                        bool isRecursive)
    : FileCategory (name, description, tag), m_pathMatcher (components, isRecursive)
{
}

PathMatcher& ComponentsCategory::getMatcher ()
{
    return m_pathMatcher;
}

ExtensionCategory::ExtensionCategory (std::string name,
                                      std::string description,
                                      std::string tag,
                                      std::unordered_set<std::string> extension,
                                      bool isRecursive)
    : FileCategory (name, description, tag), m_pathMatcher (extension, isRecursive)
{
}

PathMatcher& ExtensionCategory::getMatcher ()
{
    return m_pathMatcher;
}

} // namespace filesaver::services
