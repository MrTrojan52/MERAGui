#ifndef SWITCHDEVICE_H
#define SWITCHDEVICE_H
#include "adevice.h"

class SwitchDevice : public ADevice {
public:
    //SwitchDevice(QString name, QString value, QString group, QString feed):ADevice(name, value, group, feed) {}
    SwitchDevice(QJsonObject obj):ADevice(obj) {}
    void checkTrigger(QString triggerPhrase) override;
    QString getType() override;
};

#endif // SWITCHDEVICE_H
