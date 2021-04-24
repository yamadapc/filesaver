//
// Created by Pedro Tacla Yamada on 3/7/20.
//

#include "FileSaverFactory.h"

#include "../server/util/DefaultStatsProvider.h"

namespace filesaver
{

FileSaverFactory::FileSaverFactory () : injector (getFileSaverComponent)
{
}

FileSaver& FileSaverFactory::getRef ()
{
    return injector.get<filesaver::FileSaver&> ();
}

FileSaver* FileSaverFactory::getPtr ()
{
    return injector.get<filesaver::FileSaver*> ();
}

fruit::Component<FileSaver> FileSaverFactory::getFileSaverComponent ()
{
    return fruit::createComponent ()
        .bind<services::FileCategoryService, services::FileCategoryServiceImpl> ()
        .bind<services::RootPathProvider, services::DefaultRootPathProvider> ()
        .bind<server::StatsProvider, server::DefaultStatsProvider> ()
        .registerConstructor<fruit::Annotated<services::stats::ScanTimer, SimpleTimer> ()> ()
        .registerConstructor<data::WorkQueue<FileSizePair> ()> ()
        .registerProvider ([] () -> StorageService* { return new LevelDbStorageService ("default.db"); });
}

} // namespace filesaver
