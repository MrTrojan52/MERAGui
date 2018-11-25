#ifndef EDITDEVICE_H
#define EDITDEVICE_H
#include "Device/include/adevice.h"
class EditDevice : public ADevice {
public:
    EditDevice(QJsonObject obj):ADevice(obj) {}
    void checkTrigger(QString triggerPhrase) override;
    QString getType() override;
};

#endif // EDITDEVICE_H
