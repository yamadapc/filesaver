//
// Created by Pedro Tacla Yamada on 17/7/20.
//

#include "FileCategoryService.h"

namespace filesaver::services
{

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

ExtensionMatcher::ExtensionMatcher (const std::unordered_set<std::string>& mExtensions, bool mIsRecursive)
    : m_extensions (mExtensions), m_isRecursive (mIsRecursive)
{
}

PathMatch ExtensionMatcher::matches (const boost::filesystem::path& target)
{
    auto matches = m_extensions.find (target.extension ().string ()) != m_extensions.end ();
    return {matches, m_isRecursive};
}

ComponentMatcher::ComponentMatcher (const std::unordered_set<std::string>& mComponents, bool mIsRecursive)
    : m_components (mComponents), m_isRecursive (mIsRecursive)
{
}

PathMatch ComponentMatcher::matches (const boost::filesystem::path& target)
{
    auto matches = m_components.find (target.filename ().string ()) != m_components.end ();
    return {matches, m_isRecursive};
}

} // namespace filesaver::services