//
// Created by Semyon Tikhonenko on 3/30/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct CheckBox : View {
    @Binding var isSelected: Bool
    private let selectedStateChanged: ((Bool) -> Void)?

    init(isSelected: Binding<Bool>, selectedStateChanged: ((Bool) -> Void)? = nil) {
        self.selectedStateChanged = selectedStateChanged
        _isSelected = isSelected
    }

    var body: some View {
        Image(isSelected ? "SelectedCheckBox" : "UnelectedCheckBox").onTapGesture {
            withAnimation {
                isSelected.toggle()
            }
            selectedStateChanged?(isSelected)
        }
    }
}
