//
// Created by Pedro Tacla Yamada on 2019-09-04.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#import "VisualizationsViewController.h"


@implementation VisualizationsViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    NSURLRequest *request = [NSURLRequest requestWithURL:[[NSURL alloc] initWithString:@"http://127.0.0.1:3000"]];
    [[self webview] setUIDelegate:self];
    [[self webview] setNavigationDelegate:self];
    auto* navigation = [[self webview] loadRequest:request];
    NSLog(@"%@", navigation);
}

- (void)webView:(WKWebView *)webView didFinishNavigation:(null_unspecified WKNavigation *)navigation {
    NSLog(@"Navigation finished");
}

- (void)webView:(WKWebView *)webView didFailNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error {
    NSLog(@"Navigation failed: %@", error);
}


@end