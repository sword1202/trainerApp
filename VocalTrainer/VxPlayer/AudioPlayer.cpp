//
// Created by Semyon Tikhonenko on 1/5/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <stdexcept>
#include "AudioPlayer.h"
#include <string>
#include "TimeUtils.h"
#include "WAVFile.h"
#include <sstream>

#include <iostream>

using namespace CppUtils;
using std::cout;

#include <windows.h>
#include <cassert>

void AudioPlayer::play(const char *audioData, int size, double seek) 
{
	if (playingData._pBuffer)
	{
		stop();
		waveOutReset(playingData.queue);
		waveOutClose(playingData.queue);
	}

    int64_t i = TimeUtils::NowInMicroseconds();
	initAudioDevice(audioData, size); 
	playingData.currentTime = seek;

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

bool AudioPlayer::isPlaying() {
	return bPlaying;
}

void AudioPlayer::stop() {
	bPlaying = false;
	if (playingData.header.dwFlags & WHDR_PREPARED) {
		waveOutUnprepareHeader(playingData.queue, &playingData.header, sizeof(WAVEHDR));
	}
}

AudioPlayer::~AudioPlayer() {
	if (_status == MMSYSERR_NOERROR)
	{
		stop();
		waveOutReset(playingData.queue);
		waveOutClose(playingData.queue);
	}
}

void AudioPlayer::pause() {
	bPlaying = false;
	waveOutPause(playingData.queue);
}

void AudioPlayer::resume() {
	bPlaying = true;
	waveOutRestart(playingData.queue);
}

void AudioPlayer::seek(double timeStamp) {
	playingData.currentTime = timeStamp;
}

MMRESULT AudioPlayer::setupHeaderData()
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

void AudioPlayer::initAudioDevice(const char* audioData, int size)
{
	assert(size > 0);
	playingData.bufferSizeInBytes = (uint32_t)size;
	bPlaying = false;

	if (playingData._pBuffer)
		delete playingData._pBuffer;
	
	playingData._pBuffer = nullptr;

	// parse stream
	std::stringstream stream(std::ios::in | std::ios::out | std::ios::binary);
	stream.write((char*)audioData, size);

	WAVFile wavFileParser(&stream);
	if (!wavFileParser.isValid())
	{
		std::string message = "Failed to read audio data";
		throw std::runtime_error(message);
	}

	// fill WAVE data format
	playingData.format.nSamplesPerSec = wavFileParser.getSampleRate();
	playingData.format.wFormatTag = WAVE_FORMAT_PCM;
	playingData.format.nChannels = wavFileParser.getNumberOfChannels();
	playingData.format.wBitsPerSample = wavFileParser.getBytesPerChannel() * 8;
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
		if (playingData._pBuffer)
			delete playingData._pBuffer;

		// read audio data from its start position and update buffer size parameter
		playingData._pBuffer = new char[size] {0};
		int rawWavDataSize = wavFileParser.readData(playingData._pBuffer, size);
		playingData.bufferSizeInBytes = rawWavDataSize;
	}
}