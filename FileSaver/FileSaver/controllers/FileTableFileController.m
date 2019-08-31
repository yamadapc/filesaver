//
//  FileTableFileController.m
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 30/08/19.
//  Copyright © 2019 Pedro Tacla Yamada. All rights reserved.
//

#import "FileTableFileController.h"
#import "../views/FileTableCell.h"

@implementation FileTableFileController

NSArray <NSString*>* files = nil;

- (void)viewDidLoad {
    [super viewDidLoad];

    [self setupTableView];
}


- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
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
    files = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:@"/" error:&error];

    if (error != nil) {
        NSLog(@"Failed to list contents:");
        NSLog(@"%@", [error debugDescription]);
        return 0;
    }

    return [files count];
}

- (nullable id)tableView:(NSTableView *)tableView objectValueForTableColumn:(nullable NSTableColumn *)tableColumn row:(NSInteger)row {
    return files[(NSUInteger) row];
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(nullable NSTableColumn *)tableColumn row:(NSInteger)row {
    NSLog(@"Getting table cell");
    FileTableCell* fileTableCell = [tableView makeViewWithIdentifier:@"FileTableCell" owner:self];
    NSLog(@"%@", files[(NSUInteger) row]);
    [[fileTableCell textField] setStringValue:files[(NSUInteger) row]];
    NSLog(@"%@", [[fileTableCell textField] stringValue]);
    return fileTableCell;
}

@end
