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
    QFrame* hDivider = nullptr;
public:
    InfoDevice(QJsonObject obj);
    ~InfoDevice() override {
        widget->deleteLater();
        HBox->deleteLater();
        name->deleteLater();
        value->deleteLater();
        hDivider->deleteLater();
        tts->deleteLater();
    }
    void setValue(QString new_val) override;
    void checkTrigger(QString triggerPhrase) override;
    QString getType() override;
    void insertWidgetsIntoLayout(QLayout* layout) override;
};

#endif // INFODEVICE_H
