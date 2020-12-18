//
//  ContentView.swift
//  VocalTraineriOS
//
//  Created by Semyon Tikhonenko on 11/25/20.
//  Copyright © 2020 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        VStack(alignment: .leading) {
            HStack {

            }.frame(maxWidth: .infinity, maxHeight: 100).background(
                    LinearGradient(gradient:
                    Gradient(colors: [.white, .black]),
                            startPoint: .top,
                            endPoint: .bottom)
            )
            Spacer()
        }.frame(maxWidth: .infinity, alignment: .leading)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
