#include <iostream>
#include <Logic/MvxFile.h>
#include <Logic/StringUtils.h>
#include <Logic/MidiFileReader.h>
#include <Logic/Maps.h>
#include <map>
#include <set>
#include <Logic/MidiFileReaderException.h>
#include <Logic/MvxFile.h>
#include <Logic/VocalTrainerFilePlayer.h>
#include <Logic/VocalTrainerPlayerPrepareException.h>
#include <Logic/ApplicationModel.h>

using std::cout;
using std::cerr;
using std::endl;
using namespace CppUtils;

int main(int argc, char *argv[]) {
    const char * templateString = "The command arguments should be in the following format: "
                                  "-o example.mvx "
                                  "-midi midiExample.mid "
                                  "-trackid midiTrackId(default 1) "
                                  "-instrumental instrumental.mp3 "
                                  "-lyrics lyrics.txt "
                                  "-artistname artistName "
                                  "-title title\n";
    if (argc == 0) {
        cout << templateString;
        return -1;
    }

    if (argc == 2 && std::string(argv[1]) == "--help") {
        cout << templateString << endl;
        return 0;
    }

    std::string outputFilepath, midiFilepath, instrumentalFilePath, lyricsFilePath;
    std::string title, artistName;
    int midiTrackId;

    std::set<std::string> argDefinitions {"-o", "-midi", "-trackid", "-instrumental", "-artistname", "-title", "-lyrics"};

    for (int i = 1; i < argc; ++i) {
        std::string argDefinition(argv[i]);
        if (argDefinition[0] != '-' || !argDefinitions.count(argDefinition)) {
            continue;
        }

        auto getDescription = [&] () -> std::string {
            if (argDefinition[1] == 'o') {
                return "output file path";
            } else {
                return Strings::RemovePrefix(argDefinition, 1);
            }
        };

        if (i == argc - 1) {
            cerr << "Please specify " << getDescription();
            return -1;
        }

        std::string arg = argv[++i];

        if (argDefinitions.count(arg)) {
            cerr << "Please specify " << getDescription();
            return -1;
        }

        if (argDefinition == "-o") {
            outputFilepath = Strings::Unquote(arg);
        } else if (argDefinition == "-midi") {
            midiFilepath = Strings::Unquote(arg);
        } else if (argDefinition == "-trackid") {
            midiTrackId = Strings::TryParseInt(argDefinition, 1);
        } else if (argDefinition == "-instrumental") {
            instrumentalFilePath = Strings::Unquote(arg);
        } else if (argDefinition == "-artistname") {
            artistName = Strings::Unquote(arg);
        } else if (argDefinition == "-title") {
            title = Strings::Unquote(arg);
        } else if (argDefinition == "-lyrics") {
            lyricsFilePath = Strings::Unquote(arg);
        }
    }

    if (outputFilepath.empty()) {
        cerr << "Please specify output file path using -o command\n";
        return -1;
    }

    MidiFileReader reader;
    if (!reader.read(midiFilepath)) {
        cerr << "Broken or missing midi file, generating .mvx file failed\n";
        return -1;
    }
    double beatsPerMinute = reader.getBeatsPerMinute();

    VocalPart vocalPart;
    try {
        vocalPart = reader.tryGetVocalPartFromMidiTrackWithId(midiTrackId);
    } catch (MidiFileReaderException& e) {
        cerr << "Failed to parse midi file: " << e.what() << endl;
        return -1;
    }

    if (instrumentalFilePath.empty()) {
        cerr << "instrumental is missing\n";
        return -1;
    }

    MvxFile mvxFile;
    mvxFile.setVocalPart(vocalPart);
    mvxFile.setArtistNameUtf8(artistName);
    mvxFile.setSongTitleUtf8(title);
    mvxFile.loadInstrumentalFromFile(instrumentalFilePath.data());
    mvxFile.setBeatsPerMinute(beatsPerMinute);
    mvxFile.generateInstrumentalPreviewSamplesFromInstrumental();
    if (!lyricsFilePath.empty()) {
        mvxFile.loadLyricsFromFile(lyricsFilePath.data());
    }

    ApplicationModel::init();
    VocalTrainerFilePlayer* player = ApplicationModel::instance()->getPlayer();
    player->setSource(
            &mvxFile,
            false //destroyFileOnDestructor
    );

    try {
        player->prepare();
    } catch (VocalTrainerPlayerPrepareException& e) {
        if (e.getReason() == VocalTrainerPlayerPrepareException::DIFFERENT_DURATIONS) {
            double vocalPartDuration = vocalPart.getDurationInSeconds();
            double instrumentalDuration = player->getInstrumentalPlayer().getTrackDurationInSeconds();
            vocalPart = vocalPart.cutOrExpand(0, instrumentalDuration);
            mvxFile.setVocalPart(vocalPart);
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

    mvxFile.writeToFile(outputFilepath.data());

    cout << "mvx file has been created: " << outputFilepath << endl;

    return 0;
}
