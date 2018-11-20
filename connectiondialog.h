#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QValidator>
#include <QMessageBox>
#include <tuple>
#include "connectiondata.h"
#include "qtmaterialappbar.h"
namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectionDialog(QWidget *parent = nullptr, QString settings_file = "settings.ini");
    ConnectionData getData();
    ~ConnectionDialog();

private slots:
    void on_pbSave_clicked();

    void on_pbConnect_clicked();

private:
    Ui::ConnectionDialog *ui;
    QString _sfilename = "connection_info.ini";
    QtMaterialAppBar* m_appBar = nullptr;
};

#endif // CONNECTIONDIALOG_H
