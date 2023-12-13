//
// Created by Pedro Tacla Yamada on 3/7/20.
//

#include "FileSaverFactory.h"

#include "../FileSaver.h"
#include "../server/util/DefaultStatsProvider.h"
#include "../services/category/LevelDbFileCategoryStore.h"
#include "../services/settings/SettingsService.h"
#include "../services/stats/InMemoryMetricsReporter.h"
#include <fruit/component.h>
#include <fruit/injector.h>

namespace filesaver
{

FileSaverFactory::FileSaverFactory ()
{
}

FileSaver& FileSaverFactory::getRef ()
{
    return injector.get<FileSaver &> ();
}

FileSaver* FileSaverFactory::getPtr ()
{
    return injector.get<FileSaver *> ();
}

fruit::Injector<FILESAVER_PUBLIC_CLASSES>& FileSaverFactory::getInjector ()
{
    return injector;
}

fruit::Component<FILESAVER_PUBLIC_CLASSES> FileSaverFactory::getFileSaverComponent ()
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
        .bind<StorageService, LevelDbStorageService> ();
}

} // namespace filesaver
