#include "catch.hpp"
#include "Lyrics.h"

const char* russianLyrics = "{V1}\n"
                            "Пр[15.12,15.23]и[15.23,16.43]вет[16.43, 16.98] как ты[18.23, 20.12]\n\n"
                            "{B1}\n"
                            "Пр[20.12,20.23]и[20.23,20.43]вет[20.43, 20.98]\n\n"
                            "{C1}\n"
                            "Пр[21.12,21.23]и[21.23,21.43]вет[21.43, 21.98]\n";

TEST_CASE("Russian lyrics test") {
    Lyrics lyrics(std::string(russianLyrics));
}