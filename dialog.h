#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "SphinxReconizer/include/SphinxRecognizer.h"
#include <QtMqtt/QtMqtt>
#include <utility>
#include <vector>
#include "connectiondialog.h"
#include "SwitchWidget/include/switch.h"
#include "Device/include/adevice.h"
#include "recognizersettingsdialog.h"
using std::vector;
using std::pair;
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
signals:
    void startRecognition(string from_device = "plughw:1,0");
public slots:
    void onRecognize(string);

private slots:
    void updateLogStateChange();
    void brokerDisconnected();
    void replyFinished(QNetworkReply*);
    void connectionDialogAccepted();
    void connectionDialogRejected();
    void generateControls();

    void on_tbRecognizeSettings_clicked();

private:
    void initMQTTClient();
    void getAllFeeds();
    void executeCommand(std::map<QString,QString>&);
    Switch* find_switch(QString topic);
    Ui::Dialog *ui;
    ConnectionData _cData;
    SphinxRecognizer * _recognizer;
    QMqttClient * _mclient;
    vector<ADevice*> devices;
    vector<Switch*> vecSwitch;
    ConnectionDialog* cdlg;
    RecognizerSettingsDialog* rsDlg;
    QThread* _recognizeThread = nullptr;
};

#endif // DIALOG_H
