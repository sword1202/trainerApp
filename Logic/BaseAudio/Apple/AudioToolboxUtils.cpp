//
// Created by Semyon Tikhonenko on 12/6/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "AudioToolboxUtils.h"
#include "AudioOperationFailedException.h"

void AudioToolboxUtils::throwExceptionIfError(OSStatus status) {
    if (status == kAudioServicesNoError) {
        return;
    }

    std::string error;
    switch (status) {
        case kAudioQueueErr_InvalidBuffer:
            error = "InvalidBuffer";
            break;
        case kAudioQueueErr_BufferEmpty:
            error = "BufferEmpty";
            break;
        case kAudioQueueErr_DisposalPending:
            error = "DisposalPending";
            break;
        case kAudioQueueErr_InvalidProperty:
            error = "InvalidProperty";
            break;
        case kAudioQueueErr_InvalidPropertySize:
            error = "InvalidPropertySize";
            break;
        case kAudioQueueErr_InvalidParameter:
            error = "InvalidParameter";
            break;
        case kAudioQueueErr_CannotStart:
            error = "CannotStart";
        case kAudioQueueErr_InvalidDevice:
            error = "InvalidDevice";
            break;
        case kAudioQueueErr_BufferInQueue:
            error = "BufferInQueue";
            break;
        case kAudioQueueErr_InvalidRunState:
            error = "InvalidRunState";
            break;
        case kAudioQueueErr_InvalidQueueType:
            error = "InvalidQueueType";
            break;
        case kAudioQueueErr_Permissions:
            error = "Permissions";
            break;
        case kAudioQueueErr_InvalidPropertyValue:
            error = "InvalidPropertyValue";
            break;
        case kAudioQueueErr_PrimeTimedOut:
            error = "PrimeTimedOut";
            break;
        case kAudioQueueErr_CodecNotFound:
            error = "CodecNotFound";
            break;
        case kAudioQueueErr_InvalidCodecAccess:
            error = "InvalidCodecAccess";
            break;
        case kAudioQueueErr_QueueInvalidated:
            error = "QueueInvalidated";
            break;
        case kAudioQueueErr_TooManyTaps:
            error = "TooManyTaps";
            break;
        case kAudioQueueErr_InvalidTapContext:
            error = "InvalidTapContext";
            break;
        case kAudioQueueErr_RecordUnderrun:
            error = "RecordUnderrun";
            break;
        case kAudioQueueErr_InvalidTapType:
            error = "InvalidTapType";
            break;
        case kAudioQueueErr_BufferEnqueuedTwice:
            error = "BufferEnqueuedTwice";
            break;
        case kAudioQueueErr_CannotStartYet:
            error = "CannotStartYet";
            break;
        case kAudioQueueErr_EnqueueDuringReset:
            error = "EnqueueDuringReset";
            break;
        case kAudioQueueErr_InvalidOfflineMode:
            error = "InvalidOfflineMode";
            break;
        default:
            error = "UknownError";
    }
    error += "(" + std::to_string(status) + ")";
    
    throw AudioOperationFailedException("Prepare failed with audioToolbox error: " + error);
}

void AudioToolboxUtils::createFormat(const AudioStreamDescription &description, AudioStreamBasicDescription *out) {
    *out = { 0 };
    out->mChannelsPerFrame = static_cast<UInt32>(description.numberOfChannels);
    out->mBitsPerChannel = static_cast<UInt32>(description.bitsPerChannel);
    out->mBytesPerFrame = static_cast<UInt32>(description.getSampleBytesCount());
    out->mSampleRate = description.sampleRate;
    out->mFormatID = kAudioFormatLinearPCM;
    out->mFramesPerPacket = 1;
    out->mBytesPerPacket = out->mFramesPerPacket * out->mBytesPerFrame;
    out->mReserved = 0;
    out->mFormatFlags =  kAudioFormatFlagIsSignedInteger |
            kAudioFormatFlagsNativeEndian |
            kLinearPCMFormatFlagIsPacked;
}
