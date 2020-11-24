#include <iostream>
#include <Logic/MvxFile.h>
#include <Logic/StringUtils.h>
#include <Logic/MidiFileReader.h>
#include <Logic/Maps.h>
#include <map>
#include <Logic/MidiFileReaderException.h>
#include <Logic/MvxFile.h>
#include <Logic/VocalTrainerFilePlayer.h>
#include <Logic/VocalTrainerPlayerPrepareException.h>

using std::cout;
using std::endl;
using namespace CppUtils;

int main(int argc, char *argv[]) {
    const char * templateString = "The command arguments should be in the following format: "
                                  "mvx=outputFilePath "
                                  "vocal=vocalMidiPath "
                                  "midiTrackId=midiTrackId "
                                  "instrumental=instrumentalPath "
                                  "artistName=artistName "
                                  "title=title\n";
    if (argc == 0) {
        cout << templateString;
        return -1;
    }

    std::map<std::string, std::string> args;

    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];
        auto split = Strings::Split(arg, '=');
        if (split.size() != 2) {
            cout << "Unable to parse argument " << arg << endl;
            cout << templateString;
            return -1;
        }
        args[split[0]] = split[1];
    }

    auto outputPath = args["mvx"];
    if (outputPath.empty()) {
        cout << "mvx argument is empty\n";
        return -1;
    }

    auto vocalPath = args["vocal"];
    MidiFileReader reader;
    if (!reader.read(vocalPath)) {
        cout << "Broken or missing midi file, generating .mvx file failed\n";
        return -1;
    }
    double beatsPerMinute = reader.getBeatsPerMinute();

    VocalPart vocalPart;
    try {
        int midiIndex = Strings::ParseInt(args["midiTrackId"], 1);
        vocalPart = reader.tryGetVocalPartFromMidiTrackWithId(midiIndex);
    } catch (MidiFileReaderException& e) {
        cout << "Failed to parse midi file: " << e.what() << endl;
        return -1;
    }

    const auto& instrumental = args["instrumental"];
    if (instrumental.empty()) {
        cout << "instrumental is missing\n";
        cout << templateString << endl;
        return -1;
    }

    const auto& artistName =
            Strings::Unquote(
                Maps::GetOrDefault(args, "artistName", "Unknown artist")
            );
    const auto& title =
            Strings::Unquote(
                Maps::GetOrDefault(args, "title", "Untitled")
            );

    MvxFile mvxFile;
    mvxFile.setVocalPart(vocalPart);
    mvxFile.setArtistNameUtf8(artistName);
    mvxFile.setSongTitleUtf8(title);
    mvxFile.loadInstrumentalFromFile(instrumental.data());
    mvxFile.setBeatsPerMinute(beatsPerMinute);
    mvxFile.generateInstrumentalPreviewSamplesFromInstrumental();

    PortAudio::init();
    VocalTrainerFilePlayer player;
    player.setSource(
            &mvxFile,
            false //destroyFileOnDestructor
    );

    try {
        player.prepare();
    } catch (VocalTrainerPlayerPrepareException& e) {
        if (e.getReason() == VocalTrainerPlayerPrepareException::DIFFERENT_DURATIONS) {
            double vocalPartDuration = vocalPart.getDurationInSeconds();
            double instrumentalDuration = player.getInstrumentalPlayer().getTrackDurationInSeconds();
            vocalPart = vocalPart.cutOrExpand(0, instrumentalDuration);
            cout << "Vocal part has duration "
                << vocalPartDuration
                <<". Instrumental has duration "
                << instrumentalDuration
                << ". Vocal part has been cut/modified to match instrumental duration\n";
        } else {
            cout << e.what() << endl;
            return -1;
        }
    }

    mvxFile.writeToFile(outputPath.data());

    cout << "mvx file has been created: " << outputPath << endl;

    return 0;
}
