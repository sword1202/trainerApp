//
// Created by Semyon Tikhonenko on 9/17/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct NavigationUtil {
    static func popToRootView() {
        findNavigationController(viewController: UIApplication.shared.windows.filter { $0.isKeyWindow }.first?.rootViewController)?
                .popToRootViewController(animated: true)
    }

    private static func findNavigationController(viewController: UIViewController?) -> UINavigationController? {
        guard let viewController = viewController else {
            return nil
        }

        if let navigationController = viewController as? UINavigationController {
            return navigationController
        }

        for childViewController in viewController.children {
            return findNavigationController(viewController: childViewController)
        }

        return nil
    }
}