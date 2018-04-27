
#import <portaudio/portaudio.h>
#include "VxFile.h"

struct VxFilePlayBackData {
    VxFile* vxFile;
    int seekByteIndex;
};

static void VxFilePlayBackCallback(
        const void *inputBuffer,
        void *outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void *userData) {
    VxFilePlayBackData* data = static_cast<VxFilePlayBackData *>(userData);
}