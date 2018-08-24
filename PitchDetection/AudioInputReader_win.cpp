//
// Created by Semyon Tikhonenko on 8/7/16.
//

#include "AudioInputReader.h"
#include <windows.h>
#include <cassert>


struct RecordingData {
	AudioInputReader::Callback callback;
	HWAVEIN     queue;
	WAVEHDR		header[kNumberBuffers];
	int			currentBuffer;
	uint32_t bufferSizeInBytes;
	int16_t *_pBuffer;
	HANDLE waveInThreadHandle;
	DWORD waveInThreadId;
	CRITICAL_SECTION criticalSection;
};

DWORD WINAPI WaveInThread(void *Arg)
{
	MSG Msg;

	RecordingData* recordingData = (RecordingData*)Arg;
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
}

class WinAudioInputReader : public AudioInputReader {
	static const int kSamplingRate = 44100;

    RecordingData recordingData;
    MMRESULT _status;
public:
    WinAudioInputReader(int maximumBufferSize) {
        assert(maximumBufferSize > 0);
        recordingData.bufferSizeInBytes = (uint32_t)maximumBufferSize;

        // fill WAVE data format
        WAVEFORMATEX format;
        format.nSamplesPerSec = kSamplingRate;
		format.wFormatTag = WAVE_FORMAT_PCM;
		format.nChannels = 1;
		format.wBitsPerSample = 16;
		format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;
		format.nAvgBytesPerSec = format.nBlockAlign * format.nSamplesPerSec;
		format.cbSize = 0;

		// initialize critical section resource
		InitializeCriticalSection(&recordingData.criticalSection);

		// start recording and processing thread
		recordingData.waveInThreadHandle = CreateThread(NULL, 0, WaveInThread, (LPVOID)(&recordingData), 0, &recordingData.waveInThreadId);
		
		// open recording device and pass thread ID as a callback processing
		// Note: passing callback function instead of thread can lead to a deadlock when calling waveXXX functions inside it
		_status = waveInOpen(
			&recordingData.queue,
			WAVE_MAPPER,
			&format,
			(DWORD_PTR)recordingData.waveInThreadId,
			0,
			CALLBACK_THREAD);

		if (_status == MMSYSERR_NOERROR)
		{
			// create kNumberBuffers to store recorded data
			recordingData.currentBuffer = 0;
			recordingData._pBuffer = new int16_t[maximumBufferSize * kNumberBuffers];
			for (int i = 0; i < kNumberBuffers - 1; i++)
			{
				int16_t* pHeaderBuffer = &recordingData._pBuffer[i * maximumBufferSize];
				recordingData.header[i].lpData = (LPSTR)pHeaderBuffer;
				recordingData.header[i].dwBufferLength = maximumBufferSize * 2;
				recordingData.header[i].dwFlags = 0;
				recordingData.header[i].dwLoops = 0;

				MMRESULT res = waveInPrepareHeader(recordingData.queue, &recordingData.header[i], sizeof(WAVEHDR));
				if (res == MMSYSERR_NOERROR)
				{
					waveInAddBuffer(recordingData.queue, &recordingData.header[i], sizeof(WAVEHDR));
				}
			}
		}
	}

	virtual void setCallback(Callback callback) override {
		recordingData.callback = callback;
	}

	virtual void start() override {
		recordingData.currentBuffer = 0;
		waveInStart(recordingData.queue);
	}

	virtual void stop() override {
		waveInStop(recordingData.queue);
	}

	virtual ~WinAudioInputReader() override {
		if (_status == MMSYSERR_NOERROR)
		{
			// force stop thread recording loop
			PostThreadMessage(recordingData.waveInThreadId, MM_WIM_CLOSE, 0, 0);

			// wait for thread recording stop for 5 seconds
			WaitForSingleObject(recordingData.waveInThreadHandle, INFINITE);

			// stop recording and close device
			waveInStop(recordingData.queue);
			waveInReset(recordingData.queue);
			waveInClose(recordingData.queue);
			
			// clean up resources
			CloseHandle(recordingData.waveInThreadHandle);
			DeleteCriticalSection(&recordingData.criticalSection);
			
			delete[] recordingData._pBuffer;
		}
	}

    virtual int getSampleRate() const override {
		return kSamplingRate;
	}

	virtual int getMaximumBufferSize() const override {
		return recordingData.bufferSizeInBytes;
	}
};

AudioInputReader* CreateDefaultAudioInputReader(int maximumBufferSize) {
	return new WinAudioInputReader(maximumBufferSize);
}


