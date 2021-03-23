#import "ApplicationModel.h"
#include "StringUtils.h"

using namespace CppUtils;

std::string ApplicationModel::getSfzFilePath() const {
    NSString *sfzPath = [[NSBundle bundleWithIdentifier:@"Logic"] pathForResource:@"piano" ofType:@"sfz"];
    return std::string(sfzPath.UTF8String);
}

std::string ApplicationModel::getMetronomeDataFilePath() const {
    NSString *result = [NSBundle.mainBundle pathForResource:@"metronome" ofType:@"wav"];
    return std::string(result.UTF8String);
}