//
// Created by Pedro Tacla Yamada on 3/7/20.
//

#include <catch2/catch.hpp>
#include <fruit/fruit.h>
#include <memory>

#include "../../src/lfilesaver/factory/FileSaverFactory.h"
#include "../../src/lfilesaver/services/stats/InMemoryMetricsReporter.h"

TEST_CASE ("FileSaverFactory can build a filesaver instance")
{
    filesaver::FileSaverFactory factory;
    auto& filesaver = factory.getRef ();
    REQUIRE (!filesaver.getVersion ().empty ());
}

TEST_CASE ("FileSaverFactory will always build the same instance")
{
    filesaver::FileSaverFactory factory;
    auto* filesaver1 = factory.getPtr ();
    auto* filesaver2 = factory.getPtr ();
    REQUIRE (filesaver1 == filesaver2);
}

fruit::Component<filesaver::services::stats::MetricsReporter> getTestMetricsReporterComponent ()
{
    return fruit::createComponent ()
        .bind<filesaver::services::stats::MetricsReporter, filesaver::services::stats::InMemoryMetricsReporter> ();
};

TEST_CASE ("FileSaverFactory will always return the same stats instance")
{
    using namespace filesaver;
    using namespace filesaver::services::stats;

    // According to the documentation, all classes have 1 instance per injector.
    fruit::Injector<MetricsReporter> injector{getTestMetricsReporterComponent};

    auto* metricsReporter1 = injector.get<MetricsReporter*> ();
    auto* metricsReporter2 = injector.get<MetricsReporter*> ();

    REQUIRE (metricsReporter1 == metricsReporter2);
}
