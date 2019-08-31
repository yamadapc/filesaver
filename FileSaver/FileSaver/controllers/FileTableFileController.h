//
//  FileTableFileController.h
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 30/08/19.
//  Copyright © 2019 Pedro Tacla Yamada. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "DirectoryTableViewController.h"

@interface FileTableFileController : NSViewController <NSTableViewDelegate, NSTableViewDataSource, DirectoryTableViewControllerDelegate> {
    std::vector<DirectoryTableViewController*> childDirectoryControllers;
    boost::filesystem::path currentPath;
    NSLayoutConstraint* documentWidthConstraint;
}

@property (weak) IBOutlet NSScrollView *scrollView;

@end
