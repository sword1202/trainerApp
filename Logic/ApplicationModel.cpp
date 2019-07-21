//
// Created by Semyon Tykhonenko on 2019-07-21.
//

#include "ApplicationModel.h"

static ApplicationModel* model = nullptr;

ApplicationModel::ApplicationModel() {
    player = new MvxPlayer();
    audioInputManager = new AudioInputManager("");
}

MvxPlayer *ApplicationModel::getPlayer() {
    return player;
}

AudioInputManager *ApplicationModel::getAudioInputManager() {
    return audioInputManager;
}

void ApplicationModel::init(ApplicationModel *instance) {
    model = instance;
}

ApplicationModel *ApplicationModel::instance() {
    assert(model && "Call init before instance");
    return model;
}
