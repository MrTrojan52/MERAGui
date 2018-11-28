#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "SphinxReconizer/include/SphinxRecognizer.h"
#include <QtMqtt/QtMqtt>
#include <utility>
#include <vector>
#include "connectiondialog.h"
#include "Device/include/availabledeviceinfo.h"
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
    bool eventFilter(QObject* object, QEvent* event);
    ~Dialog();
signals:
    void startRecognition(string from_device = "plughw:1,0");
public slots:
    void onRecognize(string);

private slots:
    void updateDevices();
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
    void deleteDeviceByIndex(int index);
    void generateControls();
    void initMQTTClient();
    void getAllFeeds();
    QtMaterialAppBar* m_appBar = nullptr;
    QtMaterialIconButton* m_recognizeBtn = nullptr;
    QtMaterialIconButton* m_settingsBtn = nullptr;
    vector<ADevice*> _devices;
    Ui::Dialog *ui;
    ConnectionData _cData;
    SphinxRecognizer * _recognizer = nullptr;
    QMqttClient * _mclient = nullptr;
    ConnectionDialog* cdlg = nullptr;
    RecognizerSettingsDialog* rsDlg = nullptr;
    AddDeviceDialog* addDlg = nullptr;
    QThread* _recognizeThread = nullptr;
    QString _sfilename = "settings.ini";
    QString _sdevicesFilename = "devices.json";
    QToolButton* addButton;
    std::map<QString, std::vector<AvailableDeviceInfo>> availableDevicesByGroup;
    bool m_recognizeBtnChecked = true;
    QPoint mpos;
};

#endif // DIALOG_H
