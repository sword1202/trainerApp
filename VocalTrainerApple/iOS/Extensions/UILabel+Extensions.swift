//
// Created by Semyon Tykhonenko on 2019-06-06.

//

import UIKit

extension UILabel {
    ///Find the index of character (in the attributedText) at point
    func indexOfCharacter(at point: CGPoint) -> Int {
        assert(self.attributedText != nil, "Currently implemented only for attributed string")
        let textStorage = NSTextStorage(attributedString: self.attributedText!)
        let layoutManager = NSLayoutManager()
        textStorage.addLayoutManager(layoutManager)
        let textContainer = NSTextContainer(size: self.frame.size)
        textContainer.lineFragmentPadding = 0
        textContainer.maximumNumberOfLines = self.numberOfLines
        textContainer.lineBreakMode = self.lineBreakMode
        layoutManager.addTextContainer(textContainer)

        return layoutManager.characterIndex(for: point, in: textContainer,
                fractionOfDistanceBetweenInsertionPoints: nil)
    }

    func setGradient(startColor: UIColor, endColor: UIColor) {
        let gradientText = self.text ?? ""
        let textHeight = self.font.height(text: gradientText)
        self.textColor = UIColor(patternImage: UIImage.gradient(startColor: startColor,
                endColor: endColor,
                size: CGSize(width: 1, height: textHeight)))
    }
}