//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#import <string>
#import <boost/filesystem/path.hpp>
#import "DirectoryTableViewController.h"
#import "FileTableCell.h"

#include "../services/WorkerManagerService.h"
#include "FileSaver.h"

@implementation DirectoryTableViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self setFiles:[[NSArray alloc] init]];
    [self setupTableView];
    [[self tableView] setAction:@selector(onTableRowClicked)];

    [NSTimer scheduledTimerWithTimeInterval:1
                                     target:[self tableView]
                                   selector:@selector(reloadData)
                                   userInfo:nil
                                    repeats:YES];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
    [[self tableView] reloadData];
}

- (void)onTableRowClicked {
    auto clickedRow = static_cast<NSUInteger>([[self tableView] clickedRow]);
    if (clickedRow >= [[self files] count]) {
        return;
    }
    auto *file = [self files][clickedRow];
    [[self delegate] onClickDirectory:self atFile:file];
}

- (void)setupTableView {
    NSLog(@"FileTableFileController::setupTableView");
    NSTableView *tableView = [self tableView];

    [tableView setDelegate:self];
    [tableView setDataSource:self];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
    NSLog(@"FileTableFileController::numberOfRowsInTableView");

    NSError *error;
    [self setFiles:[[NSFileManager defaultManager] contentsOfDirectoryAtPath:[self representedObject] error:&error]];
    auto* files = [self files];
    if (error != nil) {
        NSLog(@"Failed to list contents:");
        NSLog(@"%@", [error debugDescription]);
        return 0;
    }

    return [files count];
}

- (nullable id)tableView:(NSTableView *)tableView objectValueForTableColumn:(nullable NSTableColumn *)tableColumn row:(NSInteger)row {
    auto* files = [self files];
    if (row >= [files count]) {
        return nil;
    }
    return files[(NSUInteger) row];
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(nullable NSTableColumn *)tableColumn row:(NSInteger)row {
    auto* files = [self files];
    FileTableCell *fileTableCell = [tableView makeViewWithIdentifier:@"FileTableCell" owner:self];

    if (row >= [files count]) {
        return nil;
    }

    NSString *file = files[(NSUInteger) row];
    [[fileTableCell textField] setStringValue:file];

    boost::filesystem::path filepath = representedPath;
    filepath.append([file UTF8String]);
    std::string filepathStr = filepath.string();
    auto &fileSaver = FileSaverService::getInstance();
    auto size = fileSaver.getCurrentSizeAt(filepathStr);

    if (size > 0) {
        auto *bytes = prettyPrintBytes(size).c_str();
        [[fileTableCell sizeTextField] setStringValue:[NSString stringWithUTF8String:bytes]];
    } else {
        [[fileTableCell sizeTextField] setStringValue:@"..."];
    }

    return fileTableCell;
}

@end
