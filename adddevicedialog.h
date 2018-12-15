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
    AddDeviceDialog(QWidget *parent = nullptr, QString saveFilename = "devices.json", QString grammFile = "");
    void selectDevice(std::vector<AvailableDeviceInfo>& dev, QString group);
    ~AddDeviceDialog();
    void setGrammFile(QString grammFile);

private slots:

    void addBtnClicked();

    void on_cmbType_currentIndexChanged(const QString &arg1);

    void on_chkNeedrecognize_toggled(bool checked);

    void on_chkNeedResponse_toggled(bool checked);

    void on_lstDevices_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
signals:
    void deviceListChanged();
private:
    QJsonObject generateJsonObjectFromFields();
    Ui::AddDeviceDialog *ui;
    QString _saveFilename;
    QString _grammFile;
    QString m_group;
};

#endif // ADDDEVICEDIALOG_H
