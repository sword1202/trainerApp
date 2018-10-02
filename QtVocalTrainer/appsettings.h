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
};

#endif // APPSETTINGS_H