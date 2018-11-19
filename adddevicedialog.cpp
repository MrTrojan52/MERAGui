#include "adddevicedialog.h"
#include "ui_adddevicedialog.h"

AddDeviceDialog::AddDeviceDialog(QWidget *parent, QString saveFilename) :
    QDialog(parent),
    ui(new Ui::AddDeviceDialog),
    _saveFilename(saveFilename)
{
    ui->setupUi(this);    
}

AddDeviceDialog::~AddDeviceDialog()
{
    delete ui;
}

void AddDeviceDialog::selectDevice(std::vector<ADevice*>& dev, QString group) {
    ui->lstDevices->clear();
    _devices = dev;
    _group = group;
    int cnt = 0;
    for(auto x : _devices)
        if(x->getGroup() == _group) {
            ui->lstDevices->addItem(x->getName());
            ui->lstDevices->item(cnt)->setToolTip(x->getFeed());
            ++cnt;
        }
    this->setModal(true);
    this->show();
}

void AddDeviceDialog::on_lstDevices_itemChanged(QListWidgetItem *item)
{
    ui->leTopic->setText(item->toolTip());
    ui->cmbType->setEnabled(true);
}

void AddDeviceDialog::on_cmbType_currentIndexChanged(const QString &arg1)
{
    ui->chkNeedrecognize->setEnabled(true);
    if(arg1 == "Переключаемое") {
        ui->cmbAction->clear();
        ui->cmbObject->clear();
        ui->cmbAction->addItem("включи / отключи");
        ui->cmbObject->addItems(QStringList({
                                                "лампу",
                                                "лампочку",
                                                "свет",
                                                "розетку"
                                            }));
    } else if(arg1 == "Информационное") {
        ui->cmbAction->clear();
        ui->cmbObject->clear();
        ui->cmbAction->addItems(QStringList({
                                                "скажи",
                                                "какая"
                                            }));
        ui->cmbObject->addItems(QStringList({
                                                "температура",
                                                "влажность"
                                            }));
    } else if(arg1 == "Редактируемое") {
        ui->chkNeedrecognize->setCheckState(Qt::CheckState::Unchecked);
        ui->chkNeedrecognize->setEnabled(false);
    }
}

void AddDeviceDialog::on_chkNeedrecognize_toggled(bool checked)
{
    if(checked) ui->cmbType->currentTextChanged(ui->cmbType->currentText());
    ui->cmbAction->setEnabled(checked);
    ui->cmbObject->setEnabled(checked);
    ui->cmbPretext->setEnabled(checked);
    ui->cmbWhere->setEnabled(checked);
}

void AddDeviceDialog::on_chkNeedResponse_toggled(bool checked)
{
    ui->leResponse->setEnabled(checked);
}
