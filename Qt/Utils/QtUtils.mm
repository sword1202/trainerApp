//
// Created by Semyon Tikhonenko on 10/29/18.
//

#include "QtUtils.h"

namespace QtUtils {
    NSView* getNSView(QWidget* widget) {
        return (__bridge NSView*)reinterpret_cast<void *>(widget->winId());
    }
}
