//
// Created by Pedro Tacla Yamada on 3/7/20.
//

#pragma once

#include <fruit/fruit.h>
#include <memory>

#include "../FileSaver.h"

namespace filesaver
{

class FileSaverFactory
{
public:
    FileSaverFactory ();

    FileSaver& getRef ();

    FileSaver* getPtr ();

    static fruit::Component<FileSaver> getFileSaverComponent ();

private:
    fruit::Injector<FileSaver> injector;
};

} // namespace filesaver
