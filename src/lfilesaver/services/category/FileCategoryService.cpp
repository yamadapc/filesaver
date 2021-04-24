//
// Created by Pedro Tacla Yamada on 17/7/20.
//

#include "FileCategoryService.h"

namespace filesaver::services
{

FileCategoryServiceImpl::FileCategoryServiceImpl ()
{
    m_categories.push_back (std::make_shared<NodeModulesFileCategory> ());
}

std::vector<std::shared_ptr<FileCategory>> FileCategoryServiceImpl::getCategories ()
{
    return m_categories;
}

} // namespace filesaver::services