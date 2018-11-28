#include "Device/include/editdevice.h"
#include <QColor>
EditDevice::EditDevice(QJsonObject obj):ADevice(obj) {
    lEdit = new QTextEdit;
    lEdit->setPlaceholderText("Введите значение параметра: \"" + getName() + "\"");
    lEdit->setStyleSheet("");
    submit = new QtMaterialRaisedButton("Отправить");
    submit->setBackgroundColor(QColor(0, 188, 212));
    submit->setRippleStyle(Material::NoRipple);
    submit->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    Hbox = new QHBoxLayout;
    widget = new QWidget;
    Hbox->addWidget(lEdit);
    Hbox->addWidget(submit, 0, Qt::AlignRight);
    Hbox->setMargin(0);
    widget->setMinimumHeight(50);
    widget->setFixedHeight(50);
    widget->setLayout(Hbox);
    this->setDeleteAction(new QAction("Удалить устройство", widget));
    widget->addAction(this->getDeleteAction());
    widget->setContextMenuPolicy(Qt::ActionsContextMenu);
    hDivider = new QFrame;
    hDivider->setFrameShape(QFrame::HLine);
    connect(submit, &QtMaterialRaisedButton::clicked, this, [this](){
        ADevice::setValue(lEdit->toPlainText());
        if(this->getMqttClient()) {
            this->getMqttClient()->publish(getFeedBaseUrl() + getFeed(), lEdit->toPlainText().toUtf8());
        } else {
            qDebug() << "Не задан Mqtt client";
        }
    });
}

QString EditDevice::getType() {
    return "Редактируемое";
}

void EditDevice::checkTrigger(QString triggerPhrase) {
    Q_UNUSED(triggerPhrase);
}

void EditDevice::insertWidgetsIntoLayout(QLayout* layout) {
    if(layout) {
        layout->addWidget(widget);
        layout->addWidget(hDivider);
    }
}
