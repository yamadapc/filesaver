//
// Created by Pedro Tacla Yamada on 17/7/20.
//


#pragma once

#include "PathMatcher.h"
#include <boost/filesystem.hpp>
#include <string>
#include <unordered_set>

namespace filesaver::services
{

class FileCategoryService
{
public:
    virtual ~FileCategoryService() = default;
    virtual std::vector<FileCategory> getCategories() = 0;
};

class FileCategoryServiceImpl : public FileCategoryService
{
public:
    std::vector<FileCategory> getCategories () override;

private:
};

} // namespace filesaver::services
