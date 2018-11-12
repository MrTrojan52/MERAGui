#include "dialog.h"
#include "ui_dialog.h"
#define MODELDIR "/home/trojan52/Qt/Projects/MERAGui/rus_model"
#include <QMessageBox>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    cdlg = new ConnectionDialog(this);
    cdlg->setModal(true);
    cdlg->show();
    connect(cdlg, &ConnectionDialog::accepted, this, &Dialog::connectionDialogAccepted);
    connect(cdlg, &ConnectionDialog::rejected, this, &Dialog::connectionDialogRejected);
}

Dialog::~Dialog()
{
    delete ui;
    delete _recognizer;
    delete cdlg;
    delete _mclient;

    _recognizeThread->exit();
    delete _recognizeThread;
    for(size_t i = 0; i < vecSwitch.size(); ++i)
        delete vecSwitch[i];
}

void Dialog::onRecognize(string text) {
    qDebug() << text.c_str();
}

void Dialog::brokerDisconnected() {
    QMessageBox::warning(this, QLatin1String("Error"), QLatin1String("Client disconnect from broker."));
}

void Dialog::updateLogStateChange() {
    qDebug() << "State changed to " << _mclient->state();
    if(_mclient->state() == QMqttClient::Connected) {


        auto subscription = _mclient->subscribe(QString(_cData.Username + "/feeds/+"));
        if (!subscription) {
                QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
                return;
            }
    }
}

void Dialog::replyFinished(QNetworkReply* reply) {
    QJsonDocument jDocument = QJsonDocument::fromJson(reply->readAll());
    QJsonArray jA = jDocument.array();
    for(int i = 0; i < jA.size(); ++i) {
        QJsonObject jO = jA.at(i).toObject();
        feeds.emplace_back(jO.value("name").toString(), jO.value("key").toString());
    }
    reply->deleteLater();
    feeds.shrink_to_fit();
    generateControls();
}

void Dialog::connectionDialogAccepted() {
    this->_cData = cdlg->getData();
    getAllFeeds();
    initMQTTClient();
}

void Dialog::connectionDialogRejected() {
    cdlg->show();
}

void Dialog::initMQTTClient() {
    _mclient = new QMqttClient();
    _mclient->setHostname(_cData.Host);
    _mclient->setPort(_cData.Port.toInt());
    _mclient->setUsername(_cData.Username);
    _mclient->setPassword(_cData.Password); //"36d27d262cda42cf8e357bb722795f72"
    connect(_mclient, &QMqttClient::messageReceived, this, [](const QByteArray &message, const QMqttTopicName &topic) {
            const QString content = QDateTime::currentDateTime().toString()
                        + QLatin1String(" Received Topic: ")
                        + topic.name()
                        + QLatin1String(" Message: ")
                        + message
                        + QLatin1Char('\n');

            qDebug() << content;
        });
    connect(_mclient, &QMqttClient::stateChanged, this, &Dialog::updateLogStateChange);
    connect(_mclient, &QMqttClient::disconnected, this, &Dialog::brokerDisconnected);
    connect(_mclient, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
            const QString content = QDateTime::currentDateTime().toString()
                        + QLatin1String(" Received Topic: ")
                        + topic.name()
                        + QLatin1String(" Message: ")
                        + message
                        + QLatin1Char('\n');
            ui->editLog->insertPlainText(content);
            Switch* tSwitch = nullptr;
            tSwitch = find_switch(topic.name());
            if(tSwitch){
                tSwitch->blockSignals(true);
                int checked = Qt::CheckState::Unchecked;
                if(message == "ON")
                    checked = Qt::CheckState::Checked;
                else if(message == "OFF")
                    checked = Qt::CheckState::Unchecked;
                if(tSwitch->checkState() != checked)
                    tSwitch->setChecked(checked);
                tSwitch->blockSignals(false);
            }
        });
    _mclient->connectToHost();
}

void Dialog::getAllFeeds() {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://io.adafruit.com/api/v2/" + _cData.Username + "/feeds?X-AIO-Key=" + _cData.Password)));
}

void Dialog::generateControls() {
    vecSwitch.resize(feeds.size());
    for(size_t i = 0; i < feeds.size(); ++i) {
        vecSwitch[i] = new Switch(feeds[i].first);
        vecSwitch[i]->setLayoutDirection(Qt::RightToLeft);
        vecSwitch[i]->setAccessibleDescription(_cData.Username + "/feeds/" + feeds[i].second);
        ui->verticalLayout->addWidget(vecSwitch[i]);
        connect(vecSwitch[i], &Switch::toggled, this, [i, this](bool checked) -> void {
                    _mclient->publish(QString(_cData.Username + "/feeds/" + feeds[i].second), checked ? "ON" : "OFF");
                });
    }
    this->_recognizer = new SphinxRecognizer(MODELDIR "/zero_ru.cd_semi_4000", MODELDIR "/rus_sh_dict", MODELDIR "/zero_ru.cd_semi_4000/mdef", MODELDIR "/rus.gram");
    _recognizeThread = new QThread;
    this->_recognizer->moveToThread(_recognizeThread);
    connect(this, SIGNAL(startRecognition(string)), this->_recognizer, SLOT(startRecognition(string)));
    connect(this->_recognizer, SIGNAL(recognized(string)), this, SLOT(onRecognize(string)));
    _recognizeThread->start();
    emit this->startRecognition();
}

Switch * Dialog::find_switch(QString topic) {
    return *(std::find_if(vecSwitch.begin(),vecSwitch.end(), [&topic](Switch* s)->bool {
        return s->accessibleDescription() == topic;
    }));
}
