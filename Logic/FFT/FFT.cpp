#include "FFT.h"
#include "AccelerateFFT.h"

FFT* FFT::create() {
#ifdef __APPLE__
    return new AccelerateFFT();
#else
#error Define FFT
#endif
}