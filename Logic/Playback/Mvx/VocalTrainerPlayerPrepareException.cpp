//
// Created by Semyon Tykhonenko on 2019-05-20.
//

#include "VocalTrainerPlayerPrepareException.h"

VocalTrainerPlayerPrepareException::VocalTrainerPlayerPrepareException(Reason reason) : reason(reason) {

}

VocalTrainerPlayerPrepareException::Reason VocalTrainerPlayerPrepareException::getReason() const {
    return reason;
}

const char *VocalTrainerPlayerPrepareException::what() const noexcept {
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
