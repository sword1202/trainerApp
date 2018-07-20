//
// Created by Semyon Tikhonenko on 1/4/17.
// Copyright (c) 2017 Mac. All rights reserved.
//

#import "MainViewController.h"
#import "MainController.h"
#import "PlayHeadView.h"

@implementation MainViewController {
    
}

- (void)viewDidLoad {
    [super viewDidLoad];
    PlayHeadView *playHeadView = [[PlayHeadView alloc] initWithHeight:self.view.frame.size.height];
    playHeadView.position = 100;
    [self.view addSubview:playHeadView];
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
        MvxPlayer *mvxPlayer = MainController::instance()->getMvxPlayer();
        mvxPlayer->init(path.cString);
        mvxPlayer->prepare();
        mvxPlayer->play();
    }
}

@end
