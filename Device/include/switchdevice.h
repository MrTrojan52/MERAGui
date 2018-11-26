#ifndef SWITCHDEVICE_H
#define SWITCHDEVICE_H
#include "adevice.h"
#include <qtmaterialtoggle.h>
#include <QLabel>
#include <QtTextToSpeech>
class SwitchDevice : public ADevice {
private:
    QtMaterialToggle* toggleWidget;
    QLabel* label;
    QHBoxLayout* HLay = nullptr;
    QWidget* widget = nullptr;
    QTextToSpeech* tts = nullptr;
public:
    //SwitchDevice(QString name, QString value, QString group, QString feed):ADevice(name, value, group, feed) {}
    SwitchDevice(QJsonObject obj);
    ~SwitchDevice() override {
        delete HLay;
        delete label;
        delete toggleWidget;
        delete widget;
        tts->deleteLater();
    }
    void setValue(QString new_value) override;
    void checkTrigger(QString triggerPhrase) override;
    QString getType() override;
    void insertWidgetsIntoLayout(QLayout* layout) override;
};

#endif // SWITCHDEVICE_H
