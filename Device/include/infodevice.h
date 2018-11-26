#ifndef INFODEVICE_H
#define INFODEVICE_H
#include "Device/include/adevice.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QtTextToSpeech>
class InfoDevice : public ADevice {
private:
    QLabel* name = nullptr;
    QLabel* value = nullptr;
    QHBoxLayout* HBox = nullptr;
    QWidget* widget = nullptr;
    QTextToSpeech* tts = nullptr;
public:
    InfoDevice(QJsonObject obj);
    ~InfoDevice() override {
        delete widget;
        delete HBox;
        delete name;
        delete value;
        delete tts;
    }
    void setValue(QString new_val) override;
    void checkTrigger(QString triggerPhrase) override;
    QString getType() override;
    void insertWidgetsIntoLayout(QLayout* layout) override;
};

#endif // INFODEVICE_H
