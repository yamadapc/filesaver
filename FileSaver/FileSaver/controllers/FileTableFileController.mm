//
//  FileTableFileController.m
//  FileSaver
//
//  Created by Pedro Tacla Yamada on 30/08/19.
//  Copyright © 2019 Pedro Tacla Yamada. All rights reserved.
//

#include <vector>
#include <string>
#include <boost/filesystem/path.hpp>

#import "FileTableFileController.h"
#import "DirectoryTableViewController.h"

#include "../services/WorkerManagerService.h"
#import "../views/FileTableCell.h"

@implementation FileTableFileController

- (void)viewDidLoad {
    [super viewDidLoad];

    auto* documentView = [self scrollView].documentView;
    [documentView setTranslatesAutoresizingMaskIntoConstraints:NO];
    auto frame = [self view].frame;
    [[self scrollView] addConstraint:[[documentView heightAnchor] constraintEqualToAnchor:[[self scrollView] heightAnchor]]];
    [documentView setFrame:CGRectMake(frame.origin.x, frame.origin.y, 200.0, frame.size.height)];

    documentWidthConstraint = [[documentView widthAnchor] constraintEqualToConstant:200.0];
    [documentView addConstraint:documentWidthConstraint];

    [self setupRootController];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];
}

- (void)setupRootController {
    DirectoryTableViewController *rootVC = [[NSStoryboard storyboardWithName:@"Main" bundle:nil] instantiateControllerWithIdentifier:@"DirectoryTableViewController"];
    [rootVC setRepresentedObject:@"/"];
    currentPath = "/";
    [self addChildViewController:rootVC];
    childDirectoryControllers.push_back(rootVC);

    rootVC->representedPath = currentPath;

    [rootVC setDelegate:self];
    auto* addedView = [rootVC view];
    auto* documentView = [self scrollView].documentView;
    [documentView addSubview:addedView];
    [addedView setTranslatesAutoresizingMaskIntoConstraints:NO];
    [documentView addConstraint:[[addedView leadingAnchor] constraintEqualToAnchor:[documentView leadingAnchor]]];
    [documentView addConstraint:[[addedView topAnchor] constraintEqualToAnchor:[documentView topAnchor]]];
    [documentView addConstraint:[[addedView bottomAnchor] constraintEqualToAnchor:[documentView bottomAnchor]]];
    [documentView addConstraint:[[addedView widthAnchor] constraintEqualToConstant:200.0]];
    [documentView addConstraint:[[addedView heightAnchor] constraintEqualToAnchor:[documentView heightAnchor]]];
    //[documentView setFrame:addedView.frame];
}

- (void)onClickDirectory:(DirectoryTableViewController *)directory atFile:(NSString *)file {
    if (directory == childDirectoryControllers[0]) {
        NSLog(@"Clicked %@", file);
        currentPath = "/";
        currentPath.append([file UTF8String]);

        [[[self view] window] setTitle:[NSString stringWithFormat:@"FileSaver - %s", currentPath.string().c_str()]];

        for (int i = 1; i < childDirectoryControllers.size(); i++) {
            [[childDirectoryControllers[i] view] removeFromSuperview];
            [childDirectoryControllers[i] removeFromParentViewController];
        }
        childDirectoryControllers.erase(childDirectoryControllers.begin() + 1, childDirectoryControllers.end());
        assert(childDirectoryControllers.size() == 1);

        [self addChildTable];
    } else {
        unsigned long index = 0;
        for (auto* child: childDirectoryControllers) {
            if (child == directory) {
                break;
            }
            index += 1;
        }

        boost::filesystem::path path = {[[directory representedObject] UTF8String]};
        path.append([file UTF8String]);
        currentPath = path;

        [[[self view] window] setTitle:[NSString stringWithFormat:@"FileSaver - %s", currentPath.string().c_str()]];

        for (unsigned long i = index + 1; i < childDirectoryControllers.size(); i++) {
            [[childDirectoryControllers[i] view] removeFromSuperview];
            [childDirectoryControllers[i] removeFromParentViewController];
        }
        childDirectoryControllers.erase(childDirectoryControllers.begin() + index + 1, childDirectoryControllers.end());

        [self addChildTable];
    }

    auto numControllers = childDirectoryControllers.size();
    auto* documentView = [self scrollView].documentView;
    auto frame = documentView.frame;
    documentWidthConstraint.constant = 200.0 * numControllers;
    [documentView setFrame:CGRectMake(frame.origin.x, frame.origin.y, 200.0 * numControllers, frame.size.height)];
}

- (void)addChildTable {
    DirectoryTableViewController *vc = [[NSStoryboard storyboardWithName:@"Main" bundle:nil] instantiateControllerWithIdentifier:@"DirectoryTableViewController"];
    NSString *filepath = [NSString stringWithUTF8String:currentPath.string().c_str()];

    auto index = childDirectoryControllers.size();
    childDirectoryControllers.push_back(vc);

    vc->representedPath = currentPath;
    [vc setRepresentedObject:filepath];
    [self addChildViewController:vc];
    [vc setDelegate:self];

    auto* addedView = [vc view];
    auto* documentView = [self scrollView].documentView;

    [addedView setTranslatesAutoresizingMaskIntoConstraints:NO];
    [documentView addSubview:addedView];
    [documentView addConstraint:[[addedView leadingAnchor] constraintEqualToAnchor:[documentView leadingAnchor] constant:200.0 * index]];
    [documentView addConstraint:[[addedView widthAnchor] constraintEqualToConstant:200.0]];
    [documentView addConstraint:[[addedView topAnchor] constraintEqualToAnchor:[documentView topAnchor]]];
    [documentView addConstraint:[[addedView bottomAnchor] constraintEqualToAnchor:[documentView bottomAnchor]]];
    [documentView addConstraint:[[addedView heightAnchor] constraintEqualToAnchor:[documentView heightAnchor]]];
    NSLog(@"Added view at index %lu", index);
}

@end
