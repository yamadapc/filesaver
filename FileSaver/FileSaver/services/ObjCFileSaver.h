//
//  ObjCFileSaver.h
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 27/6/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ObjCFileSaver : NSObject

+ (void)start;
+ (void)stop;
+ (void)scan:(NSString*)path;

+ (off_t)getCurrentSizeAt:(NSString*)path;
+ (bool)isPathFinished:(NSString*)path;
+ (bool)areAllTargetsFinished;
+ (void)setupDefaultStorage;
+ (void)setNumWorkers:(unsigned int)workers;

+ (unsigned long)getTotalFiles;
+ (unsigned long)getTotalKnownFiles;
+ (double)getFilesPerSecond;
+ (unsigned long)getNumWorkers;
+ (long long int)getElapsed;

+ (bool)loadSettings;
+ (bool)saveSettings;
+ (unsigned long)getSettingInt:(NSString*)name;
+ (void)setSettingInt:(NSString*)name withValue:(unsigned long)value;

+ (NSString*)getVersion;

@end

NS_ASSUME_NONNULL_END
