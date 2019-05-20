//
// Created by Semyon Tykhonenko on 2019-05-20.
//

#ifndef TEXTIMAGESGENERATOR_MVXPLAYERPREPAREEXCEPTION_H
#define TEXTIMAGESGENERATOR_MVXPLAYERPREPAREEXCEPTION_H

#include <exception>

class MvxPlayerPrepareException : public std::exception {
public:
    enum Reason {
        BROKEN_INSTRUMENTAL,
        BROKEN_VOCAL_PART,
        BROKEN_RECORDING,
        DIFFERENT_DURATIONS
    };
private:
    Reason reason;
public:
    explicit MvxPlayerPrepareException(Reason reason);
    Reason getReason() const;

    const char *what() const noexcept override;
};


#endif //TEXTIMAGESGENERATOR_MVXPLAYERPREPAREEXCEPTION_H
