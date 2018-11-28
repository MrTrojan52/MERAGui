#include "Device/include/infodevice.h"
#include <QtTextToSpeech>
#include <QAction>
InfoDevice::InfoDevice(QJsonObject obj):ADevice(obj) {
    name = new QLabel(getName());
    value = new QLabel(getValue());    
    HBox = new QHBoxLayout;
    HBox->addWidget(name);
    HBox->addWidget(value, 0, Qt::AlignRight);
    HBox->setMargin(0);
    HBox->setContentsMargins(0,0, 15,0);
    widget = new QWidget;
    widget->setMinimumHeight(50);
    widget->setFixedHeight(50);
    widget->setLayout(HBox);
    this->setDeleteAction(new QAction("Удалить устройство", widget));
    widget->addAction(this->getDeleteAction());
    widget->setContextMenuPolicy(Qt::ActionsContextMenu);
    hDivider = new QFrame;
    hDivider->setFrameShape(QFrame::HLine);
    tts = new QTextToSpeech;
    tts->setLocale(QLocale("ru_RU"));
    tts->setVolume(0.5);
}

QString InfoDevice::getType() {
    return "Информационное";
}

void InfoDevice::checkTrigger(QString triggerPhrase) {
    if(getTriggerPhrase() == triggerPhrase && needResponse() && needRecognize()) {
        tts->say(resolveVariables(getResponsePhrase()));
    }
}

void InfoDevice::setValue(QString new_value) {
    ADevice::setValue(new_value);
    value->setText(getValue());
}

void InfoDevice::insertWidgetsIntoLayout(QLayout* layout) {
    if(layout) {
        layout->addWidget(widget);
        layout->addWidget(hDivider);
    }
}
