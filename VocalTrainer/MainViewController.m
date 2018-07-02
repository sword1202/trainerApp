//
// Created by Semyon Tikhonenko on 1/4/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#import "MainViewController.h"
#import "NSString+StringUtils.h"
#import "NSAlert+AlertUtils.h"

@implementation MainViewController {
    
}

- (void)viewDidLoad {
    [super viewDidLoad];
}

-(IBAction) openDocument:(id)sender {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    NSArray* fileTypes = @[@"wav", @"WAV"];
    [panel setFloatingPanel:YES];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setAllowsMultipleSelection:YES];
    [panel setAllowedFileTypes:fileTypes];
    NSInteger i = [panel runModal];
    if(i == NSModalResponseOK){
        NSURL *url = panel.URL;
        NSLog(@"file opened = %@", url);
    }
}

@end
