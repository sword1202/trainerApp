//
//  VocalTraineriOSApp.swift
//  VocalTraineriOS
//
//  Created by Semyon Tikhonenko on 11/25/20.
//  Copyright © 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

class AppDelegate: NSObject, UIApplicationDelegate {
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey : Any]? = nil) -> Bool {
        InitializationManager.initializeApplicationIfNeed()
        return true
    }
}

@main
struct VocalTrainerApp: App {
    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate

    var body: some Scene {
        WindowGroup {
            WorkspaceView()
        }
    }
}
