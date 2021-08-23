//
// Created by Semyon Tykhonenko on 2019-07-21.
//

#ifndef TEXTIMAGESGENERATOR_APPLICATIONMODEL_H
#define TEXTIMAGESGENERATOR_APPLICATIONMODEL_H

#include "VocalTrainerFilePlayer.h"
#include "AudioInputManager.h"

class ApplicationModel {
    std::string getSfzFilePath() const;
    std::string getMetronomeDataFilePath() const;
public:
    ApplicationModel();
    static void init();
    static ApplicationModel* instance();
    VocalTrainerFilePlayer *createPlayer();
    AudioInputManager *createAudioInputManager();
};


#endif //TEXTIMAGESGENERATOR_APPLICATIONMODEL_H
