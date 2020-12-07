//
// Created by Semyon Tykhonenko on 2019-05-20.
//

#include "AudioOperationFailedException.h"

AudioOperationFailedException::AudioOperationFailedException(const std::string &string) : runtime_error(string) {}

AudioOperationFailedException::AudioOperationFailedException() : runtime_error("AudioPlayer prepare failed") {

}
