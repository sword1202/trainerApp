//
// Created by Semyon Tykhonenko on 2019-05-20.
//

#include "MvxPlayerPrepareException.h"

MvxPlayerPrepareException::MvxPlayerPrepareException(Reason reason) : reason(reason) {

}

MvxPlayerPrepareException::Reason MvxPlayerPrepareException::getReason() const {
    return reason;
}

const char *MvxPlayerPrepareException::what() const noexcept {
    switch (reason) {
        case BROKEN_INSTRUMENTAL:
            return "Instrumental file is broken";
        case BROKEN_VOCAL_PART:
            return "Vocal part is broken";
        case BROKEN_RECORDING:
            return "Recording file is broken";
        case DIFFERENT_DURATIONS:
            return "Instrumental and vocal tracks have different durations";
        default:
            return "unknown error";
    }
}
