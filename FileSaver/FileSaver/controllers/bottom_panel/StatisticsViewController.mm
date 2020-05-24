//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#import "StatisticsViewController.h"
#include "../../services/WorkerManagerService.h"

// Will pool FileSaver to get statistics and update the views that show them.
@implementation StatisticsViewController

- (void)viewDidLoad
{
    [self updateData];

    [NSTimer scheduledTimerWithTimeInterval:0.5
                                     target:self
                                   selector:@selector (updateData)
                                   userInfo:nil
                                    repeats:YES];
}

- (void)updateData
{
    auto& fileSaver = FileSaverService::getInstance ();

    auto numWorkers = fileSaver.getNumWorkers ();
    auto totalFiles = fileSaver.getTotalFiles ();
    auto totalKnownFiles = fileSaver.getTotalKnownFiles ();
    double filesPerSecond = fileSaver.getFilesPerSecond ();
    double progress = (double)totalFiles / totalKnownFiles;

    [[self workerCountField] setStringValue:[NSString stringWithFormat:@"%lu workers", numWorkers]];
    [[self fileCountField] setStringValue:[NSString stringWithFormat:@"%lu files", totalFiles]];
    if (!fileSaver.areAllTargetsFinished ())
    {
        [[self filesPerSecondField] setStringValue:[NSString stringWithFormat:@"%.0f files/s", filesPerSecond]];
    }
    [[self progressBar] setDoubleValue:100.0 * progress];
}

@end
