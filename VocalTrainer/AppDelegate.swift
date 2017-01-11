//
//  AppDelegate.swift
//  VocalTrainer
//
//  Created by Semyon Tikhonenko on 8/8/16.
//  Copyright © 2016 Mac. All rights reserved.
//

import Cocoa

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {



    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }

    func applicationDidBecomeActive(_ notification: Notification) {
        //mainMenu.
    }

    func application(_ sender: NSApplication, openFile filename: String) -> Bool {
        return true
    }
}

