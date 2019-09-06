//
//  AppDelegate.m
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 30/08/19.
//  Copyright © 2019 Pedro Tacla Yamada. All rights reserved.
//

#import <AppKit/AppKit.h>
#include <string>

#import "AppDelegate.h"
#import "services/WorkerManagerService.h"

@interface AppDelegate ()
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    FileSaverService::start();
    FileSaverService::getInstance().scan(std::string{"/"});
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    FileSaverService::destroy();
}

@end
