#include "adddevicedialog.h"
#include "ui_adddevicedialog.h"
#include <qtmaterialraisedbutton.h>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
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

    QtMaterialRaisedButton* addBtn = new QtMaterialRaisedButton("Добавить", this);
    addBtn->setBackgroundColor(QColor(0, 188, 212));
    addBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    addBtn->setRippleStyle(Material::RippleStyle::NoRipple);
    ui->verticalLayout->addWidget(addBtn);

    connect(addBtn, SIGNAL(clicked()), this, SLOT(addBtnClicked()));
}

AddDeviceDialog::~AddDeviceDialog()
{
    delete ui;
}

void AddDeviceDialog::selectDevice(std::vector<AvailableDeviceInfo>& dev, QString group) {
    m_group = group;
    ui->lstDevices->clear();
    for(auto x : dev) {
        QListWidgetItem* newItem = new QListWidgetItem(x.getName());
        newItem->setToolTip(x.getFeed());
        newItem->setData(Qt::UserRole, x.getFeed());
        ui->lstDevices->addItem(newItem);
    }
    if(ui->lstDevices->count())
        ui->lstDevices->setCurrentRow(0);
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
        ui->chkNeedResponse->setChecked(false);
        ui->chkNeedResponse->setEnabled(false);
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
    if(current) {
        ui->leTopic->setText(current->data(Qt::UserRole).toString());
        ui->cmbType->setEnabled(true);
    }
}

void AddDeviceDialog::addBtnClicked() {
    if(ui->lstDevices->count() && ui->lstDevices->currentRow() != -1)
    {

        QFile jsonFile(this->_saveFilename);
        QJsonDocument jDoc;
        if(jsonFile.open(QFile::ReadOnly | QIODevice::Text)) {
            jDoc = QJsonDocument::fromJson(jsonFile.readAll());
            jsonFile.close();
            QJsonObject jObj = jDoc.object();
            if(jObj.find("devices") != jObj.end())
            {
                if(jObj["devices"].isArray())
                {
                    QJsonArray jDevArray = jObj["devices"].toArray();
                    jDevArray.push_back(generateJsonObjectFromFields());
                    jObj["devices"] = jDevArray;
                } else {
                    jObj.remove("devices");
                    QJsonArray jDevArray;
                    jDevArray.push_back(generateJsonObjectFromFields());
                    jObj["devices"] = jDevArray;
                }
            } else {
                QJsonArray jDevArray;
                jDevArray.push_back(generateJsonObjectFromFields());
                jObj["devices"] = jDevArray;
            }

            jDoc.setObject(jObj);
            jsonFile.open(QFile::WriteOnly);
            jsonFile.write(jDoc.toJson());
            jsonFile.close();
        } else {
            jsonFile.close();
            jsonFile.open(QFile::WriteOnly);
            QJsonObject jODev;
            QJsonArray jDevArray;

            jDevArray.push_back(generateJsonObjectFromFields());

            jODev["devices"] = jDevArray;
            jDoc.setObject(jODev);
            jsonFile.write(jDoc.toJson());
            jsonFile.close();
        }
        QMessageBox::information(this, "Информация", "Устройство успешно сохранено.");
        emit deviceListChanged();

    } else {
        QMessageBox::critical(this,"Ошибка", "Не выбрано ни одно устройство или устройства отсутствуют!");
    }
}

QJsonObject AddDeviceDialog::generateJsonObjectFromFields() {
    QString triggerPhrase = ui->cmbName->currentText() + ' ' + ui->cmbAction->currentText() + ' ' + ui->cmbObject->currentText() + ' ' +
                            ui->cmbPretext->currentText() + ' ' + ui->cmbWhere->currentText();
    QJsonObject jDevice;
    jDevice["type"] = ui->cmbType->currentText();
    jDevice["group"] = m_group;
    jDevice["name"] = ui->lstDevices->currentItem()->text();
    jDevice["topic"] = ui->leTopic->text();
    jDevice["needRecognition"] = ui->chkNeedrecognize->isChecked();
    jDevice["triggerPhrase"] = ui->chkNeedrecognize->isChecked() ? triggerPhrase : "";
    jDevice["needResponse"] = ui->chkNeedResponse->isChecked();
    jDevice["responsePhrase"] = ui->chkNeedResponse->isChecked() ? ui->leResponse->text() : "";
    return  jDevice;
}
