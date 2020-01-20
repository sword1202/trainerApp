//
// Created by Semyon Tykhonenko on 2019-07-21.
//

#include "ApplicationModel.h"

static ApplicationModel* model = nullptr;

ApplicationModel::ApplicationModel() {
    player = new VocalTrainerFilePlayer();
    audioInputManager = new AudioInputManager("");
}

VocalTrainerFilePlayer *ApplicationModel::getPlayer() {
    return player;
}

AudioInputManager *ApplicationModel::getAudioInputManager() {
    return audioInputManager;
}

void ApplicationModel::init(ApplicationModel *instance) {
    model = instance;
}

ApplicationModel *ApplicationModel::instance() {
    return model;
}
