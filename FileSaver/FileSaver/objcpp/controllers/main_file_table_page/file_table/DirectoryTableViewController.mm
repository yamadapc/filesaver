//
// Created by Pedro Tacla Yamada on 2019-08-31.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#import "DirectoryTableViewController.h"
#import "FileTableCell.h"
#import <boost/filesystem/path.hpp>
#import <string>

#include "../../../services/WorkerManagerService.h"
#include "FileSaver.h"

@implementation DirectoryTableViewController {
    NSTimer* timer;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    [self setFiles:[[NSArray alloc] init]];
    [self setupTableView];
    [self reloadDataWithSelection];
    [[self tableView] setAction:@selector (onTableRowClicked)];
    [[self tableView] setDoubleAction:@selector (onTableRowDoubleClicked)];

    timer = [NSTimer scheduledTimerWithTimeInterval:1
                                             target:self
                                           selector:@selector (reloadDataWithSelection)
                                           userInfo:nil
                                            repeats:YES];
}

- (void)removeFromParentViewController
{
    [timer invalidate];
}

- (void)reloadDataWithSelection
{
    NSError* error;
    auto* files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:[self representedObject] error:&error];

    if (error != nil)
    {
        NSLog (@"DirectoryTableViewController Failed to list contents:");
        NSLog (@"DirectoryTableViewController %@", [error debugDescription]);
        return;
    }

    auto& fileSaver = FileSaverService::getInstance ();
    files = [files sortedArrayUsingComparator:^(id file1, id file2) {
      boost::filesystem::path path1 = representedPath;
      path1.append ([file1 UTF8String]);
      auto size1 = fileSaver.getCurrentSizeAt (path1.string ());
      boost::filesystem::path path2 = representedPath;
      path2.append ([file2 UTF8String]);
      auto size2 = fileSaver.getCurrentSizeAt (path2.string ());

      if (size1 > size2)
      {
          return (NSComparisonResult)NSOrderedAscending;
      }

      if (size1 == size2)
      {
          return (NSComparisonResult)NSOrderedSame;
      }

      return (NSComparisonResult)NSOrderedDescending;
    }];
    [self setFiles:files];

    auto selectedRow = [[self tableView] selectedRow];

    if (selectedRow == -1)
    {
        [[self tableView] reloadData];
        return;
    }

    // TODO - This is a pretty bad algorithm, but should be alright.
    auto file = [[self files] objectAtIndex:selectedRow];

    [[self tableView] reloadData];

    selectedRow = [[self files] indexOfObject:file];

    [[self tableView] selectRowIndexes:[NSIndexSet indexSetWithIndex:selectedRow] byExtendingSelection:NO];
}

- (void)setRepresentedObject:(id)representedObject
{
    [super setRepresentedObject:representedObject];
    [[self tableView] reloadData];
}

- (void)onTableRowClicked
{
    auto clickedRow = static_cast<NSUInteger> ([[self tableView] clickedRow]);
    if (clickedRow >= [[self files] count])
    {
        return;
    }
    auto* file = [self files][clickedRow];
    [[self delegate] onClickDirectory:self atFile:file];
}

- (void)onTableRowDoubleClicked
{
    [self openFile:self];
}

- (void)setupTableView
{
    NSTableView* tableView = [self tableView];

    [tableView setDelegate:self];
    [tableView setDataSource:self];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView*)tableView
{
    return [[self files] count];
}

- (nullable id)tableView:(NSTableView*)tableView
    objectValueForTableColumn:(nullable NSTableColumn*)tableColumn
                          row:(NSInteger)row
{
    auto* files = [self files];
    if (row >= [files count])
    {
        return nil;
    }
    return files[(NSUInteger)row];
}

- (NSView*)tableView:(NSTableView*)tableView viewForTableColumn:(nullable NSTableColumn*)tableColumn row:(NSInteger)row
{
    auto* files = [self files];
    FileTableCell* fileTableCell = [tableView makeViewWithIdentifier:@"FileTableCell" owner:self];

    if (row >= [files count])
    {
        return nil;
    }

    NSString* file = files[(NSUInteger)row];
    [[fileTableCell textField] setStringValue:file];

    boost::filesystem::path filepath = representedPath;
    filepath.append ([file UTF8String]);
    std::string filepathStr = filepath.string ();
    auto& fileSaver = FileSaverService::getInstance ();
    auto size = fileSaver.getCurrentSizeAt (filepathStr);

    if (size > 0)
    {
        auto* bytes = filesaver::prettyPrintBytes (size).c_str ();
        [[fileTableCell sizeTextField] setStringValue:[NSString stringWithUTF8String:bytes]];
    }
    else if (size < 0)
    {
        NSLog (@"DirectoryTableViewController Size is less than 0 for: %@", [self representedObject]);
        return fileTableCell;
    }
    else
    {
        [[fileTableCell sizeTextField] setStringValue:@"..."];
    }

    return fileTableCell;
}

- (IBAction)openFile:(id)sender
{
    auto clickedRow = [self tableView].clickedRow;
    auto* base = [NSURL URLWithString:[self representedObject]];
    auto* file = [[self files] objectAtIndex:clickedRow];
    auto* url = [base URLByAppendingPathComponent:file];
    NSLog (@"Opening finder at %@", [url absoluteString]);
    auto* array = [[NSArray alloc] initWithObjects:[url absoluteString], nil];
    [[NSWorkspace sharedWorkspace] activateFileViewerSelectingURLs:array];
}

@end
