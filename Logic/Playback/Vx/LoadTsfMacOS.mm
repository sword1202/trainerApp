#define TSF_IMPLEMENTATION
#include "LoadTsf.h"
#import <Foundation/Foundation.h>

tsf* LoadTsf() {
    NSString *path = [[NSBundle mainBundle] pathForResource:@"sounds" ofType:@"sf2"];
    return tsf_load_filename(path.cString);
}