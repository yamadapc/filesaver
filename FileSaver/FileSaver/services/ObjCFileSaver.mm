//
//  ObjCFileSaver.m
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 27/6/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

#import "ObjCFileSaver.h"

#include "services/settings/SettingsService.h"
#include "WorkerManagerService.h"

@implementation ObjCFileSaver

+ (void)start
{
    FileSaverService::getInstance().start();
}

+ (void)stop
{
    FileSaverService::getInstance().stop();
}

+ (void)scan:(NSString*)path
{
    std::string pathStr = std::string([path UTF8String]);
    FileSaverService::getInstance().scan(pathStr);
}

+ (off_t)getCurrentSizeAt:(NSString*)path
{
    std::string pathStr = std::string([path UTF8String]);
    return FileSaverService::getInstance().getCurrentSizeAt(pathStr);
}

+ (bool)isPathFinished:(NSString*)path
{
    std::string pathStr = std::string([path UTF8String]);
    boost::filesystem::path pathPth{pathStr};
    return FileSaverService::getInstance().isPathFinished(pathPth);
}

+ (bool)areAllTargetsFinished
{
    return FileSaverService::getInstance().areAllTargetsFinished();
}

+ (void)setNumWorkers:(unsigned int)workers
{
    FileSaverService::getInstance().setNumWorkers(workers);
}

+ (unsigned long)getTotalFiles
{
    return FileSaverService::getInstance().getTotalFiles();
}

+ (unsigned long)getTotalKnownFiles
{
    return FileSaverService::getInstance().getTotalKnownFiles();
}

+ (double)getFilesPerSecond
{
    return FileSaverService::getInstance().getFilesPerSecond();
}

+ (unsigned long)getNumWorkers
{
    return FileSaverService::getInstance().getNumWorkers();
}

+ (long long int)getElapsed
{
    return FileSaverService::getInstance().getElapsed();
}

+ (NSString*)getVersion
{
    auto version = FileSaverService::getInstance().getVersion();
    NSString* result = [[NSString alloc] initWithCString:version.c_str() encoding:NSUTF8StringEncoding];
    return result;
}

+ (bool)loadSettings
{
    using filesaver::services::settings::SettingsService;
    auto settingsService = SettingsService::defaultForMac ();
    return settingsService.loadSettings();
}

+ (bool)saveSettings
{
    using filesaver::services::settings::SettingsService;
    auto settingsService = SettingsService::defaultForMac ();
    return settingsService.saveSettings();
}

+ (unsigned long)getSettingInt:(NSString*)name 
{
    using filesaver::services::settings::SettingsService;
    auto settingsService = SettingsService::defaultForMac ();
    std::string nameStr = std::string([name UTF8String]);
    return settingsService.get<unsigned long>(nameStr).value_or (0L);
}

+ (void)setSettingInt:(NSString*)name withValue:(unsigned long)value
{
    using filesaver::services::settings::SettingsService;
    auto settingsService = SettingsService::defaultForMac ();
    std::string nameStr = std::string([name UTF8String]);
    settingsService.set<unsigned long>(nameStr, value);
}

@end
