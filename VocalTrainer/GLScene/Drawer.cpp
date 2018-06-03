#include "Drawer.h"

void Drawer::roundedRect(float x, float y, float w, float h, float r) {
    assert(r >= 0);
    if (r == 0) {
        rect(x, y, w, h);
        return;
    }

    if (w < 2 * r) {
        r = w / 2;
    }
    if (h < 2 * r) {
        r = h / 2;
    }

    beginPath();
    moveTo(x+r, y);
    arcTo(x+w, y,   x+w, y+h, r);
    arcTo(x+w, y+h, x,   y+h, r);
    arcTo(x,   y+h, x,   y,   r);
    arcTo(x,   y,   x+w, y,   r);
    closePath();
}
