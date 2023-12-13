//
// Created by Pedro Tacla Yamada on 3/7/20.
//

#pragma once

#include <fruit/fruit.h>
#include <memory>

#include "../FileSaver.h"
#include "../services/paths/RootPathProvider.h"

#define FILESAVER_PUBLIC_CLASSES \
  FileSaver, \
  services::FileCategoryService, \
  services::LevelDbFileCategoryStore

namespace filesaver
{

class FileSaverFactory
{
public:
    FileSaverFactory ();

    FileSaver& getRef ();
    FileSaver* getPtr ();

    fruit::Injector<FILESAVER_PUBLIC_CLASSES>& getInjector ();

    static fruit::Component<FILESAVER_PUBLIC_CLASSES> getFileSaverComponent ();

private:
    fruit::Injector<FILESAVER_PUBLIC_CLASSES> injector{getFileSaverComponent};
};

} // namespace filesaver
