#ifndef INFODEVICE_H
#define INFODEVICE_H
#include "Device/include/adevice.h"
class InfoDevice : public ADevice {
public:
    InfoDevice(QJsonObject obj):ADevice(obj) {}
    void checkTrigger(QString triggerPhrase) override;
    QString getType() override;
};

#endif // INFODEVICE_H
