#include "DrawerColor.h"
#include "assert.h"

DrawerColor::DrawerColor() {
    rgba[3] = 255;
}

DrawerColor::DrawerColor(uchar r, uchar g, uchar b, uchar a) {
    rgba[0] = r;
    rgba[1] = g;
    rgba[2] = b;
    rgba[3] = a;
}

uchar DrawerColor::operator[](int index) const {
    assert(index >= 0 && index < 4);
    return rgba[index];
}

uchar &DrawerColor::operator[](int index) {
    assert(index >= 0 && index < 4);
    return rgba[index];
}

uchar DrawerColor::r() const {
    return rgba[0];
}

uchar DrawerColor::g() const {
    return rgba[1];
}

uchar DrawerColor::b() const {
    return rgba[2];
}

uchar DrawerColor::a() const {
    return rgba[3];
}

uchar &DrawerColor::r() {
    return rgba[0];
}

uchar &DrawerColor::g() {
    return rgba[1];
}

uchar &DrawerColor::b() {
    return rgba[2];
}

uchar &DrawerColor::a() {
    return rgba[3];
}

DrawerColor DrawerColor::fromHex(int hex) {
    //If we have alpha, it's in the format 0xRRGGBBAA
    if(hex > 0x00FFFFFF) {
        uchar r = static_cast<uchar>((hex >> 24) & 0xff);
        uchar g = static_cast<uchar>((hex >> 16) & 0xff);
        uchar b = static_cast<uchar>((hex >> 8) & 0xff);
        uchar a = static_cast<uchar>(hex & 0xff);
        return DrawerColor(r, g, b, a);
    } else {
        //If we *don't* have alpha, it's in the format 0x00RRGGBB
        uchar r = static_cast<uchar>((hex >> 16) & 0xff);
        uchar g = static_cast<uchar>((hex >> 8) & 0xff);
        uchar b = static_cast<uchar>(hex & 0xff);
        uchar a = 255;
        return DrawerColor(r, g, b, a);
    }
}

DrawerColor DrawerColor::white() {
    return DrawerColor(255, 255, 255, 255);
}

DrawerColor DrawerColor::black() {
    return DrawerColor(0, 0, 0, 255);
}

bool DrawerColor::operator==(const DrawerColor &rhs) const {
    return *reinterpret_cast<const int32_t*>(rgba) == *reinterpret_cast<const int32_t*>(rhs.rgba);
}

bool DrawerColor::operator!=(const DrawerColor &rhs) const {
    return !(rhs == *this);
}
