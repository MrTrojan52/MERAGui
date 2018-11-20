#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>
#include "Device/include/adevice.h"
#include <QListWidgetItem>
#include "qtmaterialappbar.h"
namespace Ui {
class AddDeviceDialog;
}

class AddDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    AddDeviceDialog(QWidget *parent = nullptr, QString saveFilename = "devices.ini");
    void selectDevice(std::vector<ADevice*>& dev, QString group);
    ~AddDeviceDialog();

private slots:
    void on_lstDevices_itemChanged(QListWidgetItem *item);

    void on_cmbType_currentIndexChanged(const QString &arg1);

    void on_chkNeedrecognize_toggled(bool checked);

    void on_chkNeedResponse_toggled(bool checked);

private:
    Ui::AddDeviceDialog *ui;
    std::vector<ADevice*> _devices;
    QString _group;
    QString _saveFilename;
    QtMaterialAppBar* m_appBar = nullptr;
};

#endif // ADDDEVICEDIALOG_H
