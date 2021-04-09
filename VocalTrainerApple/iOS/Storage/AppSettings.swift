//
// Created by Semyon Tikhonenko on 4/8/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import Foundation

private let notShowSwipeAndZoomSuggestionKey = "notShowSwipeAndZoomSuggestionKey";
private let notShowBoundsSuggestion = "notShowBoundsSuggestion";

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

    var showBoundsSuggestion: Bool {
        get {
            userDefaults.value(forKey: notShowBoundsSuggestion) as? Bool != true
        }
        set {
            userDefaults.set(!newValue, forKey: notShowBoundsSuggestion)
        }
    }
}
