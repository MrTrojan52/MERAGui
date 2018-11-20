#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include <QSettings>
#include <QVariant>
ConnectionDialog::ConnectionDialog(QWidget *parent, QString settings_file) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog),
    _sfilename(settings_file)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);

    QLabel *label = new QLabel("Настройки подключения", this);
    label->setAttribute(Qt::WA_TranslucentBackground);
    label->setForegroundRole(QPalette::Foreground);
    label->setContentsMargins(6, 0, 0, 0);

    QPalette palette = label->palette();
    palette.setColor(label->foregroundRole(), Qt::white);
    label->setPalette(palette);

    label->setFont(QFont("Roboto", 18, QFont::Normal));

    m_appBar = new QtMaterialAppBar(this);
    m_appBar->appBarLayout()->addWidget(label);
    m_appBar->appBarLayout()->setAlignment(label, Qt::AlignCenter);
    m_appBar->setFixedHeight(65);
    ui->verticalLayout->insertWidget(0, m_appBar);

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

void ConnectionDialog::on_pbSave_clicked()
{
    if(ui->editHost->text().trimmed() != "" && ui->editUsername->text().trimmed() != "" && ui->editPassword->text().trimmed() != "" && ui->editPort->hasAcceptableInput()) {
        QSettings sett(_sfilename, QSettings::IniFormat);
        sett.setValue("AUTH/HOST", ui->editHost->text());
        sett.setValue("AUTH/PORT", ui->editPort->text());
        sett.setValue("AUTH/USERNAME", ui->editUsername->text());
        sett.setValue("AUTH/PASSWORD", ui->editPassword->text());
    } else {
        QMessageBox::critical(this, QLatin1String("Validation Error"), QLatin1String("Please, check your input"));
    }
}

void ConnectionDialog::on_pbConnect_clicked()
{
    if(ui->editHost->text().trimmed() != "" && ui->editUsername->text().trimmed() != "" && ui->editPassword->text().trimmed() != "" && ui->editPort->hasAcceptableInput()) {
        emit accept();
    } else {
        QMessageBox::critical(this, QLatin1String("Validation Error"), QLatin1String("Please, check your input"));
    }
}

ConnectionData ConnectionDialog::getData() {
    return ConnectionData(ui->editHost->text(), ui->editPort->text(), ui->editUsername->text(), ui->editPassword->text());
}
