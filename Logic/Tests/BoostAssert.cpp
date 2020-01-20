#include <sstream>

namespace boost {
    void assertion_failed(char const * expr, char const * function, char const * file, long line) {
        std::stringstream str;
        str<<expr<<" "<<function<<" in file "<<file<<" line: "<<line;
        throw std::runtime_error(str.str());
    }
}