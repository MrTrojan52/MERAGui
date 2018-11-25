#include "Device/include/infodevice.h"

InfoDevice::InfoDevice(QJsonObject obj):ADevice(obj) {
    name = new QLabel(getName() + ":");
    value = new QLabel(getValue());
    HBox = new QHBoxLayout;
    HBox->addWidget(name);
    HBox->addWidget(value, 0, Qt::AlignRight);
    HBox->setMargin(0);
    widget = new QWidget;
    widget->setFixedHeight(50);
    widget->setLayout(HBox);

}

QString InfoDevice::getType() {
    return "Информационное";
}

void InfoDevice::checkTrigger(QString triggerPhrase) {

}

void InfoDevice::setValue(QString new_value) {
    ADevice::setValue(new_value);
    value->setText(getValue());
}

void InfoDevice::insertWidgetsIntoLayout(QLayout* layout) {
    if(layout)
        layout->addWidget(widget);
}
