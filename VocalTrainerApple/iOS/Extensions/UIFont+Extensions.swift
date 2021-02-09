//
// Created by Semyon Tykhonenko on 2019-06-18.

//

import UIKit

extension UIFont {
    func width(text: String) -> CGFloat {
        size(text: text).width
    }

    func height(text: String) -> CGFloat {
        size(text: text).height
    }

    func size(text: String) -> CGSize {
        let fontAttributes = [NSAttributedString.Key.font: self]
        return (text as NSString).size(withAttributes: fontAttributes)
    }
}