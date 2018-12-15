#include "recognizersettingsdialog.h"
#include "ui_recognizersettingsdialog.h"
#include <QProcess>
#include <QFileDialog>
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <qtmaterialraisedbutton.h>
#include <QLabel>

RecognizerSettingsDialog::RecognizerSettingsDialog(QWidget *parent, QString settings_file) :
    CustomMaterialDialog(parent),
    ui(new Ui::RecognizerSettingsDialog),
    _sfilename(settings_file)
{
    ui->setupUi(this);
    this->setHeaderText("Настройки распознавания");

    ui->verticalLayout_2->insertWidget(0, this->getAppbar());
    ui->verticalLayout_2->insertWidget(0, this->closeBtn);
    ui->verticalLayout_2->setAlignment(this->closeBtn, Qt::AlignRight);

    QtMaterialRaisedButton* saveBtn = new QtMaterialRaisedButton("Сохранить", this);
    saveBtn->setBackgroundColor(QColor(0, 188, 212));
    saveBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    saveBtn->setRippleStyle(Material::RippleStyle::NoRipple);
    ui->verticalLayout_2->addWidget(saveBtn);
    /*****          Fill combobox with audio devices          *****/
    QProcess sh;
    sh.start("sh");
    sh.write("arecord -L | grep plughw");
    sh.closeWriteChannel();
    sh.waitForFinished();
    QString res(sh.readAllStandardOutput());
    QStringList devices = res.split('\n');
    ui->cmbAudioDevice->addItems(devices.filter(QRegExp("^(?!\s*$).+")));
    foreach(QString engine, QTextToSpeech::availableEngines())
    {
        ui->cmbEngine->addItem(engine, engine);
    }



    if(devices.empty())
        QMessageBox::critical(this, "Ошибка", "Отсутствуют устройства записи");
    connect(ui->leModel, SIGNAL(textChanged(QString)), this, SLOT(fieldEdited(QString)));
    connect(ui->leMdef, SIGNAL(textChanged(QString)), this, SLOT(fieldEdited(QString)));
    connect(ui->leDictionary, SIGNAL(textChanged(QString)), this, SLOT(fieldEdited(QString)));
    connect(ui->leGramm, SIGNAL(textChanged(QString)), this, SLOT(fieldEdited(QString)));
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveBtnClicked()));
    fillFieldsFromIniFile();
}

RecognizerSettingsDialog::~RecognizerSettingsDialog()
{
    delete tts;
    delete ui;
}

void RecognizerSettingsDialog::fillFieldsFromIniFile() {
    QSettings sett(_sfilename, QSettings::IniFormat);
    ui->leModel->setText(sett.value("SPHINX/MODEL").toString());
    ui->leMdef->setText(sett.value("SPHINX/MDEF").toString());
    ui->leDictionary->setText(sett.value("SPHINX/DICTIONARY").toString());
    ui->leGramm->setText(sett.value("SPHINX/GRAMMAR").toString());
    QString aDevice = sett.value("SPHINX/ADEVICE").toString();
    QString engine = sett.value("TTS/ENGINE").toString();
    QString voice = sett.value("TTS/VOICE").toString();
    if(!aDevice.isEmpty()) {
        int index = ui->cmbAudioDevice->findText(aDevice);
        if(index != -1)
            ui->cmbAudioDevice->setCurrentIndex(index);
    }

    int engineId = ui->cmbEngine->findData(engine);
    if(engineId != -1) {
        ui->cmbEngine->setCurrentIndex(engineId);
        int itemId = ui->cmbVoice->findData(voice);
        if(itemId != -1) {
            ui->cmbVoice->setCurrentIndex(itemId);
        }
    }
}

bool RecognizerSettingsDialog::validateAllFields() {
    bool res = true;
    if(ui->leModel->text().isEmpty() || !QDir().exists(ui->leModel->text()))
    {
        ui->leModel->setStyleSheet("border: 1px solid red;");
        res = false;
    }

    if(ui->leMdef->text().isEmpty() || !QFile().exists(ui->leMdef->text()))
    {
        ui->leMdef->setStyleSheet("border: 1px solid red;");
        res = false;
    }

    if(ui->leDictionary->text().isEmpty() || !QFile().exists(ui->leDictionary->text()))
    {
        ui->leDictionary->setStyleSheet("border: 1px solid red;");
        res = false;
    }

    if(ui->leGramm->text().isEmpty() || !QFile().exists(ui->leGramm->text()))
    {
        ui->leGramm->setStyleSheet("border: 1px solid red;");
        res = false;
    }

    if(ui->cmbAudioDevice->currentIndex() == -1 ) {
        ui->leGramm->setStyleSheet("border: 1px solid red;");
        res = false;
    }

    if(!res)
        QMessageBox::critical(this, "Ошибка", "Проверьте правильность введенных данных");
    return res;
}

void RecognizerSettingsDialog::on_tbModelSelect_clicked()
{
    ui->leModel->setText(QFileDialog::getExistingDirectory(this, "Выберите папку с языковой моделью"));
}

void RecognizerSettingsDialog::on_tbMdefSelect_clicked()
{
    ui->leMdef->setText(QFileDialog::getOpenFileName(this, "Выберите mdef файл"));
}



void RecognizerSettingsDialog::on_tbDictSelect_clicked()
{
    ui->leDictionary->setText(QFileDialog::getOpenFileName(this, "Выберите файл со словарём"));
}



void RecognizerSettingsDialog::on_tbGrammSelect_clicked()
{
    ui->leGramm->setText(QFileDialog::getOpenFileName(this, "Выберите файл с грамматикой"));
}

void RecognizerSettingsDialog::saveBtnClicked()
{
    if(validateAllFields()) {
        if(_fieldsStateChanged) {
            QSettings sett(_sfilename, QSettings::IniFormat);
            sett.setValue("SPHINX/MODEL", ui->leModel->text());
            sett.setValue("SPHINX/MDEF", ui->leMdef->text());
            sett.setValue("SPHINX/DICTIONARY", ui->leDictionary->text());
            sett.setValue("SPHINX/GRAMMAR", ui->leGramm->text());
            sett.setValue("SPHINX/ADEVICE", ui->cmbAudioDevice->currentText());
            sett.setValue("TTS/ENGINE", ui->cmbEngine->currentData().toString());
            sett.setValue("TTS/VOICE", ui->cmbVoice->currentData().toString());
            TTSSettingsChanged();
        }

        emit accept();
    }
}

void RecognizerSettingsDialog::fieldEdited(QString text) {
    Q_UNUSED(text)
    this->_fieldsStateChanged = true;
    QLineEdit* obj = qobject_cast<QLineEdit*>(sender());
    obj->setStyleSheet("");
}

void RecognizerSettingsDialog::on_cmbAudioDevice_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->_fieldsStateChanged = true;
    ui->leGramm->setStyleSheet("");
}

void RecognizerSettingsDialog::on_cmbEngine_currentIndexChanged(int index)
{
    delete tts;
    tts = new QTextToSpeech(ui->cmbEngine->itemData(index).toString());
    QVector<QVoice> voices = tts->availableVoices();
    foreach(const QVoice& voice, voices) {
        ui->cmbVoice->addItem(QString("%1 - %2 - %3").arg(voice.name())
                          .arg(QVoice::genderName(voice.gender()))
                          .arg(QVoice::ageName(voice.age())), voice.name());

    }
}
