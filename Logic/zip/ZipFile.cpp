//
// Created by Semyon Tykhonenko on 1/21/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#include "ZipFile.h"

ZipFile::ZipFile(const char *fileName) {
    zip = zip_open(fileName, 0, 'r');
}

int ZipFile::getEntriesCount() const {
    return zip_total_entries(zip);
}

const char* ZipFile::getEntryName(int index) const {
    zip_entry_openbyindex(zip, index);
    auto result = zip_entry_name(zip);
    zip_entry_close(zip);
    return result;
}

std::string ZipFile::getEntryData(int index) const {
    zip_entry_openbyindex(zip, index);
    return getEntryData();
}

std::string ZipFile::getEntryData(const char *name) const {
    zip_entry_open(zip, name);
    return getEntryData();
}

std::string ZipFile::getEntryData() const {
    void* buff = nullptr;
    ssize_t size = zip_entry_read(zip, &buff, nullptr);
    if (size < 0 || !buff) {
        throw std::runtime_error("Failed to read entry");
    }

    char* chBuff = static_cast<char*>(buff);
    std::string result(chBuff, chBuff + size);
    free(buff);
    zip_entry_close(zip);
    return result;
}

ZipFile::~ZipFile() {
    zip_close(zip);
}
