#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "SphinxReconizer/include/SphinxRecognizer.h"
#include <QtMqtt/QtMqtt>
#include <utility>
#include <vector>
#include "connectiondialog.h"
//#include "SwitchWidget/include/switch.h"
#include "Device/include/adevice.h"
#include "recognizersettingsdialog.h"
#include "adddevicedialog.h"
#include <QToolButton>
#include "qtmaterialappbar.h"
#include <qtmaterialiconbutton.h>
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
    void addButtonClicked();
    void recognitionSettingsClicked();
    void startRecognizeClicked();
    void closeBtnClicked();
    void minimizeBtnClicked();

private:    
    void generateControls();
    void initMQTTClient();
    void getAllFeeds();
    void executeCommand(std::map<QString,QString>&);
    QtMaterialAppBar* m_appBar = nullptr;
    QtMaterialIconButton* m_recognizeBtn = nullptr;
    QtMaterialIconButton* m_settingsBtn = nullptr;
   // Switch* find_switch(QString topic);
    Ui::Dialog *ui;
    ConnectionData _cData;
    SphinxRecognizer * _recognizer = nullptr;
    QMqttClient * _mclient = nullptr;
    vector<ADevice*> devices;
   // vector<Switch*> vecSwitch;
    ConnectionDialog* cdlg = nullptr;
    RecognizerSettingsDialog* rsDlg = nullptr;
    AddDeviceDialog* addDlg = nullptr;
    QThread* _recognizeThread = nullptr;
    QString _sfilename = "settings.ini";
    QString _sdevicesFilename = "devices.ini";
    QToolButton* addButton;
    bool m_recognizeBtnChecked = true;
};

#endif // DIALOG_H
