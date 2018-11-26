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
    tts = new QTextToSpeech;
    tts->setLocale(QLocale("ru_RU"));
    tts->setVolume(0.5);
    auto x = tts->availableLocales();
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
        bool checked = new_value == "ON";
        toggleWidget->setChecked(checked);
    }
}

QString SwitchDevice::getType() {
    return "Переключаемое";
}

void SwitchDevice::checkTrigger(QString triggerPhrase) {
    QString actPhrase = getTriggerPhrase();
    if(needRecognize() && !actPhrase.isEmpty()) {
        QString onPhrase;
        QString offPhrase;
        if(actPhrase.indexOf('/') == -1)
        {
            setValue(getValue() == "ON" ? "OFF" : "ON");
            if(needResponse() && !getResponsePhrase().isEmpty())
            {

                tts->say(resolveVariables(getResponsePhrase()));
            }
        } else {
            QStringList lst = actPhrase.split('/');
            onPhrase = lst[0].trimmed();
            QStringList lstNext = lst[1].trimmed().split(' ');
            for(int i = 1; i < lstNext.size(); ++i)
                onPhrase += ' ' + lstNext[i];
            offPhrase = lst[1];
            lstNext = lst[0].trimmed().split(' ');
            for(int i = 0; i < lstNext.size() - 1; ++i)
                offPhrase = lstNext[i] + offPhrase;

            if(onPhrase == offPhrase)
                setValue(getValue() == "ON" ? "OFF" : "ON");
            else if(triggerPhrase == onPhrase) {
                setValue("ON");
                if(needResponse() && !getResponsePhrase().isEmpty())
                {


                    tts->say(resolveVariables(getResponsePhrase()));
                }
            }
            else if(triggerPhrase == offPhrase) {
                setValue("OFF");
                if(needResponse() && !getResponsePhrase().isEmpty())
                {

                    tts->say(resolveVariables(getResponsePhrase()));
                }
            }
        }

    }

}


void SwitchDevice::insertWidgetsIntoLayout(QLayout* layout) {
    if(layout)
        layout->addWidget(widget);
}
