//
//  MainWindowController.m
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 21/5/20.
//  Copyright © 2020 Pedro Tacla Yamada. All rights reserved.
//

#import "MainWindowController.h"

@interface MainWindowController ()

@end

@implementation MainWindowController

- (void)windowDidLoad
{
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

- (void)close
{
    [[NSApplication sharedApplication] terminate:self];
    [super close];
}

@end
