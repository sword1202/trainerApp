//
// Created by Semyon Tykhonenko on 2019-05-20.
//

#include "PrepareFailedException.h"

PrepareFailedException::PrepareFailedException(const std::string &string) : runtime_error(string) {}

PrepareFailedException::PrepareFailedException() : runtime_error("AudioPlayer prepare failed") {

}
