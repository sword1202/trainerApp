#ifndef CPP_UTILS_PRIMITIVES_H
#define CPP_UTILS_PRIMITIVES_H

#import <limits>
#import <cmath>

namespace CppUtils {
namespace Primitives {
    template<typename Float>
    bool CompareFloats(Float a, Float b) {
        Float epsilon = std::numeric_limits<Float>::epsilon();
        return fabs(a - b) < epsilon;
    }
}
}

#endif