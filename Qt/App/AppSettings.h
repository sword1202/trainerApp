#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>
#include <QByteArray>

class AppSettings : public QSettings
{
public:
    AppSettings();
    QByteArray getMicrophoneDeviceName() const;
    void setMicrophoneDeviceName(const QByteArray &);

    float getInputVolume();
    void setOutputVolume(float value);
    float getOutputVolume();
    void setInputVolume(float value);
    float getRecordingVolume();
    void setRecordingVolume(float value);


    QStringList getRecordings();
    void setRecordings(const QStringList &recordings);
    QStringList getProjects();
    void setProjects(const QStringList &projects);
    void addProject(const QString& filePath);
    void removeProject(const QString& filePath);
    void addRecording(const QString& filePath);
    void removeRecording(const QString& filePath);
};

#endif // APPSETTINGS_H
