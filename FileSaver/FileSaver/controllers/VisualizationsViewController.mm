//
// Created by Pedro Tacla Yamada on 2019-09-04.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#import "VisualizationsViewController.h"
#import "../services/WorkerManagerService.h"

@implementation VisualizationsViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    NSURLRequest *request = [NSURLRequest requestWithURL:[[NSURL alloc] initWithString:@"http://127.0.0.1:3000"]];
    [[self webview] setUIDelegate:self];
    [[self webview] setNavigationDelegate:self];

    auto *navigation = [[self webview] loadRequest:request];
    NSLog(@"%@", navigation);

    [NSTimer scheduledTimerWithTimeInterval:0.5
                                     target:self
                                   selector:@selector(timerFired)
                                   userInfo:nil
                                    repeats:YES];
}

- (void)timerFired {
    auto &instance = FileSaverService::getInstance();
    auto elapsed = instance.getElapsed();
    auto totalFiles = instance.getTotalFiles();
    double filesPerSecond = 1000 * (double)totalFiles/elapsed;

    [[self webview] evaluateJavaScript:[NSString stringWithFormat:@"window.filesaver.emit('filesPerSecond', %f)", filesPerSecond]
                     completionHandler:^(id webview, NSError *error) {
                         NSLog(@"%@", error);
                     }];
}

- (void)webView:(WKWebView *)webView didFinishNavigation:(null_unspecified WKNavigation *)navigation {
    NSLog(@"Navigation finished");
}

- (void)webView:(WKWebView *)webView didFailNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error {
    NSLog(@"Navigation failed: %@", error);
}


@end
