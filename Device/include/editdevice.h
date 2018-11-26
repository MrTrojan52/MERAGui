#ifndef EDITDEVICE_H
#define EDITDEVICE_H
#include "Device/include/adevice.h"
#include <qtmaterialraisedbutton.h>
#include <QHBoxLayout>
#include <QTextEdit>
class EditDevice : public ADevice {
private:
    QHBoxLayout* Hbox = nullptr;
    QTextEdit* lEdit = nullptr;
    QtMaterialRaisedButton* submit = nullptr;
    QWidget* widget = nullptr;
    QFrame* hDivider = nullptr;
public:
    EditDevice(QJsonObject obj);
    ~EditDevice() override {
        Hbox->deleteLater();
        lEdit->deleteLater();
        submit->deleteLater();
        widget->deleteLater();
        hDivider->deleteLater();
    }
    void checkTrigger(QString triggerPhrase) override;
    QString getType() override;
    void insertWidgetsIntoLayout(QLayout* layout) override;
};

#endif // EDITDEVICE_H
