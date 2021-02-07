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
        //try! AVAudioSession.sharedInstance().overrideOutputAudioPort(.speaker)
        InitializationManager.initializeApplicationIfNeed()
        ProjectController.shared.togglePlay()
        return true
    }
}

@main
struct VocalTrainerApp: App {
    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate

    var body: some Scene {
        WindowGroup {
            VStack {
                ProjectView()
            }
        }
    }
}
