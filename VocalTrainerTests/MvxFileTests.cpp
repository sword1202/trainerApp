//
// Created by Semyon Tikhonenko on 2018-12-31.
//

#include "catch.hpp"
#include "MvxFile.h"
#include "Algorithms.h"

TEST_CASE("generatePreviewSamplesFromRawPcm test") {
    std::vector<int16_t> rawPcm;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < (i == 9 ? 5 : 10); ++j) {
            rawPcm.push_back(i);
        }
    }

    int batchSize = 10;
    std::vector<int16_t> result = MvxFile::generatePreviewSamplesFromRawPcm(rawPcm.data(), rawPcm.size(), batchSize);
    REQUIRE(result == std::vector<int16_t> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
}

TEST_CASE("generatePreviewSamplesFromRawPcm test 2") {
    std::vector<int16_t> rawPcm;

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < (i == 9 ? 5 : 10); ++j) {
            rawPcm.push_back(rand() % std::numeric_limits<unsigned short>::max());
        }
    }

    int batchSize = 10;
    std::vector<int16_t> result = MvxFile::generatePreviewSamplesFromRawPcm(rawPcm.data(), rawPcm.size(), batchSize);

    std::vector<int16_t> expected;

    const short* begin = rawPcm.data();
    for (int i = 0; i < 95; i += 10) {
        expected.push_back(CppUtils::Average<short>(begin + i, i < 90 ? 10 : 5));
    }
}