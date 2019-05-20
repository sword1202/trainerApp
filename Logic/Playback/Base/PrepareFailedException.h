//
// Created by Semyon Tykhonenko on 2019-05-20.
//

#ifndef TEXTIMAGESGENERATOR_PREPAREFAILEDEXCEPTION_H
#define TEXTIMAGESGENERATOR_PREPAREFAILEDEXCEPTION_H

#include <stdexcept>

class PrepareFailedException : public std::runtime_error {
public:
    explicit PrepareFailedException(const std::string &string);
    PrepareFailedException();
};


#endif //TEXTIMAGESGENERATOR_PREPAREFAILEDEXCEPTION_H
