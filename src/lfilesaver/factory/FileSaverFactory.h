//
// Created by Pedro Tacla Yamada on 3/7/20.
//

#pragma once

#include <fruit/fruit.h>
#include <memory>

#include "../FileSaver.h"
#include "../services/paths/RootPathProvider.h"

namespace filesaver
{

class FileSaverFactory
{
public:
    FileSaverFactory ();

    FileSaver& getRef ();
    FileSaver* getPtr ();

    fruit::Injector<FileSaver>& getInjector ();

    static fruit::Component<FileSaver> getFileSaverComponent ();

private:
    fruit::Injector<FileSaver> injector;
};

} // namespace filesaver
