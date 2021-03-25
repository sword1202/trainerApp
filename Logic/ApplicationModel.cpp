//
// Created by Semyon Tykhonenko on 2019-07-21.
//

#include "ApplicationModel.h"
#include "SfzPitchRenderer.h"
#include "StringUtils.h"

using namespace CppUtils;

static ApplicationModel* model = nullptr;

ApplicationModel::ApplicationModel() {
    auto sfzPath = getSfzFilePath();
    SfzPitchRenderer::initSfz(sfzPath);
    player = new VocalTrainerFilePlayer();
    std::string metronomeFilePath = getMetronomeDataFilePath();
    if (!metronomeFilePath.empty()) {
        std::string metronomeData = Strings::ReadBinaryFileContent(metronomeFilePath.data());
        player->setMetronomeSoundData(std::move(metronomeData));
    }
    audioInputManager = new AudioInputManager("");
}

VocalTrainerFilePlayer *ApplicationModel::getPlayer() {
    return player;
}

AudioInputManager *ApplicationModel::getAudioInputManager() {
    return audioInputManager;
}

void ApplicationModel::init() {
    model = new ApplicationModel();
}

ApplicationModel *ApplicationModel::instance() {
    return model;
}
