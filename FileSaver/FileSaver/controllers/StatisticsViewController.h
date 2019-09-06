//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

@interface StatisticsViewController : NSViewController

@property (weak) IBOutlet NSTextField *workerCountField;
@property (weak) IBOutlet NSTextField *fileCountField;
@property (weak) IBOutlet NSProgressIndicator *progressBar;

@end
