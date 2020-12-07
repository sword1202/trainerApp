#include "AudioPlayer.h"
#include "Executors.h"
#include "AudioUtils.h"
#include "AudioOperationFailedException.h"
#include <boost/assert.hpp>

using namespace CppUtils;
using namespace std::placeholders;

void AudioPlayer::writerCallback(void* outputBuffer, int samplesPerBuffer) {
    memset(outputBuffer, 0, static_cast<size_t>(playbackData.getCallbackBufferSizeInBytes()));
    int samplesCopiedToOutputBufferCount = readAudioDataApplySoundTouchIfNeed(outputBuffer, samplesPerBuffer);

    assert(samplesCopiedToOutputBufferCount <= samplesPerBuffer);
    int sampleSize = getSampleSize();

    // no data available, return silence and wait
    if (samplesCopiedToOutputBufferCount < 0) {
        Executors::ExecuteOnMainThread([=] {
            onNoDataAvailableListeners.executeAll();
        });
        return;
    } else {
        if (volume == 0.0f) {
            memset(outputBuffer, 0, sampleSize * samplesCopiedToOutputBufferCount);
        } else if (volume != 1.0f) {
            int bufferSize = samplesCopiedToOutputBufferCount * playbackData.numberOfChannels;
            int bytesPerChannel = playbackData.getBytesPerChannel();
            switch (bytesPerChannel) {
                case 1:
                    for (int i = 0; i < bufferSize; ++i) {
                        static_cast<int8_t*>(outputBuffer)[i] *= volume;
                    }
                    break;
                case 2:
                    for (int i = 0; i < bufferSize; ++i) {
                        static_cast<int16_t*>(outputBuffer)[i] *= volume;
                    }
                    break;
                case 4:
                    for (int i = 0; i < bufferSize; ++i) {
                        static_cast<int32_t*>(outputBuffer)[i] *= volume;
                    }
                    break;
                default:
                    throw std::runtime_error("Unsupported bytesPerChannel " + std::to_string(bytesPerChannel));
            }
        }

        onDataSentToOutputListeners.executeAll(outputBuffer, samplesCopiedToOutputBufferCount * sampleSize);

        if (samplesCopiedToOutputBufferCount != samplesPerBuffer) {
            if (!looping) {
                onComplete();
                writer->stop();
            } else {
                setBufferSeek(0);
            }
        }
    }
}

void AudioPlayer::play() {
    play(getSeek());
}

void AudioPlayer::prepareAsync(const std::function<void()>& callback) {
    Executors::ExecuteOnBackgroundThread([=] {
        prepare();
        Executors::ExecuteOnMainThread(callback);
    });
}

void AudioPlayer::prepare() {
    assert(!isPrepared());
    providePlaybackData(&playbackData);

    if (playbackData.sampleRate <= 0 ||
            playbackData.samplesPerBuffer < 0 ||
            playbackData.numberOfChannels <= 0 ||
            playbackData.totalDurationInSeconds < 0) {
        throw AudioOperationFailedException("providePlaybackData doesn't initialise all the required fields");
    }

    // Init soundtouch
    if (soundTouch) {
        soundTouch->setChannels(playbackData.numberOfChannels);
        soundTouch->setSampleRate(playbackData.sampleRate);
        assert(playbackData.samplesPerBuffer > 0 && playbackData.numberOfChannels > 0);
        soundTouchTempFloatBuffer.resize(static_cast<size_t>(playbackData.samplesPerBuffer) * playbackData.numberOfChannels);
    }

    writer = AudioOutputWriter::create(playbackData);
    writer->callback = std::bind(&AudioPlayer::writerCallback, this, _1, _2);

    onNoDataAvailableListeners.addListener([=] {
        onPlaybackStoppedListeners.executeAll();
    });
}

void AudioPlayer::play(double seek) {
    BOOST_ASSERT_MSG(isPrepared(), "call prepare before play");
    BOOST_ASSERT(seek >= 0);

    if (playing || dataSentToOutputListenerKey != 0) {
        return;
    }

    completed = false;
    setSeek(seek);

    setupPlaybackStartedListener();
    writer->start();
}

bool AudioPlayer::isPlaying() const {
    return playing;
}

AudioPlayer::~AudioPlayer() {
    destroy();
    delete soundTouch;
    soundTouch = nullptr;
}

void AudioPlayer::destroy() {
    playing = false;
    playbackData = PlaybackData();
    delete writer;
    writer = nullptr;
}

void AudioPlayer::pause() {
    assert(isPrepared());
    if (!playing) {
        return;
    }

    playing = false;
    writer->stop();
    onDataSentToOutputListeners.removeListener(dataSentToOutputListenerKey);
    onPlaybackStoppedListeners.executeAll();
}

void AudioPlayer::setSeek(double timeStamp) {
    assert(timeStamp >= 0);
    double durationInSeconds = getTrackDurationInSeconds();
    if (timeStamp > durationInSeconds) {
        timeStamp = durationInSeconds;
    }

    if (timeStamp < 0) {
        timeStamp = 0;
    }

    setBufferSeek(secondsSeekToBufferSeek(timeStamp));
}

double AudioPlayer::getTrackDurationInSeconds() const {
    return playbackData.totalDurationInSeconds;
}

AudioPlayer::AudioPlayer() {
    playing = false;
    volume = 1.0f;
    pitchShift = 0;
    tempoFactor = 1;
    looping = false;
    completed = false;
}

float AudioPlayer::getVolume() const {
    return volume;
}

void AudioPlayer::setVolume(float volume) {
    BOOST_ASSERT(volume >= 0.0f && volume <= 1.0f);
    this->volume = volume;
}

double AudioPlayer::getSeek() const {
    return bufferSeekToSecondsSeek(getBufferSeek());
}

int AudioPlayer::secondsToSamplesCount(double secondsSeek) const {
    return (int)round(secondsSeek * playbackData.sampleRate);
}

double AudioPlayer::samplesCountToSeconds(int samplesCount) const {
    return AudioUtils::GetSampleTimeInSeconds(samplesCount, playbackData.sampleRate);
}

void AudioPlayer::onComplete() {
    playing = false;
    completed = true;
    setSeek(0);

    Executors::ExecuteOnMainThread([=] {
        onPlaybackStoppedListeners.executeAll();
        onCompleteListeners.executeAll();
    });
}

int AudioPlayer::getSampleSize() const {
    return playbackData.getSampleBytesCount();
}

double AudioPlayer::bufferSeekToSecondsSeek(int bufferSeek) const {
    return samplesCountToSeconds(bufferSeek);
}

int AudioPlayer::secondsSeekToBufferSeek(double timestamp) const {
    return secondsToSamplesCount(timestamp);
}

void AudioPlayer::setBufferSeek(int bufferSeek) {
    double seek = bufferSeekToSecondsSeek(bufferSeek);
    double total = getTrackDurationInSeconds();
    seekChangedListeners.executeAll(seek, total);
}

int AudioPlayer::getPitchShiftInSemiTones() const {
    return pitchShift;
}

void AudioPlayer::setPitchShiftInSemiTones(int value) {
    if (value == pitchShift) {
        return;
    }

    pitchShift = value;
    onTonalityChanged(value);
}

const PlaybackData &AudioPlayer::getPlaybackData() const {
    return playbackData;
}

void AudioPlayer::setupPlaybackStartedListener() {
    assert(dataSentToOutputListenerKey == 0);
    dataSentToOutputListenerKey = onDataSentToOutputListeners.addOneShotListener([=] (void*, int) {
        Executors::ExecuteOnMainThread([this] {
            playing = true;
            onPlaybackStartedListeners.executeAll();
            dataSentToOutputListenerKey = 0;
        });
    });
}

bool AudioPlayer::isPrepared() const {
    return writer != nullptr;
}

double AudioPlayer::getTempoFactor() const {
    return tempoFactor;
}

void AudioPlayer::setTempoFactor(double tempoFactor) {
    this->tempoFactor = tempoFactor;
}

double AudioPlayer::getCallbackBufferDurationInSeconds() const {
    return AudioUtils::GetSampleTimeInSeconds(playbackData.samplesPerBuffer, playbackData.sampleRate);
}

bool AudioPlayer::isLooping() const {
    return looping;
}

void AudioPlayer::setLooping(bool looping) {
    this->looping = looping;
}

void AudioPlayer::setTotalDurationInSeconds(double totalDurationInSeconds) {
    playbackData.totalDurationInSeconds = totalDurationInSeconds;
}

void AudioPlayer::setPlaybackData(const PlaybackData &playbackData) {
    this->playbackData = playbackData;
}

bool AudioPlayer::isCompleted() const {
    return completed;
}

const std::string &AudioPlayer::getPlayerName() const {
    return playerName;
}

void AudioPlayer::setPlayerName(const std::string &playerName) {
    this->playerName = playerName;
}

void AudioPlayer::initSoundTouch() {
    assert(!soundTouch && "SoundTouch already initialised");
    soundTouch = new soundtouch::SoundTouch();
}

void AudioPlayer::onTonalityChanged(int value) {
    assert(pitchShift == 0 || soundTouch && "tonality changes are not allowed, soundtouch not "
                                            "initialised, call initSoundTouch() before prepare to use pitch shifting");
    soundTouch->setPitchSemiTones(value);
}

int AudioPlayer::readAudioDataApplySoundTouchIfNeed(void *outputBuffer, int requestedSamplesCount) {
    // Apply tempo and tonality changes, if need.
    if (soundTouch && (pitchShift != 0 || tempoFactor != 1)) {
        auto* samplesData = static_cast<int16_t *>(outputBuffer);

        while (soundTouch->numSamples() < requestedSamplesCount) {
            int readFramesCount = readNextSamplesBatch(outputBuffer, requestedSamplesCount, playbackData);
            int dataArraySize = readFramesCount * playbackData.numberOfChannels;
            AudioUtils::Int16SamplesIntoFloatSamples(samplesData,
                    dataArraySize,
                    soundTouchTempFloatBuffer.data());
            soundTouch->putSamples(soundTouchTempFloatBuffer.data(), uint(readFramesCount));

            // End of a track reached
            if (readFramesCount != requestedSamplesCount) {
                break;
            }
        }

        int readSamplesCount = std::min(soundTouch->numSamples(), uint(requestedSamplesCount));
        int dataArraySize = readSamplesCount * playbackData.numberOfChannels;
        soundTouch->receiveSamples(soundTouchTempFloatBuffer.data(), uint(readSamplesCount));
        AudioUtils::FloatSamplesIntoInt16Samples(soundTouchTempFloatBuffer.data(), dataArraySize, samplesData);
        return readSamplesCount;
    } else {
        return readNextSamplesBatch(outputBuffer, requestedSamplesCount, playbackData);
    }
}
