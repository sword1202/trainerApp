#include "BaseAudioPlayer.h"
#include "Executors.h"
#include "AudioUtils.h"
#include "AudioOperationFailedException.h"
#include <boost/assert.hpp>
#include "MathUtils.h"
#include "Primitives.h"
#include "Functions.h"

using namespace CppUtils;
using namespace std::placeholders;

void BaseAudioPlayer::writerCallback(void* outputBuffer, int samplesPerBuffer) {
    memset(outputBuffer, 0, static_cast<size_t>(playbackData.getCallbackBufferSizeInBytes()));
    int samplesCopiedToOutputBufferCount = readAudioDataApplySoundTouchIfNeed(outputBuffer, samplesPerBuffer);

    assert(samplesCopiedToOutputBufferCount <= samplesPerBuffer);
    int sampleSize = getSampleSize();

    // no data available, return silence and wait
    if (samplesCopiedToOutputBufferCount < 0) {
        executeOnMainThread([=] {
            onNoDataAvailableListeners.executeAll();
        });
        return;
    } else {
        if (volume == 0.0f) {
            memset(outputBuffer, 0, sampleSize * samplesCopiedToOutputBufferCount);
        } else if (volume < 1.0f) {
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
        } else if (volume > 1.0f) {
            // handle overload
            int bufferSize = samplesCopiedToOutputBufferCount * playbackData.numberOfChannels;
            int bytesPerChannel = playbackData.getBytesPerChannel();

            int lowLimit, highLimit;
            switch (bytesPerChannel) {
                case 1:
                    lowLimit = std::numeric_limits<int8_t>::min();
                    highLimit = std::numeric_limits<int8_t>::max();
                    for (int i = 0; i < bufferSize; ++i) {
                        int8_t* ptr = static_cast<int8_t*>(outputBuffer) + i;
                        int newValue = Math::CutIfOutOfClosedRange(*ptr * volume, lowLimit, highLimit);
                        *ptr = static_cast<int8_t>(newValue);
                    }
                    break;
                case 2:
                    lowLimit = std::numeric_limits<int16_t>::min();
                    highLimit = std::numeric_limits<int16_t>::max();
                    for (int i = 0; i < bufferSize; ++i) {
                        int16_t* ptr = static_cast<int16_t*>(outputBuffer) + i;
                        int newValue = Math::CutIfOutOfClosedRange(*ptr * volume, lowLimit, highLimit);
                        *ptr = static_cast<int16_t>(newValue);
                    }
                    break;
                case 4:
                    lowLimit = std::numeric_limits<int32_t>::min();
                    highLimit = std::numeric_limits<int32_t>::max();
                    for (int i = 0; i < bufferSize; ++i) {
                        int32_t* ptr = static_cast<int32_t*>(outputBuffer) + i;
                        int newValue = Math::CutIfOutOfClosedRange(*ptr * volume, lowLimit, highLimit);
                        *ptr = static_cast<int32_t>(newValue);
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

void BaseAudioPlayer::play() {
    play(getSeek());
}

void BaseAudioPlayer::prepare() {
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

    delete writer;
    writer = AudioOutputWriter::create(playbackData);
    writer->callback = std::bind(&BaseAudioPlayer::writerCallback, this, _1, _2);

    int key = onNoDataAvailableListeners.addListener([=] {
        onPlaybackStoppedListeners.executeAll();
    });
    onResetQueue.push_back([=] {
        onNoDataAvailableListeners.removeListener(key);
    });
}

void BaseAudioPlayer::play(double seek) {
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

bool BaseAudioPlayer::isPlaying() const {
    return playing;
}

BaseAudioPlayer::~BaseAudioPlayer() {
    reset();
    delete soundTouch;
    soundTouch = nullptr;
}

void BaseAudioPlayer::reset() {
    return;
    playing = false;
    playbackData = PlaybackData();
    delete writer;
    writer = nullptr;

    Functions::ExecuteAllAndClear(onResetQueue);
    cancelAllOperations();
}

void BaseAudioPlayer::pause() {
    assert(isPrepared());
    if (!playing) {
        return;
    }

    playing = false;
    writer->stop();
    onDataSentToOutputListeners.removeListener(dataSentToOutputListenerKey);
    executeOnMainThread([this] {
        this->onPlaybackStoppedListeners.executeAll();
    });
}

void BaseAudioPlayer::setSeek(double timeStamp) {
    assert(timeStamp >= 0);
    double durationInSeconds = getTrackDurationInSecondsWithTempoApplied();
    if (timeStamp > durationInSeconds) {
        timeStamp = durationInSeconds;
    }

    if (timeStamp < 0) {
        timeStamp = 0;
    }

    setBufferSeek(secondsSeekToBufferSeek(timeStamp));
}

double BaseAudioPlayer::getOriginalTrackDurationInSeconds() const {
    return playbackData.totalDurationInSeconds;
}

BaseAudioPlayer::BaseAudioPlayer() {
    playing = false;
    volume = 1.0f;
    pitchShift = 0;
    tempoFactor = 1;
    looping = false;
    completed = false;
}

float BaseAudioPlayer::getVolume() const {
    return volume;
}

void BaseAudioPlayer::setVolume(float volume) {
    BOOST_ASSERT(volume >= 0.0f && volume <= 10.0f);
    this->volume = volume;
}

double BaseAudioPlayer::getSeek() const {
    return bufferSeekToSecondsSeek(getBufferSeek());
}

void BaseAudioPlayer::onComplete() {
    playing = false;
    completed = true;

    executeOnMainThread([=] {
        onPlaybackStoppedListeners.executeAll();
        onCompleteListeners.executeAll();
    });
}

int BaseAudioPlayer::getSampleSize() const {
    return playbackData.getSampleBytesCount();
}

double BaseAudioPlayer::bufferSeekToSecondsSeek(int bufferSeek) const {
    return AudioUtils::GetSampleTimeInSeconds(bufferSeek, playbackData.sampleRate) / tempoFactor;
}

int BaseAudioPlayer::secondsSeekToBufferSeek(double timestamp) const {
    return Math::RoundToInt(timestamp * playbackData.sampleRate / tempoFactor);
}

void BaseAudioPlayer::setBufferSeek(int bufferSeek) {
    double seek = bufferSeekToSecondsSeek(bufferSeek);
    double total = getTrackDurationInSecondsWithTempoApplied();
    seekChangedListeners.executeAll(seek, total);
}

int BaseAudioPlayer::getPitchShiftInSemiTones() const {
    return pitchShift;
}

void BaseAudioPlayer::setPitchShiftInSemiTones(int value) {
    if (value == pitchShift) {
        return;
    }

    pitchShift = value;
    onTonalityChanged(value);
}

const PlaybackData &BaseAudioPlayer::getPlaybackData() const {
    return playbackData;
}

void BaseAudioPlayer::setupPlaybackStartedListener() {
    assert(dataSentToOutputListenerKey == 0);
    dataSentToOutputListenerKey = onDataSentToOutputListeners.addOneShotListener([=] (void*, int) {
        executeOnMainThread([this] {
            playing = true;
            onPlaybackStartedListeners.executeAll();
            dataSentToOutputListenerKey = 0;
        });
    });
}

bool BaseAudioPlayer::isPrepared() const {
    return writer != nullptr;
}

double BaseAudioPlayer::getTempoFactor() const {
    return tempoFactor;
}

void BaseAudioPlayer::setTempoFactor(double tempoFactor) {
    if (Primitives::CompareFloatsUsingEpsilon(double(this->tempoFactor), tempoFactor, 0.000001)) {
        return;
    }

    double oldValue = this->tempoFactor;
    this->tempoFactor = tempoFactor;
    onTempoFactorChanged(tempoFactor, oldValue);
}

double BaseAudioPlayer::getCallbackBufferDurationInSeconds() const {
    return AudioUtils::GetSampleTimeInSeconds(playbackData.samplesPerBuffer, playbackData.sampleRate);
}

bool BaseAudioPlayer::isLooping() const {
    return looping;
}

void BaseAudioPlayer::setLooping(bool looping) {
    this->looping = looping;
}

void BaseAudioPlayer::setTotalDurationInSeconds(double totalDurationInSeconds) {
    playbackData.totalDurationInSeconds = totalDurationInSeconds;
}

void BaseAudioPlayer::setPlaybackData(const PlaybackData &playbackData) {
    this->playbackData = playbackData;
}

bool BaseAudioPlayer::isCompleted() const {
    return completed;
}

const std::string &BaseAudioPlayer::getPlayerName() const {
    return playerName;
}

void BaseAudioPlayer::setPlayerName(const std::string &playerName) {
    this->playerName = playerName;
}

void BaseAudioPlayer::initSoundTouch() {
    assert(!soundTouch && "SoundTouch already initialised");
    soundTouch = new soundtouch::SoundTouch();
}

void BaseAudioPlayer::onTonalityChanged(int value) {
    assert(pitchShift == 0 || soundTouch && "tonality changes are not allowed, soundtouch not "
                                            "initialised, call initSoundTouch() before prepare to use pitch shifting");
    soundTouch->setPitchSemiTones(value);
}

void BaseAudioPlayer::onTempoFactorChanged(double value, double oldValue) {
    assert(value == 1 || soundTouch && "tempo changes are not allowed, soundtouch not "
                                            "initialised, call initSoundTouch() before prepare to use tempo changing");
    soundTouch->setTempo(value);
}

int BaseAudioPlayer::readAudioDataApplySoundTouchIfNeed(void *outputBuffer, int requestedSamplesCount) {
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

double BaseAudioPlayer::getTrackDurationInSecondsWithTempoApplied() const {
    return getOriginalTrackDurationInSeconds() / tempoFactor;
}
