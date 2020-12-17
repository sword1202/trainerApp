#include "pitch_detection.h"
#include <algorithm>
#include <complex>
#include <numeric>
#include <vector>

template <typename T>
void
util::acorr_r(const std::vector<T> &audio_buffer, pitch_alloc::BaseAlloc<T> *ba)
{
	if (audio_buffer.size() == 0)
		throw std::invalid_argument("audio_buffer shouldn't be empty");

	std::transform(audio_buffer.begin(), audio_buffer.begin() + ba->N,
	    ba->out_im.begin(), [](T x) -> std::complex<T> {
		    return std::complex(x, static_cast<T>(0.0));
	    });

	ba->fft->execute(&ba->out_im, true);

	std::complex<float> scale = {
	    1.0f / (float)(ba->N * 2), static_cast<T>(0.0)};
	for (int i = 0; i < ba->N; ++i)
		ba->out_im[i] *= std::conj(ba->out_im[i]) * scale;

	ba->fft->execute(&ba->out_im, false);

	std::transform(ba->out_im.begin(), ba->out_im.begin() + ba->N,
	    ba->out_real.begin(),
	    [](std::complex<float> cplx) -> T { return std::real(cplx); });
}

template void
util::acorr_r<double>(const std::vector<double> &audio_buffer,
    pitch_alloc::BaseAlloc<double> *ba);

template void
util::acorr_r<float>(
    const std::vector<float> &audio_buffer, pitch_alloc::BaseAlloc<float> *ba);
