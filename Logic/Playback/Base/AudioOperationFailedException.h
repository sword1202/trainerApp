//
// Created by Semyon Tykhonenko on 2019-05-20.
//

#ifndef TEXTIMAGESGENERATOR_PREPAREFAILEDEXCEPTION_H
#define TEXTIMAGESGENERATOR_PREPAREFAILEDEXCEPTION_H

#include <stdexcept>
#include <string>

class AudioOperationFailedException : public std::runtime_error {
public:
    explicit AudioOperationFailedException(const std::string &string);
    AudioOperationFailedException();
};


#endif //TEXTIMAGESGENERATOR_PREPAREFAILEDEXCEPTION_H
