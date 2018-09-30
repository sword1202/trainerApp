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
};

#endif // APPSETTINGS_H