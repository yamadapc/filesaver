//
//  AppDelegate.m
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 30/08/19.
//  Copyright © 2019 Pedro Tacla Yamada. All rights reserved.
//

#include <thread>

#import "AppDelegate.h"
#include "workers/WorkerManager.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

WorkerManager workerManager;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    auto numCpus = std::thread::hardware_concurrency();
    workerManager.start(numCpus);
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    workerManager.stop();
}

@end
