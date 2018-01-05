#define TSF_IMPLEMENTATION
#include "tsf.h"
#import "WAVFile.h"
#import <AVFoundation/AVAudioPlayer.h>

AVAudioPlayer* testTsf() {
    NSBundle *myBundle = [NSBundle mainBundle];
    NSString *absPath= [myBundle pathForResource:@"a" ofType:@"sf2"];
    tsf* TinySoundFont = tsf_load_filename(absPath.cString);
    tsf_set_output(TinySoundFont, TSF_MONO, 44100, 0); //sample rate
    tsf_note_on(TinySoundFont, 0, 60, 1.0f); //preset 0, middle C
    short HalfSecond[22050]; //synthesize 0.5 seconds
    tsf_render_short(TinySoundFont, HalfSecond, 22050, 0);
    std::vector<char> wavData = WAVFile::addWavHeaderToRawPcmData((char*)HalfSecond, sizeof(HalfSecond));
    NSError *error;
    AVAudioPlayer *player = [[AVAudioPlayer alloc] initWithData:[NSData dataWithBytes:wavData.data()
                                                                               length:wavData.size()]
                                                          error:&error];
    [player play];
    return player;
}