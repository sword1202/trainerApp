/*
 * libaudiodecoder - Native Portable Audio Decoder Library
 * libaudiodecoder API Header File
 * Latest version available at: http://www.oscillicious.com/libaudiodecoder
 *
 * Copyright (c) 2010-2012 Albert Santoni, Bill Good, RJ Ryan  
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire libaudiodecoder license; however, 
 * the Oscillicious community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also 
 * requested that these non-binding requests be included along with the 
 * license above.
 */

#include <string>
#include <iostream>
#include "audiodecodercoreaudio_mac.h"

static SInt64 GetSizeProc(void *inClientData) {
    const std::string& data = *static_cast<std::string*>(inClientData);
    return (SInt64)data.size();
}

static OSStatus ReadProc(void *inClientData, SInt64 inPosition, UInt32 requestCount, void *buffer, UInt32 *actualCount) {
    assert(inPosition >= 0);
    const std::string& data = *static_cast<std::string*>(inClientData);
    SInt64 end = inPosition + requestCount;
    if (end > data.size()) {
        end = (SInt64)data.size();
    }
    
    if (end <= inPosition) {
        *actualCount = 0;
    } else {
        size_t count = static_cast<size_t>(end - inPosition);
        memcpy(buffer, data.data() + inPosition, count);
        *actualCount = (UInt32)count;
    }
    
    return kAudioServicesNoError;
}

AudioDecoderCoreAudio::AudioDecoderCoreAudio() {

}

AudioDecoderCoreAudio::~AudioDecoderCoreAudio() {
	ExtAudioFileDispose(audioFile);
    AudioFileClose(audioFileID);
}

void AudioDecoderCoreAudio::open(std::string &&data) {

    audioData = std::move(data);
    //Open the audio file.
    OSStatus err;

    err = AudioFileOpenWithCallbacks(&audioData, ReadProc, 0, GetSizeProc, 0, 0, &audioFileID);
    if (err != noErr)
    {
        throw std::runtime_error("AudioDecoderCoreAudio: AudioFileOpenWithCallbacks error " + std::to_string(err));
    }

    err = ExtAudioFileWrapAudioFileID(audioFileID, false, &audioFile);
    if (err != noErr)
    {
        throw std::runtime_error("AudioDecoderCoreAudio: ExtAudioFileWrapAudioFileID error " + std::to_string(err));
    }

    /** TODO: Use FSRef for compatibility with 10.4 Tiger. 
        Note that ExtAudioFileOpen() is deprecated above Tiger, so we must maintain
        both code paths if someone finishes this part of the code.
    FSRef fsRef;
    CFURLGetFSRef(reinterpret_cast<CFURLRef>(url.get()), &fsRef);
    err = ExtAudioFileOpen(&fsRef, &m_audioFile);
    */

    // get the input file format
    CAStreamBasicDescription inputFormat;
    UInt32 size = sizeof(inputFormat);
    err = ExtAudioFileGetProperty(audioFile, kExtAudioFileProperty_FileDataFormat, &size, &inputFormat);
	if (err != noErr)
	{
        throw std::runtime_error("AudioDecoderCoreAudio: Error getting file format.");
	}    
    inputFormat = inputFormat;
    
	// create the output format
	CAStreamBasicDescription outputFormat;
    bzero(&outputFormat, sizeof(AudioStreamBasicDescription));
	outputFormat.mFormatID = kAudioFormatLinearPCM;
	outputFormat.mSampleRate = inputFormat.mSampleRate;
	outputFormat.mChannelsPerFrame = 2;
    outputFormat.mFormatFlags =  kAudioFormatFlagIsSignedInteger | kAudioFormatFlagsNativeEndian |
            kLinearPCMFormatFlagIsPacked;

    //Note iPhone/iOS only supports signed integers supposedly:
    //outputFormat.mFormatFlags = kAudioFormatFlagIsSignedInteger;
	
    //Debugging:
    //printf ("Source File format: "); inputFormat.Print();
    //printf ("Dest File format: "); outputFormat.Print();


	/*
	switch(inputFormat.mBitsPerChannel) {
		case 16:
			outputFormat.mFormatFlags =  kAppleLosslessFormatFlag_16BitSourceData;
			break;
		case 20:
			outputFormat.mFormatFlags =  kAppleLosslessFormatFlag_20BitSourceData;
			break;
		case 24:
			outputFormat.mFormatFlags =  kAppleLosslessFormatFlag_24BitSourceData;
			break;
		case 32:
			outputFormat.mFormatFlags =  kAppleLosslessFormatFlag_32BitSourceData;
			break;
	}*/

    // get and set the client format - it should be lpcm
    CAStreamBasicDescription clientFormat = outputFormat; //We're always telling the OS to do the conversion to floats for us now
	clientFormat.mChannelsPerFrame = 2;
	clientFormat.mBytesPerFrame = sizeof(SAMPLE)*clientFormat.mChannelsPerFrame;
	clientFormat.mBitsPerChannel = sizeof(SAMPLE)*8; //16 for signed int, 32 for float;
	clientFormat.mFramesPerPacket = 1;
	clientFormat.mBytesPerPacket = clientFormat.mBytesPerFrame*clientFormat.mFramesPerPacket;
	clientFormat.mReserved = 0;
	clientFormat = clientFormat;
    size = sizeof(clientFormat);
    
    err = ExtAudioFileSetProperty(audioFile, kExtAudioFileProperty_ClientDataFormat, size, &clientFormat);
	if (err != noErr)
	{
		//qDebug() << "SSCA: Error setting file property";
        throw std::runtime_error("AudioDecoderCoreAudio: Error setting file property " + std::to_string(err));
	}
	
	//Set m_iChannels and m_iNumSamples;
	m_iChannels = clientFormat.NumberChannels();

	//get the total length in frames of the audio file - copypasta: http://discussions.apple.com/thread.jspa?threadID=2364583&tstart=47
	UInt32		dataSize;
	SInt64		totalFrameCount;		
	dataSize	= sizeof(totalFrameCount); //XXX: This looks sketchy to me - Albert
	err			= ExtAudioFileGetProperty(audioFile, kExtAudioFileProperty_FileLengthFrames, &dataSize, &totalFrameCount);
	if (err != noErr)
	{
	    throw std::runtime_error("AudioDecoderCoreAudio: Error getting number of frames " + std::to_string(err));
	}

      //
      // WORKAROUND for bug in ExtFileAudio
      //
      
      AudioConverterRef acRef;
      UInt32 acrsize=sizeof(AudioConverterRef);
      err = ExtAudioFileGetProperty(audioFile, kExtAudioFileProperty_AudioConverter, &acrsize, &acRef);
    if (err != noErr)
    {
        throw std::runtime_error("AudioDecoderCoreAudio: ExtAudioFileGetProperty error " + std::to_string(err));
    }

      //_ThrowExceptionIfErr(@"kExtAudioFileProperty_AudioConverter", err);

      AudioConverterPrimeInfo primeInfo;
      UInt32 piSize=sizeof(AudioConverterPrimeInfo);
      memset(&primeInfo, 0, piSize);
      err = AudioConverterGetProperty(acRef, kAudioConverterPrimeInfo, &piSize, &primeInfo);
      if(err != kAudioConverterErr_PropertyNotSupported) // Only if decompressing
      {
         //_ThrowExceptionIfErr(@"kAudioConverterPrimeInfo", err);
         
         headerFrames=primeInfo.leadingFrames;
      }
	
	m_iNumSamples = (totalFrameCount/*-m_headerFrames*/)*m_iChannels;
	m_iSampleRate = inputFormat.mSampleRate;
	m_fDuration = m_iNumSamples / static_cast<float>(m_iSampleRate * m_iChannels);
	
    //Convert mono files into stereo
    if (inputFormat.NumberChannels() == 1)
    {
        SInt32 channelMap[2] = {0, 0}; // array size should match the number of output channels
        AudioConverterSetProperty(acRef, kAudioConverterChannelMap, 
                                    sizeof(channelMap), channelMap);
    }

	//Seek to position 0, which forces us to skip over all the header frames.
	//This makes sure we're ready to just let the Analyser rip and it'll
	//get the number of samples it expects (ie. no header frames).
	seek(0);
}

int AudioDecoderCoreAudio::seek(int sampleIdx) {
    OSStatus err = noErr;
    SInt64 segmentStart = sampleIdx / 2;

    err = ExtAudioFileSeek(audioFile, (SInt64)segmentStart+headerFrames);
    //_ThrowExceptionIfErr(@"ExtAudioFileSeek", err);
	//qDebug() << "SSCA: Seeking to" << segmentStart;

	//err = ExtAudioFileSeek(m_audioFile, sampleIdx / 2);		
	if (err != noErr)
	{
        std::cerr << "AudioDecoderCoreAudio: Error seeking to sample " << sampleIdx << std::endl;
	}

    m_iPositionInSamples = sampleIdx;

    return m_iPositionInSamples; //filepos;
}

int AudioDecoderCoreAudio::read(int size, const SAMPLE *destination) {
    OSStatus err;
    SAMPLE *destBuffer(const_cast<SAMPLE*>(destination));
    unsigned int samplesWritten = 0;
    unsigned int i = 0;
    UInt32 numFrames = 0;
    unsigned int totalFramesToRead = size/2;
    unsigned int numFramesRead = 0;
    unsigned int numFramesToRead = totalFramesToRead;

    while (numFramesRead < totalFramesToRead) { 
    	numFramesToRead = totalFramesToRead - numFramesRead;
    	
		AudioBufferList fillBufList;
		fillBufList.mNumberBuffers = 1; //Decode a single track
        //See CoreAudioTypes.h for definitins of these variables:
		fillBufList.mBuffers[0].mNumberChannels = clientFormat.NumberChannels();
		fillBufList.mBuffers[0].mDataByteSize = numFramesToRead*2 * sizeof(SAMPLE);
		fillBufList.mBuffers[0].mData = (void*)(&destBuffer[numFramesRead*2]);
			
        // client format is always linear PCM - so here we determine how many frames of lpcm
        // we can read/write given our buffer size
		numFrames = numFramesToRead; //This silly variable acts as both a parameter and return value.
		err = ExtAudioFileRead (audioFile, &numFrames, &fillBufList);
		//The actual number of frames read also comes back in numFrames.
		//(It's both a parameter to a function and a return value. wat apple?)
		//XThrowIfError (err, "ExtAudioFileRead");	
        /*
        if (err != noErr)
        {
            std::cerr << "Error reading samples from file" << std::endl;
            return 0;
        }*/

		if (!numFrames) {
				// this is our termination condition
			break;
		}
		numFramesRead += numFrames;
    }
    
    m_iPositionInSamples += numFramesRead*m_iChannels;

    return numFramesRead*m_iChannels;
}


// static
std::vector<std::string> AudioDecoderCoreAudio::supportedFileExtensions() {
    std::vector<std::string> list;
    list.push_back(std::string("m4a"));
    list.push_back(std::string("mp3"));
    list.push_back(std::string("mp2"));
    //Can add mp3, mp2, ac3, and others here if you want.
    //See:
    //  http://developer.apple.com/library/mac/documentation/MusicAudio/Reference/AudioFileConvertRef/Reference/reference.html#//apple_ref/doc/c_ref/AudioFileTypeID
    
    return list;
}


