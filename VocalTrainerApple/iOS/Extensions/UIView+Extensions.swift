//
// Created by Semyon Tykhonenko on 2019-06-12.

//

import UIKit

extension UIView {
    private static var nibCache: [String:UINib] = [:]

    class func fromNib<T: UIView>() -> T {
        let key = String(describing: T.self)
        var nib = nibCache[key]
        if nib == nil {
            nib = UINib(nibName: key, bundle: Bundle.main)
            nibCache[key] = nib
        }

        return nib!.instantiate(withOwner: nil)[0] as! T
    }

    func asImage() -> UIImage {
        let renderer = UIGraphicsImageRenderer(bounds: bounds)
        return renderer.image { rendererContext in
            layer.render(in: rendererContext.cgContext)
        }
    }

    var absoluteOrigin: CGPoint {
        get {
            return self.superview?.convert(self.frame.origin, to: nil) ?? self.frame.origin
        }
    }

    static func getHeightNeededToFit(views: [UIView]) -> CGFloat {
        let lastViewFrame = views.max(usingKeyProvider: {
            $0.frame.height + $0.frame.origin.y
        })?.frame ?? CGRect.zero

        return lastViewFrame.height + lastViewFrame.origin.y
    }

    static func getWidthNeededToFit(views: [UIView]) -> CGFloat {
        let lastViewFrame = views.max(usingKeyProvider: {
            $0.frame.width + $0.frame.origin.x
        })?.frame ?? CGRect.zero

        return lastViewFrame.width + lastViewFrame.origin.x
    }

    func getHeightNeededToFitSubviews() -> CGFloat {
        UIView.getHeightNeededToFit(views: self.subviews)
    }

    func getWidthNeededToFitSubviews() -> CGFloat {
        UIView.getWidthNeededToFit(views: self.subviews)
    }

    func addSubviewIfNotExist(_ view: UIView) {
        if !subviews.contains(view) {
            addSubview(view)
        }
    }
    
    var firstResponder: UIView? {
        guard !isFirstResponder else { return self }

        for subview in subviews {
            if let firstResponder = subview.firstResponder {
                return firstResponder
            }
        }

        return nil
    }
}
