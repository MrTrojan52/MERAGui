#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include <QSettings>
#include <QVariant>
#include <qtmaterialraisedbutton.h>
ConnectionDialog::ConnectionDialog(QWidget *parent, QString settings_file) :
    CustomMaterialDialog(parent),
    ui(new Ui::ConnectionDialog),
    _sfilename(settings_file)
{
    ui->setupUi(this);
    this->setHeaderText("Настройки подключения");
    ui->verticalLayout->insertWidget(0, this->getAppbar());
    ui->verticalLayout->insertWidget(0, this->closeBtn);
    ui->verticalLayout->setAlignment(this->closeBtn, Qt::AlignRight);

    QtMaterialRaisedButton* connectBtn = new QtMaterialRaisedButton("Подключиться", this);
    connectBtn->setBackgroundColor(QColor(238, 123, 66));
    connectBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    connectBtn->setRippleStyle(Material::RippleStyle::NoRipple);
    connect(connectBtn, SIGNAL(clicked()), this, SLOT(connectBtnClicked()));
    ui->verticalLayout->addWidget(connectBtn);

    QtMaterialRaisedButton* saveBtn = new QtMaterialRaisedButton("Сохранить", this);
    saveBtn->setBackgroundColor(QColor(0, 188, 212));
    saveBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    saveBtn->setRippleStyle(Material::RippleStyle::NoRipple);
    connect(saveBtn, SIGNAL(clicked()), this, SLOT(saveBtnClicked()));
    ui->verticalLayout->addWidget(saveBtn);


    ui->editPort->setValidator(new QIntValidator(1,999999));
    QSettings sett(_sfilename, QSettings::IniFormat);
    ui->editHost->setText(sett.value("AUTH/HOST").toString());
    ui->editPort->setText(sett.value("AUTH/PORT").toString());
    ui->editUsername->setText(sett.value("AUTH/USERNAME").toString());
    ui->editPassword->setText(sett.value("AUTH/PASSWORD").toString());
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::saveBtnClicked()
{
    if(validateFields()) {
        QSettings sett(_sfilename, QSettings::IniFormat);
        sett.setValue("AUTH/HOST", ui->editHost->text());
        sett.setValue("AUTH/PORT", ui->editPort->text());
        sett.setValue("AUTH/USERNAME", ui->editUsername->text());
        sett.setValue("AUTH/PASSWORD", ui->editPassword->text());
    } else {
        QMessageBox::critical(this, "Ошибка", "Пожалуйста, проверьте введенные данные!");
    }
}

void ConnectionDialog::connectBtnClicked()
{
    if(validateFields()) {
        emit accept();
    } else {
        QMessageBox::critical(this, "Ошибка", "Пожалуйста, проверьте введенные данные!");
    }
}

ConnectionData ConnectionDialog::getData() {
    return ConnectionData(ui->editHost->text(), ui->editPort->text(), ui->editUsername->text(), ui->editPassword->text());
}

bool ConnectionDialog::validateFields() {
    return (!ui->editHost->text().isEmpty() && !ui->editUsername->text().isEmpty() && !ui->editPassword->text().isEmpty() && ui->editPort->hasAcceptableInput());
}
