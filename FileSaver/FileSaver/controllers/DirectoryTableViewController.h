//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <boost/filesystem/path.hpp>
#import <functional>

@interface DirectoryTableViewController : NSViewController <NSTableViewDelegate, NSTableViewDataSource> {
    @public boost::filesystem::path representedPath;
}

@property(weak) IBOutlet NSTableView *tableView;
@property NSArray <NSString *> *files;
@property id delegate;

@end

@protocol DirectoryTableViewControllerDelegate

- (void) onClickDirectory:(DirectoryTableViewController*)directory atFile:(NSString*)file;

@end

@interface NSObject (DirectoryTableViewControllerDelegate)

- (void) onClickDirectory:(DirectoryTableViewController*)directory atFile:(NSString*)file;

@end
