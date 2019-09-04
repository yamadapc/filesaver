//
// Created by Pedro Tacla Yamada on 2019-09-04.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <WebKit/WebKit.h>

@interface VisualizationsViewController : NSViewController<WKNavigationDelegate, WKUIDelegate>

@property(weak) IBOutlet WKWebView *webview;

@end
