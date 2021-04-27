//
// Created by Pedro Tacla Yamada on 17/7/20.
//

#include "FileCategoryService.h"

namespace filesaver::services
{

FileCategoryServiceImpl::FileCategoryServiceImpl ()
{
    m_categories.push_back (
        std::make_shared<ComponentsCategory> ("Node.js modules",
                                              "Dependency and build files for Node.js",
                                              "node_modules",
                                              std::unordered_set<std::string>{"node_modules"},
                                              true));
    m_categories.push_back (
        std::make_shared<ComponentsCategory> ("Build directories",
                                              "Build directories",
                                              "build",
                                              std::unordered_set<std::string>{"cmake-build-debug",
                                                                              "cmake-build-release",
                                                                              "cmake-build-nodeaddon",
                                                                              ".gradle",
                                                                              ".conan",
                                                                              "build",
                                                                              "dist",
                                                                              "Build"},
                                              true));
    m_categories.push_back (std::make_shared<ExtensionCategory> (
        "Source code files",
        "Source code files",
        "source_code",
        std::unordered_set<std::string>{".swift", ".cpp", ".c", ".h", ".js", ".hs", ".java", ".kt", ".ts", ".py"},
        false));
    m_categories.push_back (
        std::make_shared<ExtensionCategory> ("Documents",
                                             "PDF files, document files & text files",
                                             "documents",
                                             std::unordered_set<std::string>{".md", ".pdf", ".doc", ".txt"},
                                             false));
    m_categories.push_back (std::make_shared<ExtensionCategory> (
        "Images", "Image files", "images", std::unordered_set<std::string>{".png", ".jpeg", ".jpg"}, false));
    m_categories.push_back (
        std::make_shared<ExtensionCategory> ("Audio files",
                                             "Audio files",
                                             "audio",
                                             std::unordered_set<std::string>{".mp3", ".flac", ".aiff", ".wav", ".ogg"},
                                             false));
    m_categories.push_back (std::make_shared<ExtensionCategory> (
        "Video files", "Video files", "video", std::unordered_set<std::string>{".mp4", ".mov", ".mkv"}, false));
    m_categories.push_back (std::make_shared<ExtensionCategory> (
        "Applications", "Applications", "applications", std::unordered_set<std::string>{".app"}, false));
    m_categories.push_back (std::make_shared<ComponentsCategory> (
        "Containers", "Docker VM disk image", "containers", std::unordered_set<std::string>{"Docker.raw"}, false));
}

std::vector<std::shared_ptr<FileCategory>> FileCategoryServiceImpl::getCategories ()
{
    return m_categories;
}

} // namespace filesaver::services