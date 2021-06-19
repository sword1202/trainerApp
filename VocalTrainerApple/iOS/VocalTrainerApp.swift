//
//  VocalTraineriOSApp.swift
//  VocalTraineriOS
//
//  Created by Semyon Tikhonenko on 11/25/20.
//  Copyright © 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI
import Combine
import AVFoundation

struct ProjectController {
    static let shared: ProjectControllerBridge = ProjectControllerBridge()
}

class AppDelegate: NSObject, UIApplicationDelegate {
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey : Any]? = nil) -> Bool {
        guard !SwiftUIUtils.isPreview() else {
            return true
        }

        try! AVAudioSession.sharedInstance().setCategory(.playAndRecord, options: .allowBluetoothA2DP)
        InitializationManager.initializeApplicationIfNeed()
        let mvxFilePath = Bundle.main.path(forResource: "drm", ofType: "mvx")
        ProjectController.shared.setPlaybackSource(filePath: mvxFilePath)
        return true
    }
}

@main
struct VocalTrainerApp: App {
    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate

    var body: some Scene {
        WindowGroup {
            NavigationView {
                ProjectView()
            }
        }
    }
}
