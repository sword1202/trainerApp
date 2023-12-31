//
// Created by Semyon Tikhonenko on 10/6/18.
//

#ifndef VOCALTRAINER_TEXTIMAGESFACTORY_H
#define VOCALTRAINER_TEXTIMAGESFACTORY_H

#include "Drawer.h"

class QtDrawerTextImagesFactory : public DrawerTextImagesFactory {
    void addImage(Drawer *drawer, int devicePixelRatio, int fontSize, char character,
                      const CppUtils::Color &color, bool useCharacterCode = false);
public:
    void load(Drawer* drawer, int devicePixelRatio);
};

#endif //VOCALTRAINER_TEXTIMAGESFACTORY_H
