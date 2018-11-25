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
public:
    EditDevice(QJsonObject obj);
    ~EditDevice() override {
        delete Hbox;
        delete lEdit;
        delete submit;
        delete widget;
    }
    void checkTrigger(QString triggerPhrase) override;
    QString getType() override;
    void insertWidgetsIntoLayout(QLayout* layout) override;
};

#endif // EDITDEVICE_H
