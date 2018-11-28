#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include "custommaterialdialog.h"
#include <QValidator>
#include <QMessageBox>
#include "connectiondata.h"
#include "qtmaterialappbar.h"
namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public CustomMaterialDialog
{
    Q_OBJECT

public:
    ConnectionDialog(QWidget *parent = nullptr, QString settings_file = "settings.ini");
    ConnectionData getData();
    ~ConnectionDialog();

private slots:
    void saveBtnClicked();

    void connectBtnClicked();

private:
    bool validateFields();
    Ui::ConnectionDialog *ui;
    QString _sfilename = "connection_info.ini";
};

#endif // CONNECTIONDIALOG_H
