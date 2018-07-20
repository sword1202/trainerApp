//
// Created by Semyon Tikhonenko on 1/4/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#import "MainViewController.h"
#import "MainController.h"
#import "MvxPlayer.h"

@implementation MainViewController {
    
}

- (void)viewDidLoad {
    [super viewDidLoad];
}

-(IBAction) openDocument:(id)sender {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    NSArray* fileTypes = @[@"mvx"];
    [panel setFloatingPanel:YES];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setAllowsMultipleSelection:NO];
    [panel setAllowedFileTypes:fileTypes];
    NSInteger i = [panel runModal];
    if(i == NSModalResponseOK){
        NSURL *url = panel.URL;
        NSString* path = [url.absoluteString stringByReplacingOccurrencesOfString:@"file://" withString:@""];
        MvxPlayer *mvxPlayer = MainController::instance()->getPlayer();
        mvxPlayer->init(path.cString);
        mvxPlayer->prepare();
        mvxPlayer->play();
    }
}

@end
