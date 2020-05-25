//
//  SettingsViewController.h
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 21/5/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include <thread>

#include "services/settings/SettingsService.h"
#include "../../services/WorkerManagerService.h"

NS_ASSUME_NONNULL_BEGIN

@interface SettingsViewController : NSViewController

@property (weak) IBOutlet NSSlider *numWorkersSlider;
@property (weak) IBOutlet NSTextField *numWorkersLabel;

- (IBAction)sliderMoved:(id)sender;

@end

NS_ASSUME_NONNULL_END
