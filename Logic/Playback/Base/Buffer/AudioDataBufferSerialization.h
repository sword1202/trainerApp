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
                if (save) {
                    if (buffer) {
                        if (const char* data = buffer->provideBinaryDataBuffer()) {
                            int64_t s = buffer->getNumberOfBytes();
                            archive.asRaw(s);
                            archive.asBytes(const_cast<char*>(data), s);
                        } else {
                            std::string str = buffer->toBinaryString();
                            archive(str);
                        }
                    } else {
                        int64_t s = 0;
                        archive.asRaw(s);
                    }
                } else {
                    std::string str;
                    archive(str);
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
