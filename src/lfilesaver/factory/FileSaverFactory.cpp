//
// Created by Pedro Tacla Yamada on 3/7/20.
//

#include "FileSaverFactory.h"

#include "../server/util/DefaultStatsProvider.h"
#include "../services/category/LevelDbFileCategoryStore.h"
#include "../services/settings/SettingsService.h"
#include "../services/stats/InMemoryMetricsReporter.h"

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

fruit::Injector<FileSaver>& FileSaverFactory::getInjector ()
{
    return injector;
}

fruit::Component<FileSaver> FileSaverFactory::getFileSaverComponent ()
{
    return fruit::createComponent ()
        .bind<services::FileCategoryService, services::FileCategoryServiceImpl> ()
        .bind<services::FileCategoryStore, services::LevelDbFileCategoryStore> ()
        .bind<services::RootPathProvider, services::DefaultRootPathProvider> ()
        .bind<services::stats::MetricsReporter, services::stats::InMemoryMetricsReporter> ()
        .bind<server::StatsProvider, server::DefaultStatsProvider> ()
        .registerConstructor<fruit::Annotated<services::stats::ScanTimer, SimpleTimer> ()> ()
        .registerConstructor<data::WorkQueue<FileSizePair> ()> ()
        // TODO - Support for other Operating Systems here
        .registerProvider ([] () -> services::settings::SettingsService
                           { return services::settings::SettingsService::defaultForMac (); })
        .registerProvider ([] () -> StorageService* { return new LevelDbStorageService ("default.db"); });
}

} // namespace filesaver
