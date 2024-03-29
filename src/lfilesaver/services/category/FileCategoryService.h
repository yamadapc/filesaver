//
// Created by Pedro Tacla Yamada on 17/7/20.
//


#pragma once

#include <boost/filesystem.hpp>
#include <fruit/fruit.h>
#include <memory>
#include <string>
#include <unordered_set>

#include "PathMatcher.h"

namespace filesaver
{
namespace services
{

class FileCategoryService
{
public:
    virtual ~FileCategoryService () = default;
    virtual std::vector<std::shared_ptr<FileCategory>> getCategories () = 0;
};

class FileCategoryServiceImpl final : public FileCategoryService
{
public:
    INJECT (FileCategoryServiceImpl ());
    ~FileCategoryServiceImpl () override = default;

    std::vector<std::shared_ptr<FileCategory>> getCategories () override;

private:
    std::vector<std::shared_ptr<FileCategory>> m_categories;
};

} // namespace services
} // namespace filesaver
