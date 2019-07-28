#define TSF_IMPLEMENTATION
#include "LoadTsf.h"
#import <Foundation/Foundation.h>

tsf* LoadTsf() {
    NSString *path = [[NSBundle bundleWithIdentifier:@"Logic"] pathForResource:@"sounds" ofType:@"sf2"];
    return tsf_load_filename(path.cString);
}