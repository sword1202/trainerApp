//
// Created by Semyon Tikhonenko on 2018-12-17.
//

#include <QQuickWidget>
#include <QFileDialog>
#include <QQmlContext>
#include <Playback/Midi/MidiFileReader.h>
#include <QMessageBox>
#include <QStandardPaths>
#include <Decoder/audiodecoder.h>
#include <MidiFileReaderException.h>
#include <MvxPlayer.h>
#include "MvxFile.h"
#include "AudioUtils.h"

#ifndef MVXGENERATOR_HANDLER_H
#define MVXGENERATOR_HANDLER_H

#define assert(condition) if (!condition) { \
    QMessageBox::critical(dynamic_cast<QWidget *>(parent()), "Error", #condition); \
    return; \
}

class Handler : public QObject {
    Q_OBJECT
public:
    explicit Handler(QWidget *parent) : QObject(parent) {}

    Q_INVOKABLE QString showSelectInstrumental() {
        return QFileDialog::getOpenFileName(
                dynamic_cast<QWidget *>(parent()),
                "Select instrumental", "", "Mp3 files(*.mp3);; All files(*)");
    }

    Q_INVOKABLE QString showSelectMidi() {
        return QFileDialog::getOpenFileName(
                dynamic_cast<QWidget *>(parent()),
                "Select midi", "", "Midi files(*.midi *.mid);; All files(*)");
    }

    Q_INVOKABLE void submit(const QString& instrumentalFilePath,
            const QString& midiFilePath,
            int midiIndex,
            const QString& artistName,
            const QString& title) {
        assert(!instrumentalFilePath.isEmpty())
        assert(!midiFilePath.isEmpty())

        MidiFileReader reader;
        reader.read(midiFilePath.toLocal8Bit().toStdString());
        double beatsPerMinute = reader.getBeatsPerMinute();
        if (beatsPerMinute < 0) {
            QMessageBox::critical(dynamic_cast<QWidget *>(parent()), "Error", "Broken or unsupported midi file");
            return;
        }

        VocalPart vocalPart;
        try {
            vocalPart = reader.tryGetVocalPartFromMidiTrackWithId(midiIndex);
        } catch (MidiFileReaderException& e) {
            QMessageBox::critical(dynamic_cast<QWidget *>(parent()), "Error", e.what());
            return;
        }

        auto outputFilePath = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation)[0];
        outputFilePath += "/" + artistName + "-" + title;

        while (QFile::exists(outputFilePath + ".mvx")) {
            outputFilePath += "_";
        }

        outputFilePath += ".mvx";

        MvxFile mvxFile;
        mvxFile.setVocalPart(vocalPart);
        mvxFile.setArtistNameUtf8(artistName.toUtf8().toStdString());
        mvxFile.setSongTitleUtf8(title.toUtf8().toStdString());
        mvxFile.loadInstrumentalFromFile(instrumentalFilePath.toLocal8Bit().data());
        mvxFile.setBeatsPerMinute(beatsPerMinute);
        mvxFile.generateInstrumentalPreviewSamplesFromInstrumental();

        MvxPlayer mvxPlayer;
        mvxPlayer.init(std::move(mvxFile));
        try {
            mvxPlayer.prepare();
        } catch (std::exception& e) {
            QMessageBox::critical(dynamic_cast<QWidget *>(parent()), "Error", e.what());
            return;
        }

        mvxPlayer.getMvxFile().writeToFile(outputFilePath.toLocal8Bit().data());

        QMessageBox::information(dynamic_cast<QWidget *>(parent()), "File created", outputFilePath);
    }
};

#endif //MVXGENERATOR_HANDLER_H
