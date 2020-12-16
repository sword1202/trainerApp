//
// Created by Semyon Tykhonenko on 2019-05-20.
//

#include "VocalTrainerPlayerPrepareException.h"

VocalTrainerPlayerPrepareException::VocalTrainerPlayerPrepareException(Reason reason, const std::string& description)
: reason(reason), description(description) {
    this->description = description.empty() ? "" : ": " + description;
    const char* message;
    switch (reason) {
        case BROKEN_INSTRUMENTAL:
            message = "Instrumental file is broken";
            break;
        case BROKEN_VOCAL_PART:
            message = "Vocal part is broken";
            break;
        case BROKEN_RECORDING:
            message = "Recording file is broken";
            break;
        case DIFFERENT_DURATIONS:
            message = "Instrumental and vocal tracks have different durations";
            break;
        default:
            message = "unknown error";
    }
    this->description = message + this->description;
}

VocalTrainerPlayerPrepareException::Reason VocalTrainerPlayerPrepareException::getReason() const {
    return reason;
}

const char *VocalTrainerPlayerPrepareException::what() const noexcept {
    return description.data();
}
