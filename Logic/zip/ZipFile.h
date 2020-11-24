//
// Created by Semyon Tykhonenko on 1/21/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#ifndef VOCALTRAINER_ZIPFILE_H
#define VOCALTRAINER_ZIPFILE_H

#include "zip.h"
#include <string>

// Only single level archives are supported
class ZipFile {
    zip_t* zip;
    std::string getEntryData() const;
public:
    ZipFile(const char* fileName);
    ~ZipFile();
    int getEntriesCount() const;
    const char* getEntryName(int index) const;
    std::string getEntryData(int index) const;
    std::string getEntryData(const char* name) const;
};


#endif //VOCALTRAINER_ZIPFILE_H
