//
// Created by Semyon Tykhonenko on 2019-08-22.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

import Cocoa
import SwiftHEXColors

class HeaderButton : ToggleButton {
    private func updateShadow() {
        assert(self.layer != nil)
        if self.state == .on {
            self.layer?.removeShadow()
        } else {
            HeaderUiUtils.applyShadow(view: self, cornerRadius: 7.5)
        }
    }

    override var layer: CALayer? {
        didSet {
            // Postpone the shadow update or otherwise the shadow is not updated for some reasons
            DispatchQueue.main.async {
                self.updateShadow()
            }
        }
    }

    override var state: StateValue {
        didSet {
            updateShadow()
        }
    }
}
