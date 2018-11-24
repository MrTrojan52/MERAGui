#include "adddevicedialog.h"
#include "ui_adddevicedialog.h"

AddDeviceDialog::AddDeviceDialog(QWidget *parent, QString saveFilename) :
    CustomMaterialDialog(parent),
    ui(new Ui::AddDeviceDialog),
    _saveFilename(saveFilename)
{
    ui->setupUi(this);    

    this->setHeaderText("Добавление устройства");
    ui->verticalLayout->insertWidget(0, this->getAppbar());
    ui->verticalLayout->insertWidget(0, this->closeBtn);
    ui->verticalLayout->setAlignment(this->closeBtn, Qt::AlignRight);
}

AddDeviceDialog::~AddDeviceDialog()
{
    delete ui;
}

void AddDeviceDialog::selectDevice(std::vector<AvailableDeviceInfo>& dev) {
    ui->lstDevices->clear();
    for(auto x : dev) {
        QListWidgetItem* newItem = new QListWidgetItem(x.getName());
        newItem->setToolTip(x.getFeed());
        newItem->setData(Qt::UserRole, x.getFeed());
        ui->lstDevices->addItem(newItem);
    }
    this->setModal(true);
    this->show();
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
        ui->chkNeedrecognize->setChecked(false);
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

void AddDeviceDialog::on_lstDevices_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)
    ui->leTopic->setText(current->data(Qt::UserRole).toString());
    ui->cmbType->setEnabled(true);
}
