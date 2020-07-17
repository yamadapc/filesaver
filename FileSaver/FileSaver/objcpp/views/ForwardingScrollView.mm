//
// Created by Pedro Tacla Yamada on 2019-09-01.
// Copyright (c) 2019 Pedro Tacla Yamada. All rights reserved.
//

#import "ForwardingScrollView.h"


@implementation ForwardingScrollView
{
}

- (void)scrollWheel:(NSEvent*)theEvent
{
    if (fabs ([theEvent scrollingDeltaX]) > fabs ([theEvent scrollingDeltaY]))
    {
        [self.nextResponder scrollWheel:theEvent];
    }
    else
    {
        [super scrollWheel:theEvent];
    }
}

@end
