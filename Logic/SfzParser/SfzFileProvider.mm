//
// Created by Semyon Tikhonenko on 11/29/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "SfzFileProvider.h"
#include "StringUtils.h"

using namespace CppUtils;

static SfzFile* create() {
    NSString *sfzPath = [[NSBundle bundleWithIdentifier:@"Logic"] pathForResource:@"piano" ofType:@"sfz"];
    auto sfzData = Strings::ReadFileIntoString([sfzPath cStringUsingEncoding:NSString.defaultCStringEncoding]);
    return new SfzFile(sfzData, [] (const std::string& samplePath) {
        auto fileName = Strings::RemoveLongestPrefixEndingWith(samplePath, "/");
        std::string name, extension;
        Strings::ExtractFilenameAndExtension(fileName, &name, &extension);
        NSString *path = [[NSBundle bundleWithIdentifier:@"Logic"]
                pathForResource:Strings::ToNSString(name)
                         ofType:Strings::ToNSString(extension)];
        return Strings::ReadBinaryFileContent([path cStringUsingEncoding:NSString.defaultCStringEncoding]);
    });
}

SfzFile *SfzFileProvider::get() {
    static SfzFile* sfz = create();
    return sfz;
}
