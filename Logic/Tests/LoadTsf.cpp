//
// Created by Semyon Tikhonenko on 2018-12-31.
//

#define TSF_IMPLEMENTATION
#include "LoadTsf.h"
#include "StringUtils.h"

using namespace CppUtils;

tsf* LoadTsf() {
    std::string data = Strings::ReadBinaryFileContent("sounds.sf2");
    return tsf_load_memory(data.data(), data.size());
}
