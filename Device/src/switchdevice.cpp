#include "Device/include/switchdevice.h"

SwitchDevice::SwitchDevice(QJsonObject obj):ADevice(obj) {
    toggleWidget = new QtMaterialToggle;
    toggleWidget->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    label = new QLabel(this->getName());
    widget = new QWidget;
    HLay = new QHBoxLayout;
    HLay->addWidget(label);
    HLay->addWidget(toggleWidget,0,Qt::AlignRight);
    HLay->setMargin(0);
    widget->setFixedHeight(50);
    widget->setLayout(HLay);

    connect(toggleWidget, &QtMaterialToggle::toggled, this, [=](bool checked){
        QString val = checked ? "ON" : "OFF";
        ADevice::setValue(val);
        if(this->getMqttClient()) {
            this->getMqttClient()->publish(getFeedBaseUrl() + getFeed(), val.toUtf8());
        } else {
            qDebug() << "Не задан Mqtt client";
        }
    });
}

void SwitchDevice::setValue(QString new_value) {
    if(new_value != getValue()) {
        ADevice::setValue(new_value);
        toggleWidget->blockSignals(true);
        toggleWidget->setChecked(new_value == "ON");
        toggleWidget->blockSignals(false);
    }


}

QString SwitchDevice::getType() {
    return "Переключаемое";
}

void SwitchDevice::checkTrigger(QString triggerPhrase) {

}


void SwitchDevice::insertWidgetsIntoLayout(QLayout* layout) {
    if(layout)
        layout->addWidget(widget);
}
