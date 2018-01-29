//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <stdexcept>
#include "AudioPlayer.h"
#include <string>
#include "TimeUtils.h"

#include <iostream>

using namespace CppUtils;
using std::cout;

#include <windows.h>
#include <cassert>

struct PlayingData {
	HWAVEOUT     queue;
	WAVEHDR		header;
	uint32_t bufferSizeInBytes;
	char* _pBuffer;
	double currentTime;
	HANDLE waveOutThreadHandle;
	DWORD waveOutThreadId;
	CRITICAL_SECTION criticalSection;
	WAVEFORMATEX format;
};

/*DWORD WINAPI WaveOutThread(void *Arg)
{
	MSG Msg;

	PlayingData* recordingData = (PlayingData*)Arg;
	AudioInputReader::Callback& callback = recordingData->callback;

	while (GetMessage(&Msg, NULL, 0, 0) == TRUE)
	{
		if (MM_WIM_DATA == Msg.message)
		{
			// data is received in current buffer
			WAVEHDR *Hdr = (WAVEHDR *)Msg.lParam;

			// fetch data from the recording queue
			MMRESULT res = waveInUnprepareHeader(recordingData->queue, &recordingData->header[recordingData->currentBuffer], sizeof(WAVEHDR));
			int prevBuf = recordingData->currentBuffer - 1;
			if (prevBuf < 0)
				prevBuf = kNumberBuffers - 1;

			if (res != MMSYSERR_NOERROR)
				continue;

			// enter critical section to process received data
			EnterCriticalSection(&recordingData->criticalSection);

			// call processing callback
			if (callback)
			{
				int16_t* receivedBuffer = &(recordingData->_pBuffer[recordingData->currentBuffer * recordingData->bufferSizeInBytes]);
				callback(receivedBuffer, (recordingData->bufferSizeInBytes) / sizeof(int16_t));
			}

			// increase current buffer index
			recordingData->currentBuffer++;
			if (recordingData->currentBuffer == kNumberBuffers)
				recordingData->currentBuffer = 0;

			// refill received buffer and pass to processing 
			int16_t* pHeaderBuffer = &(recordingData->_pBuffer[prevBuf * recordingData->bufferSizeInBytes]);
			recordingData->header[prevBuf].lpData = (LPSTR)pHeaderBuffer;
			recordingData->header[prevBuf].dwBufferLength = recordingData->bufferSizeInBytes * 2;
			recordingData->header[prevBuf].dwFlags = 0;
			recordingData->header[prevBuf].dwLoops = 0;

			LeaveCriticalSection(&recordingData->criticalSection);

			// prepare processed buffer to receive data again
			res = waveInPrepareHeader(recordingData->queue, &recordingData->header[prevBuf], sizeof(WAVEHDR));
			if (res == MMSYSERR_NOERROR && recordingData->callback)
			{
				waveInAddBuffer(recordingData->queue, &recordingData->header[prevBuf], sizeof(WAVEHDR));
			}
		}

		if (MM_WIM_CLOSE == Msg.message)
		{
			// fetch all buffers and leave the loop
			for (int i = 0; i < kNumberBuffers; i++)
			{
				waveInUnprepareHeader(recordingData->queue, &recordingData->header[i], sizeof(WAVEHDR));
			}
			break;
		}
	}

	return 0;
}*/

class WinAudioPlayer{
	static const int kSamplingRate = 44100;

	PlayingData playingData;
	MMRESULT _status;
	bool bPlaying;
public:
	WinAudioPlayer(const char* audioData, int maximumBufferSize) {
		assert(maximumBufferSize > 0);
		playingData.bufferSizeInBytes = (uint32_t)maximumBufferSize;
		bPlaying = false;

		// fill WAVE data format
		playingData.format.nSamplesPerSec = kSamplingRate;
		playingData.format.wFormatTag = WAVE_FORMAT_PCM;
		playingData.format.nChannels = 1;
		playingData.format.wBitsPerSample = 16;
		playingData.format.nBlockAlign = playingData.format.nChannels * playingData.format.wBitsPerSample / 8;
		playingData.format.nAvgBytesPerSec = playingData.format.nBlockAlign * playingData.format.nSamplesPerSec;
		playingData.format.cbSize = 0;

		// open recording device and null callback, because we are already in a separate thread
		_status = waveOutOpen(
			&playingData.queue,
			WAVE_MAPPER,
			&playingData.format,
			0,
			0,
			CALLBACK_NULL);

		if (_status == MMSYSERR_NOERROR)
		{
			// create kNumberBuffers to store recorded data
			playingData._pBuffer = new char[maximumBufferSize] {0};
			memcpy(playingData._pBuffer, audioData, maximumBufferSize);
		}
	}

	void stop() {
		bPlaying = false;
		if (playingData.header.dwFlags & WHDR_PREPARED) {
			waveOutUnprepareHeader(playingData.queue, &playingData.header, sizeof(WAVEHDR));
		}
	}

	void setCurrentTimestamp(double seek) {
		playingData.currentTime = seek;
	}

	void pause() {
		bPlaying = false;
		waveOutPause(playingData.queue);
	}

	void resume() {
		bPlaying = true;
		waveOutRestart(playingData.queue);
	}

	bool isPlaying() {
		return bPlaying;
	}

	MMRESULT setupHeaderData()
	{
		if (playingData.header.dwFlags & WHDR_PREPARED) {
			waveOutUnprepareHeader(playingData.queue, &playingData.header, sizeof(WAVEHDR));
		}

		// calculate position by timestamp (ms)
		DWORD cb = (int)playingData.currentTime * playingData.format.nAvgBytesPerSec / 1000;
		if (cb % playingData.format.nBlockAlign) 
		{
			if ((cb % playingData.format.nBlockAlign) <= (playingData.format.nBlockAlign / 2)) {
				cb -= (cb % playingData.format.nBlockAlign);
			}
			else {
				cb += (playingData.format.nBlockAlign - (cb % playingData.format.nBlockAlign));
			}
		}

		if (cb<0 || cb >= playingData.bufferSizeInBytes) 
			return MMSYSERR_BADERRNUM;

		waveOutReset(playingData.queue);
		
		ZeroMemory(&playingData.header, sizeof(WAVEHDR));
		char* pHeaderBuffer = playingData._pBuffer;
		playingData.header.lpData = (LPSTR)(pHeaderBuffer + cb);
		playingData.header.dwBufferLength = playingData.bufferSizeInBytes - cb;
		playingData.header.dwFlags = 0;
		playingData.header.dwLoops = 0;

		MMRESULT res = waveOutPrepareHeader(playingData.queue, &playingData.header, sizeof(WAVEHDR));
		return res;
	}

	void play() {

		_status = setupHeaderData();
		if (_status == MMSYSERR_NOERROR)
		{
			_status = waveOutWrite(playingData.queue, &playingData.header, sizeof(WAVEHDR));
		}
		
		if (_status != MMSYSERR_NOERROR)
		{
			std::string message = "Failed to play audio data, errorCode: ";
			message += std::to_string(_status);
			throw std::runtime_error(message);
		}

		bPlaying = true;
	}

	~WinAudioPlayer() {
		if (_status == MMSYSERR_NOERROR)
		{
			stop();
			waveOutReset(playingData.queue);
			waveOutClose(playingData.queue);
		}
	}
};

void AudioPlayer::play(const char *audioData, int size, double seek) 
{
    int64_t i = TimeUtils::NowInMicroseconds();
	WinAudioPlayer *audioPlayer = new WinAudioPlayer(audioData, size);
    
	audioPlayer->setCurrentTimestamp(seek);
	audioPlayer->play();
    if (this->player)
	{
		delete this->player;
    }

	this->player = (void*)audioPlayer;
}

bool AudioPlayer::isPlaying() {
    return player && ((WinAudioPlayer*)player)->isPlaying();
}

void AudioPlayer::stop() {
    if (this->player) {
		WinAudioPlayer* pointer = (WinAudioPlayer*)this->player;
		pointer->stop();
		delete pointer;
        this->player = 0;
    }
}

AudioPlayer::~AudioPlayer() {
    if (this->player) 
	{
		WinAudioPlayer* pointer = (WinAudioPlayer*)this->player;
		delete pointer;
    }
}

void AudioPlayer::pause() {
	if (this->player)
	{
		WinAudioPlayer* pointer = (WinAudioPlayer*)this->player;
		pointer->pause();
	}
}

void AudioPlayer::resume() {
	if (this->player)
	{
		WinAudioPlayer* pointer = (WinAudioPlayer*)this->player;
		pointer->resume();
	}
}

void AudioPlayer::seek(double timeStamp) {
    if (this->player)
	{
		WinAudioPlayer* pointer = (WinAudioPlayer*)this->player;
		pointer->setCurrentTimestamp(timeStamp);
	}
}
