//
// Created by Semyon Tikhonenko on 9/17/21.
// Copyright (c) 2021 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_AUDIODATABUFFERSERIALIZATION_H
#define VOCALTRAINER_AUDIODATABUFFERSERIALIZATION_H

#include "StlContainerAudioDataBuffer.h"

namespace CppUtils {
    namespace Serialization {

        namespace {
            template<typename Buffer, typename Archive>
            void AudioDataBufferSaveOrLoad(Buffer &buffer, Archive &archive, bool save) {
                std::string str;
                if (save) {
                    if (buffer) {
                        str = buffer->toBinaryString();
                    }
                }
                archive(str);
                if (!save) {
                    if (!str.empty()) {
                        buffer.reset(new StdStringAudioDataBuffer(std::move(str)));
                    } else {
                        buffer = nullptr;
                    }
                }
            }
        }

        template<typename Archive>
        void SaveOrLoad(AudioDataBufferConstPtr &buffer, Archive &archive, bool save) {
            AudioDataBufferSaveOrLoad(buffer, archive, save);
        }

        template<typename Archive>
        void SaveOrLoad(AudioDataBufferPtr &buffer, Archive &archive, bool save) {
            AudioDataBufferSaveOrLoad(buffer, archive, save);
        }
    }
}


#endif //VOCALTRAINER_AUDIODATABUFFERSERIALIZATION_H
