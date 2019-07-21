//
// Created by Semyon Tykhonenko on 2019-07-21.
//

#ifndef TEXTIMAGESGENERATOR_APPLICATIONMODEL_H
#define TEXTIMAGESGENERATOR_APPLICATIONMODEL_H

#include "MvxPlayer.h"
#include "AudioInputManager.h"

class ApplicationModel {
    MvxPlayer* player;
    AudioInputManager* audioInputManager;
    ApplicationModel();
public:
    static void init(ApplicationModel* instance);
    static ApplicationModel* instance();
    MvxPlayer *getPlayer();

    AudioInputManager *getAudioInputManager();
};


#endif //TEXTIMAGESGENERATOR_APPLICATIONMODEL_H
