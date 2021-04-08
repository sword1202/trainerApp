//
// Created by Semyon Tikhonenko on 4/8/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

private let notShowSwipeAndZoomSuggestionKey = "notShowSwipeAndZoomSuggestionKey";

class AppSettings {
    static let shared = AppSettings()

    private let userDefaults = UserDefaults.standard

    var showSwipeAndZoomSuggestion: Bool {
        get {
            userDefaults.value(forKey: notShowSwipeAndZoomSuggestionKey) as? Bool != true
        }
        set {
            userDefaults.set(!newValue, forKey: notShowSwipeAndZoomSuggestionKey)
        }
    }
}
