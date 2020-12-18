//
//  VocalTraineriOSApp.swift
//  VocalTraineriOS
//
//  Created by Semyon Tikhonenko on 11/25/20.
//  Copyright © 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI
import Combine

struct ProjectController {
    static let shared: ProjectControllerBridge = ProjectControllerBridge()
}

class AppDelegate: NSObject, UIApplicationDelegate {
    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey : Any]? = nil) -> Bool {
        InitializationManager.initializeApplicationIfNeed()
        return true
    }
}

@main
struct VocalTrainerApp: App {
    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
    @Environment(\.scenePhase) private var scenePhase
    @ObservedObject private var viewModel = ProjectViewModel()

    var body: some Scene {
        WindowGroup {
            VStack {
                WorkspaceView().onChange(of: scenePhase) { phase in
                    switch phase {
                    case .active:
                        viewModel.didBecomeActive()
                    case .inactive:
                        viewModel.willBecomeInactive()
                    case .background:
                        print("App goes background")
                    default:
                        print("Unknown state")
                    }
                }
                VStack(alignment: .leading) {
                    Spacer().frame(maxWidth: .infinity, maxHeight: .infinity)
                }.background(Colors.light).frame(maxWidth: .infinity, maxHeight: 147, alignment: .bottomLeading)
            }
        }
    }
}
