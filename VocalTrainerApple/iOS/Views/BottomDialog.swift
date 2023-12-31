//
// Created by Semyon Tikhonenko on 3/23/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

import SwiftUI

private struct BlurView: UIViewRepresentable {
    let style: UIBlurEffect.Style

    func makeUIView(context: UIViewRepresentableContext<BlurView>) -> UIView {
        let view = UIView(frame: .zero)
        view.backgroundColor = UIColor(cgColor: Colors.headerBackground.cgColor!)
        let blurEffect = UIBlurEffect(style: style)
        let blurView = UIVisualEffectView(effect: blurEffect)
        blurView.translatesAutoresizingMaskIntoConstraints = false
        view.insertSubview(blurView, at: 0)
        NSLayoutConstraint.activate([
            blurView.heightAnchor.constraint(equalTo: view.heightAnchor),
            blurView.widthAnchor.constraint(equalTo: view.widthAnchor),
        ])
        return view
    }

    func updateUIView(_ uiView: UIView, context: UIViewRepresentableContext<BlurView>) {
    }
}

struct BottomDialogHeader : View {
    private let title: String
    private let titleIcon: String
    @Binding var isShown: Bool

    init(title: String, titleIcon: String, isShown: Binding<Bool>) {
        self.title = title
        self.titleIcon = titleIcon
        _isShown = isShown
    }

    var body: some View {
        HStack {
            ZStack {
                RoundedRectangle(cornerRadius: 5).fill(Colors.tone2).frame(width: 40, height: 40)
                Image(titleIcon)
            }.padding(.leading, 16)
            Text(title).foregroundColor(Colors.tone2)
                    .font(Font.system(size: 24, weight: .semibold))
                    .padding(.leading, 8)
            Spacer()
            CircleButton(icon: "x").padding(.trailing, 16).onTapGesture {
                isShown = false
            }
        }.frame(height: 72).background(BlurView(style: .systemMaterialLight))
    }
}

struct BottomDialog<Content: View> : View {
    private let content: Content
    private let title: String
    private let titleIcon: String

    @Binding var isShown: Bool

    init(title: String, titleIcon: String, isShown: Binding<Bool>, @ViewBuilder content: () -> Content) {
        self.content = content()
        self.title = title
        self.titleIcon = titleIcon
        _isShown = isShown
    }

    var body: some View {
        GeometryReader { geometry in
            VStack(spacing: 0) {
                Spacer()
                VStack(spacing: 0) {
                    BottomDialogHeader(title: title, titleIcon: titleIcon, isShown: $isShown)
                    content
                    Spacer().frame(width: geometry.size.width, height: geometry.safeAreaInsets.bottom)
                            .background(Color.white)
                }

            }.ignoresSafeArea(edges: .bottom)
        }
    }
}
