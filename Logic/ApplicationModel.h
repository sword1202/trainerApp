//
// Created by Semyon Tykhonenko on 2019-07-21.
//

#ifndef TEXTIMAGESGENERATOR_APPLICATIONMODEL_H
#define TEXTIMAGESGENERATOR_APPLICATIONMODEL_H

#include "VocalTrainerFilePlayer.h"
#include "AudioInputManager.h"

class ApplicationModel {
    VocalTrainerFilePlayer* player;
    AudioInputManager* audioInputManager;
public:
    ApplicationModel();
    static void init(ApplicationModel* instance);
    static ApplicationModel* instance();
    VocalTrainerFilePlayer *getPlayer();

    AudioInputManager *getAudioInputManager();
};


#endif //TEXTIMAGESGENERATOR_APPLICATIONMODEL_H
