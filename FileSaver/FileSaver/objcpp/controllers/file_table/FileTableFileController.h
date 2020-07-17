//
//  FileTableFileController.h
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 30/08/19.
//  Copyright © 2019 Pedro Tacla Yamada. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface FileTableFileController
    : NSViewController <NSTableViewDelegate, NSTableViewDataSource>
{
}

@property (weak) IBOutlet NSScrollView* scrollView;

@end
