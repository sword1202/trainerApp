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

class RecordingsListController : RecordingsListControllerBridge {
    static func recordingsPath() -> String {
        let documentsDirectory = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
        return documentsDirectory + "/Recordings/"
    }

    override init() {
        super.init(recordingsPath: RecordingsListController.recordingsPath())
    }
}

class ProjectController : ProjectControllerBridge {
    static let shared = ProjectController()

    override init() {
        super.init(recordingsPath: RecordingsListController.recordingsPath())
    }
}

class AppDelegate: NSObject, UIApplicationDelegate {
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey : Any]? = nil) -> Bool {
        guard !SwiftUIUtils.isPreview() else {
            return true
        }

        try! AVAudioSession.sharedInstance().setCategory(.playAndRecord, options: .allowBluetoothA2DP)
        InitializationManager.initializeApplicationIfNeed()
        return true
    }
}

@main
struct VocalTrainerApp: App {
    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate

    var body: some Scene {
        WindowGroup {
            NavigationView {
                RecordingsView().navigationBarHidden(true)
            }
        }
    }
}
