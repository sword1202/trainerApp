#include "AudioPlayer.h"
#include "Executors.h"

using namespace CppUtils;

void AudioPlayer::play() {
    play(getSeek());
}

void AudioPlayer::prepareAsync(const std::function<void()>& callback) {
    Executors::ExecuteOnBackgroundThread([=] {
        prepare();
        Executors::ExecuteOnMainThread(callback);
    });
}