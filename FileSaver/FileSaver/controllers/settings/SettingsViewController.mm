//
//  SettingsViewController.m
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 21/5/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

#import "SettingsViewController.h"

@interface SettingsViewController ()

@end

@implementation SettingsViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    auto& fileSaver = FileSaverService::getInstance ();
    auto numCpus = std::thread::hardware_concurrency ();
    auto numWorkers = fileSaver.getNumWorkers ();

    [[self numWorkersSlider] setNumberOfTickMarks:numCpus*2];
    [[self numWorkersSlider] setAllowsTickMarkValuesOnly:true];
    [[self numWorkersSlider] setIntValue:static_cast<int> (numWorkers)];

    [self updateValueLabel];
}

- (IBAction)sliderMoved:(id)sender
{
    [self updateValueLabel];

    auto& fileSaver = FileSaverService::getInstance ();
    fileSaver.setNumWorkers (static_cast<unsigned int> ([[self numWorkersSlider] intValue]));
    fileSaver.stop ();
    fileSaver.start ();

    using filesaver::services::settings::SettingsService;
    auto settingsService = SettingsService::defaultForMac ();
    settingsService.loadSettings ();
    settingsService.set ("numWorkers", fileSaver.getNumWorkers ());
    settingsService.saveSettings ();
}

- (void) updateValueLabel
{
    auto value = [[self numWorkersSlider] intValue];
    [[self numWorkersLabel] setStringValue:[NSString stringWithFormat:@"%d worker threads", value]];
}

@end
