//
// Created by Semyon Tikhonenko on 3/30/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct CheckBox : View {
    @Binding var isSelected: Bool

    init(isSelected: Binding<Bool>) {
        _isSelected = isSelected
    }

    var body: some View {
        Image(isSelected ? "SelectedCheckBox" : "UnselectedCheckBox")
    }
}
