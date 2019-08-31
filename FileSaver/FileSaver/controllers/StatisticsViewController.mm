//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#import "StatisticsViewController.h"
#include "../services/WorkerManagerService.h"

@implementation StatisticsViewController

-(void) viewDidLoad {
    [self updateData];

    [NSTimer scheduledTimerWithTimeInterval:0.5
                                     target:self
                                   selector:@selector(updateData)
                                   userInfo:nil
                                    repeats:YES];
}

-(void) updateData {
    auto& fileSaver = FileSaverService::getInstance();

    auto numWorkers = fileSaver.getNumWorkers();
    auto totalFiles = fileSaver.getTotalFiles();

    [[self workerCountField] setStringValue:[NSString stringWithFormat:@"%d workers", numWorkers]];
    [[self fileCountField] setStringValue:[NSString stringWithFormat:@"%lu files", totalFiles]];
}

@end
