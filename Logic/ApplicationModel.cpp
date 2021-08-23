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
    std::string metronomeFilePath = getMetronomeDataFilePath();
    if (!metronomeFilePath.empty()) {
        std::string metronomeData = Strings::ReadBinaryFileContent(metronomeFilePath.data());
        MetronomeAudioPlayer::setMetronomeAudioData(std::move(metronomeData));
    }
}

VocalTrainerFilePlayer *ApplicationModel::createPlayer() {
    return new VocalTrainerFilePlayer();
}

AudioInputManager *ApplicationModel::createAudioInputManager() {
    return new AudioInputManager("");
}

void ApplicationModel::init() {
    model = new ApplicationModel();
}

ApplicationModel *ApplicationModel::instance() {
    return model;
}
