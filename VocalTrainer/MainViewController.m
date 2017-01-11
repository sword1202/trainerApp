//
// Created by Semyon Tikhonenko on 1/4/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#import "MainViewController.h"
#import "PitchGraphView.h"
#import "NSString+StringUtils.h"
#import "NSAlert+AlertUtils.h"

@implementation MainViewController {

    __weak IBOutlet PitchGraphView *pitchGraphView;
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
        if (![url.pathExtension isEqualToStringIgnoreCase:@"wav"]) {
            [NSAlert error:@"Only .wav files are supported"];
        } else {
            [pitchGraphView onWavFileSelected:url.path];
        }
    }
}

@end
