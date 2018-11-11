#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "SphinxReconizer/include/SphinxRecognizer.h"
#include <QtMqtt/QtMqtt>
#include <map>
#include "connectiondialog.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

public slots:
    void onRecognize(string);

private slots:
    void on_pushButton_clicked();
    void updateLogStateChange();
    void brokerDisconnected();
    void replyFinished(QNetworkReply*);
    void connectionDialogAccepted();
    void connectionDialogRejected();


private:
    Ui::Dialog *ui;
    ConnectionData _cData;
    SphinxRecognizer * _recognizer;
    QMqttClient * _mclient;
    std::map<QString, QString> feeds;
    ConnectionDialog* cdlg;
};

#endif // DIALOG_H
