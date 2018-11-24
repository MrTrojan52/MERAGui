#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include "custommaterialdialog.h"
#include "Device/include/adevice.h"
#include <QListWidgetItem>
#include "qtmaterialappbar.h"
#include "Device/include/availabledeviceinfo.h"
namespace Ui {
class AddDeviceDialog;
}

class AddDeviceDialog : public CustomMaterialDialog
{
    Q_OBJECT

public:
    AddDeviceDialog(QWidget *parent = nullptr, QString saveFilename = "devices.ini");
    void selectDevice(std::vector<AvailableDeviceInfo>& dev);
    ~AddDeviceDialog();

private slots:

    void on_cmbType_currentIndexChanged(const QString &arg1);

    void on_chkNeedrecognize_toggled(bool checked);

    void on_chkNeedResponse_toggled(bool checked);

    void on_lstDevices_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::AddDeviceDialog *ui;
    QString _saveFilename;
};

#endif // ADDDEVICEDIALOG_H
