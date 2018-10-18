//
// Created by Semyon Tikhonenko on 8/28/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#include <iostream>
#include <windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <shlwapi.h>
#include <propvarutil.h>
#include <assert.h>

#include "audiodecodermediafoundation_win.h"

const int kBitsPerSample = 16;
const int kNumChannels = 2;
const int kSampleRate = 44100;
const int kLeftoverSize = 4096;

AudioDecoderMediaFoundation::AudioDecoderMediaFoundation()
{
    // Defaults
    m_iChannels = kNumChannels;
    m_iSampleRate = kSampleRate;
    m_BitsPerSample = kBitsPerSample;
}

AudioDecoderMediaFoundation::~AudioDecoderMediaFoundation()
{
    if (reader != nullptr)
        delete [] m_leftoverBuffer;

    if (reader != nullptr)
        reader->Release();
    if (audioType != nullptr)
        audioType->Release();
    if (dataStream != nullptr)
        dataStream->Release();
    if (byteStream != nullptr)
        byteStream->Release();

    MFShutdown();
    CoUninitialize();
}

void AudioDecoderMediaFoundation::open(std::string &&data)
{
    // Initialize the COM library.
    if (CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) < 0) {
       throw std::runtime_error(" Failed to initialize COM");
    }

    // Initialize the Media Foundation platform.
    if (MFStartup(MF_VERSION) < 0) {
        throw std::runtime_error("Failed to initialize Media Foundation");
    }

    assert(dataStream == nullptr);

    dataStream = SHCreateMemStream(reinterpret_cast<const BYTE*>(data.c_str()), data.size());

    if (MFCreateMFByteStreamOnStream(dataStream, &byteStream) < 0) {
        throw std::runtime_error("Failed to create stream from array");
    }

    if (MFCreateSourceReaderFromByteStream(byteStream, nullptr, &reader) < 0) {
        throw std::runtime_error("Failed to create source reader from stream");
    }

    // Read properties
    // Use only first stream
    if (reader->SetStreamSelection(MF_SOURCE_READER_ALL_STREAMS, false) < 0) {
        throw std::runtime_error("Failed to deselect all streams");
    }

    if (reader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, true) < 0) {
        throw std::runtime_error("Failed to select first audio stream");
    }

    if (reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &audioType) < 0) {
        throw std::runtime_error("Failed to retrieve completed media type");
    }

    UINT32 allSamplesIndependent = 0;
    UINT32 fixedSizeSamples	= 0;
    UINT32 sampleSize = 0;
    UINT32 bitsPerSample = 0;
    UINT32 blockAlignment = 0;
    UINT32 numChannels = 0;
    UINT32 samplesPerSecond	= 0;
    audioType->GetUINT32(MF_MT_ALL_SAMPLES_INDEPENDENT, &allSamplesIndependent);
    audioType->GetUINT32(MF_MT_FIXED_SIZE_SAMPLES, &fixedSizeSamples);
    audioType->GetUINT32(MF_MT_SAMPLE_SIZE, &sampleSize);
    audioType->GetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, &bitsPerSample);
    audioType->GetUINT32(MF_MT_AUDIO_BLOCK_ALIGNMENT, &blockAlignment);
    audioType->GetUINT32(MF_MT_AUDIO_NUM_CHANNELS, &numChannels);
    audioType->GetUINT32(MF_MT_AUDIO_SAMPLES_PER_SECOND, &samplesPerSecond);

    std::cout << "Bits per sample: " << bitsPerSample << std::endl;
    std::cout << "All samples independent: " << allSamplesIndependent << std::endl;
    std::cout << "Fixed size samples: " << fixedSizeSamples << std::endl;
    std::cout << "Sample size: " << sampleSize << std::endl;
    std::cout << "Bits per sample: " << bitsPerSample << std::endl;
    std::cout << "Block alignment: " << blockAlignment << std::endl;
    std::cout << "Channels: " << numChannels << std::endl;
    std::cout << "Samples per second: " << samplesPerSecond << std::endl;

    m_iChannels = static_cast<int>(numChannels);
    m_iSampleRate = static_cast<int>(samplesPerSecond);
    m_BitsPerSample = bitsPerSample;

    // For compressed files, the bits per sample is undefined, so by convention we're going to get 16-bit integers out.
    if (m_BitsPerSample == 0)
        m_BitsPerSample = kBitsPerSample;

    if (m_BitsPerSample != sizeof(SAMPLE) * 8)
         throw std::runtime_error("Unsupported audio format");

    if (MFCreateMediaType(&audioType) < 0) {
        throw std::runtime_error("Failed to create media type");
    }

    if (audioType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio) < 0) {
        throw std::runtime_error("Failed to set major type");
    }

    if (audioType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM) < 0) {
        throw std::runtime_error("Failed to set subtype");
    }

    // Set this type on the source reader. The source reader will load the necessary decoder.
    if (reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, audioType) < 0) {
        throw std::runtime_error("Failed to set media type");
    }

    // Get the complete uncompressed format.
    audioType->Release();
    if (reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &audioType) < 0) {
        throw std::runtime_error("Failed to retrieve completed media type");
    }

    // Ensure the stream is selected.
    if (reader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, true) < 0) {
        throw std::runtime_error("Failed to select first audio stream (again)");
    }

    UINT32 leftoverBufferSize = 0;
    if (audioType->GetUINT32(MF_MT_SAMPLE_SIZE, &leftoverBufferSize) < 0) {
        leftoverBufferSize = 32;
        std::cerr << "Failed to get buffer size" << std::endl;
    }
    m_leftoverBufferSize = static_cast<size_t>(leftoverBufferSize);
    m_leftoverBufferSize /= 2; // Convert size in bytes to size in int16s
    m_leftoverBuffer = new short[m_leftoverBufferSize];

    // Get the duration, provided as a 64-bit integer of 100-nanosecond units
    PROPVARIANT prop;
    if (reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &prop) < 0) {
        throw std::runtime_error("Error getting duration");
    }
    m_fDuration = static_cast<double>(prop.hVal.QuadPart) / 1e7; // Convert a 100ns Media Foundation value to a number of seconds.
    mfDuration = prop.hVal.QuadPart;
    std::cout << "Duration: " << m_fDuration << std::endl;
    PropVariantClear(&prop);

    seek(0);
}

void AudioDecoderMediaFoundation::seek(int sampleIdx)
{
    if (!decoding)
        return;

    PROPVARIANT prop;
    long long seekTarget = sampleIdx / m_iChannels;
    long long mfSeekTarget = mfFromFrame(seekTarget) - 1;
    long result = m_iPositionInSamples;

    InitPropVariantFromInt64(mfSeekTarget < 0 ? 0 : mfSeekTarget, &prop);

    if (reader->Flush(MF_SOURCE_READER_FIRST_AUDIO_STREAM) < 0) {
        std::cerr << "Failed to flush before seek" << std::endl;
    }

    if (reader->SetCurrentPosition(GUID_NULL, prop) < 0)
        std::cerr << "Failed to seek" << std::endl;
    else
        result = sampleIdx;

    PropVariantClear(&prop);

    nextFrame = seekTarget;
    seeking = true;
    m_iPositionInSamples = result;
}

int AudioDecoderMediaFoundation::read(int samplesCount, SAMPLE *destination)
{
    assert(samples < sizeof(destBufferShort) / sizeof(SAMPLE));

    short *destBuffer = destBufferShort;
    size_t framesRequested = samplesCount / m_iChannels;
    size_t framesNeeded = framesRequested;

    // Copy frames from leftover buffer if the leftover buffer is at the correct frame
    if (m_leftoverBufferLength > 0 && m_leftoverBufferPosition == nextFrame) {
        copyFrames(destBuffer, &framesNeeded, m_leftoverBuffer, m_leftoverBufferLength);
        if (m_leftoverBufferLength > 0) {
            if (framesNeeded != 0) {
                std::cerr << "WARNING: Expected frames needed to be 0. Abandoning this file." << std::endl;
                decoding = false;
            }
            m_leftoverBufferPosition += framesRequested;
        }
    } else {
        //  Leftover buffer already empty or in the wrong position, clear it
        m_leftoverBufferLength = 0;
    }

    while (decoding && framesNeeded > 0) {
        DWORD dwFlags = 0;
        long long timestamp = 0;
        IMFSample *pSample;
        bool error = false; // Set to true to break after releasing

        if (reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwFlags, &timestamp, &pSample) < 0) {
            std::cout << "ReadSample failed." << std::endl;
            break;
        }

        // Check for errors
        if (dwFlags & MF_SOURCE_READERF_ERROR) {
            std::cerr << "ReadSample set ERROR, SourceReader is now dead" << std::endl;
            decoding = false;
            break;
        } else if (dwFlags & MF_SOURCE_READERF_ENDOFSTREAM) {
            std::cout << "End of input file." << std::endl;
            break;
        } else if (dwFlags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED) {
            std::cerr << "Type change" << std::endl;
            break;
        } else if (pSample == nullptr) {
            std::cerr << "No sample" << std::endl;
            continue;
        }

        IMFMediaBuffer *pMBuffer;
        if (pSample->ConvertToContiguousBuffer(&pMBuffer) < 0) {
            error = true;
            pSample->Release();
            break;
        }

        short *buffer;
        size_t bufferLength = 0;
        if (pMBuffer->Lock(reinterpret_cast<BYTE**>(&buffer), nullptr, reinterpret_cast<DWORD*>(&bufferLength)) < 0) {
            pMBuffer->Release();
            pSample->Release();
            break;
        }

        bufferLength /= (m_BitsPerSample / 8 * m_iChannels); // now in frames

        if (seeking) {
            long long bufferPosition = frameFromMF(timestamp);
            if (nextFrame < bufferPosition) {
                // We are farther forward than our seek target. Emit silence.
                short* pBufferCurpos = destBuffer + samples - framesNeeded * m_iChannels;
                long long offshootFrames = bufferPosition - nextFrame;

                // If we can correct this immediately, write zeros and adjust m_nextFrame to pretend it never happened.
                if (offshootFrames <= framesNeeded) {
                    std::cerr << "Working around inaccurate seeking. Writing silence for " << offshootFrames << " frames" << std::endl;
                    // Set offshootFrames * m_iChannels samples to zero.
                    memset(pBufferCurpos, 0, sizeof(*pBufferCurpos) * offshootFrames * m_iChannels);
                    // Now m_nextFrame == bufferPosition
                    nextFrame += offshootFrames;
                    framesNeeded -= offshootFrames;
                } else {
                    // The buffer we have just decoded is more than framesNeeded frames away from us.
                    seeking = false;
                    nextFrame = bufferPosition;
                    std::cerr << "Buffer error" << std::endl;
                }
            }

            if (nextFrame >= bufferPosition && nextFrame < bufferPosition + bufferLength) {
                // m_nextFrame is in this buffer.
                buffer += (nextFrame - bufferPosition) * m_iChannels;
                bufferLength -= nextFrame - bufferPosition;
                seeking = false;
            } else {
                // We need to keep going forward
                pMBuffer->Unlock();
                pMBuffer->Release();
                pSample->Release();
            }
        }

        // If the bufferLength is larger than the leftover buffer, re-allocate it with 2x the space
        if (bufferLength * m_iChannels > m_leftoverBufferSize) {
            size_t newSize = m_leftoverBufferSize;

            while (newSize < bufferLength * m_iChannels) {
                newSize *= 2;
            }
            short* newBuffer = new short[newSize];
            memcpy(newBuffer, m_leftoverBuffer, sizeof(m_leftoverBuffer[0]) * m_leftoverBufferSize);
            delete [] m_leftoverBuffer;
            m_leftoverBuffer = newBuffer;
            m_leftoverBufferSize = newSize;
        }
        copyFrames(destBuffer + (samples - framesNeeded * m_iChannels), &framesNeeded, buffer, bufferLength);

        pMBuffer->Unlock();
        pMBuffer->Release();
        pSample->Release();
    }

    nextFrame += framesRequested - framesNeeded;
    if (m_leftoverBufferLength > 0) {
        if (framesNeeded != 0) {
            std::cerr << "WARNING: Expected frames needed to be 0. Abandoning this file." << std::endl;
            decoding = false;
        }
        m_leftoverBufferPosition = nextFrame;
    }
    int samples_read = samples - framesNeeded * m_iChannels;
    m_iPositionInSamples += samples_read;

    // Convert to float samples
    for (int i = 0; i < samples_read; i++)
    {
        destination[i] = destBuffer[i];
    }
    return samples_read;
}

std::vector<std::string> AudioDecoderMediaFoundation::supportedFileExtensions()
{
    std::vector<std::string> list;
    list.push_back("m4a");
    list.push_back("mp4");
    list.push_back("wma");
    list.push_back("mp3");
    list.push_back("wav");
    list.push_back("aif");
    list.push_back("aiff");
    return list;
}

/**
 * Copies min(destFrames, srcFrames) frames to dest from src. Anything leftover
 * is moved to the beginning of m_leftoverBuffer, so empty it first (possibly
 * with this method). If src and dest overlap, I'll hurt you.
 */
void AudioDecoderMediaFoundation::copyFrames(short *dest, size_t *destFrames, const short *src, size_t srcFrames)
{
    if (srcFrames > *destFrames) {
        int samplesToCopy = *destFrames * m_iChannels;
        memcpy(dest, src, samplesToCopy * sizeof(*src));
        srcFrames -= *destFrames;
        memmove(m_leftoverBuffer, src + samplesToCopy, srcFrames * m_iChannels * sizeof(*src));
        *destFrames = 0;
        m_leftoverBufferLength = srcFrames;
    } else {
        int samplesToCopy(srcFrames * m_iChannels);
        memcpy(dest, src, samplesToCopy * sizeof(*src));
        *destFrames -= srcFrames;
        if (src == m_leftoverBuffer)
            m_leftoverBufferLength = 0;
    }
}

/**
 * Convert a 100ns Media Foundation value to a frame offset.
 */
inline long long AudioDecoderMediaFoundation::frameFromMF(long long mf)
{
    return static_cast<double>(mf) * m_iSampleRate / 1e7;
}

/**
 * Convert a frame offset to a 100ns Media Foundation value.
 */
inline long long AudioDecoderMediaFoundation::mfFromFrame(long long frame)
{
    return static_cast<double>(frame) / m_iSampleRate * 1e7;
}
