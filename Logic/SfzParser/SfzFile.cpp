//
// Created by Semyon Tykhonenko on 3/29/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "SfzFile.h"
#include "SfzSectionsParser.h"
#include "Algorithms.h"
#include "MathUtils.h"
#include "StringUtils.h"

using OptString = std::optional<std::string>;
using namespace CppUtils;

SfzFile::SfzFile(const std::string& data, const SampleReader& sampleReader) {
    SfzSectionsParser parser;
    std::vector<SfzSection> sections = parser.parse(data);

    bool isLowVelocity = false;

    for (auto& section : sections) {
        if (section.name == "region") {
            OptString sampleFilePath = section.attrs["sample"];
            OptString highKeyOpt = section.attrs["hikey"];
            OptString lowKeyOpt = section.attrs["lokey"];
            OptString keyCenterOpt = section.attrs["pitch_keycenter"];
            OptString keyOpt = section.attrs["key"];
            OptString loopStartOpt = section.attrs["loop_start"];
            OptString loopEndOpt = section.attrs["loop_end"];

            assert((keyCenterOpt || keyOpt) && "pitch_keycenter not present");
            assert(sampleFilePath && "sample not present");

            SfzRegion region;
            region.audioData = sampleReader(*sampleFilePath);

            auto key = *(keyCenterOpt ? keyCenterOpt : keyOpt);
            region.keyCenter = std::stoi(key);

            if (highKeyOpt) {
                region.highKey = std::stoi(*highKeyOpt);
            } else {
                region.highKey = region.keyCenter;
            }

            if (lowKeyOpt) {
                region.lowKey = std::stoi(*lowKeyOpt);
            } else {
                region.lowKey = region.keyCenter;
            }

            if (loopStartOpt) {
                assert(loopEndOpt && "loop_start is defined, so loop_end should be defined");
                region.loop = SfzLoop(std::stoi(*loopStartOpt), std::stoi(*loopEndOpt));
            }

            assert(region.lowKey <= region.highKey);

            region.isLowVelocity = isLowVelocity;
            regions.push_back(region);
        } else if (section.name == "group") {
        } else if (section.name == "global") {
            isLowVelocity = section.attrs.count("lovel") == 0;
        }
    }
}

const SfzRegion &SfzFile::findRegion(const Pitch &pitch) const {
    int index = pitch.getPerfectFrequencyIndex();
    const auto* result = CppUtils::Find(regions, [=] (const SfzRegion& region) -> bool {
        return region.keyCenter == index || Math::IsInClosedInterval(region.lowKey, region.highKey, index);
    });

    assert(result && "Failed to find a region");
    return *result;
}
