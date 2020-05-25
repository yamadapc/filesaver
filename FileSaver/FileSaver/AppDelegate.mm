//
//  AppDelegate.m
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 30/08/19.
//  Copyright © 2019 Pedro Tacla Yamada. All rights reserved.
//

#import <AppKit/AppKit.h>
#include <string>
#include "services/settings/SettingsService.h"

#import "AppDelegate.h"
#import "services/WorkerManagerService.h"

@interface AppDelegate ()
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification
{
    using filesaver::services::settings::SettingsService;

    auto& fileSaver = FileSaverService::getInstance ();
    auto settingsService = SettingsService::defaultForMac ();
    settingsService.loadSettings();

    auto maybeNumWorkers = settingsService.get<unsigned int> ("numWorkers");
    if (maybeNumWorkers.has_value ())
    {
        fileSaver.setNumWorkers (maybeNumWorkers.value ());
    }

    fileSaver.start ();
    fileSaver.scan (std::string{"/"});
}

- (void)applicationWillTerminate:(NSNotification*)aNotification
{
    FileSaverService::destroy ();
}

@end
