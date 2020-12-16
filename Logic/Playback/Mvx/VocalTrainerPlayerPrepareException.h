//
// Created by Semyon Tykhonenko on 2019-05-20.
//

#ifndef TEXTIMAGESGENERATOR_MVXPLAYERPREPAREEXCEPTION_H
#define TEXTIMAGESGENERATOR_MVXPLAYERPREPAREEXCEPTION_H

#include <exception>
#include <string>

class VocalTrainerPlayerPrepareException : public std::exception {
public:
    enum Reason {
        BROKEN_INSTRUMENTAL,
        BROKEN_VOCAL_PART,
        BROKEN_RECORDING,
        DIFFERENT_DURATIONS
    };
private:
    Reason reason;
    std::string description;
public:
    explicit VocalTrainerPlayerPrepareException(Reason reason, const std::string& description = "");
    Reason getReason() const;

    const char *what() const noexcept override;
};


#endif //TEXTIMAGESGENERATOR_MVXPLAYERPREPAREEXCEPTION_H
