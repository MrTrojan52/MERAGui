#include "dialog.h"
#include "ui_dialog.h"
#define MODELDIR "/home/trojan52/Qt/Projects/MERAGui/rus_model"
#include <QMessageBox>
#define IO_KEY "36d27d262cda42cf8e357bb722795f72"
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
    _mclient = new QMqttClient();
    _mclient->setHostname("io.adafruit.com");
    _mclient->setPort(1883);
    _mclient->setUsername("Trojan52");
    _mclient->setPassword("36d27d262cda42cf8e357bb722795f72");
    connect(_mclient, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
            const QString content = QDateTime::currentDateTime().toString()
                        + QLatin1String(" Received Topic: ")
                        + topic.name()
//                        + QLatin1String(" Message: ")
//                        + message
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
        });

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
//    this->_recognizer = new SphinxRecognizer(MODELDIR "/zero_ru.cd_semi_4000", MODELDIR "/rus_sh_dict", MODELDIR "/zero_ru.cd_semi_4000/mdef", MODELDIR "/rus.gram");
//    connect(this->_recognizer, SIGNAL(recognized(string)), this, SLOT(onRecognize(string)));
//    this->_recognizer->recognize_from_microphone();
    //_mclient->connectToHost();
//    _mclient->publish(QString("Trojan52/feeds/kitchen.lamp-on-the-kitchen-1"), "OFF");


}


void Dialog::onRecognize(string text) {
    qDebug() << text.c_str();
}

void Dialog::brokerDisconnected() {
    qDebug() << "BROKER DISCONNECTED";
}

void Dialog::updateLogStateChange() {
    qDebug() << "State changed to " << _mclient->state();
    if(_mclient->state() == QMqttClient::Connected) {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
        manager->get(QNetworkRequest(QUrl("https://io.adafruit.com/api/v2/Trojan52/feeds?X-AIO-Key=" IO_KEY)));
        //_mclient->publish(QString("Trojan52/feeds/kitchen.lampa-na-kukhnie-1"), "OFF");
//        auto subscription = _mclient->subscribe(QString("Trojan52/feeds/+"));
//        if (!subscription) {
//                QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
//                return;
//            }
    }
}

void Dialog::replyFinished(QNetworkReply* reply) {
    QJsonDocument jDocument = QJsonDocument::fromJson(reply->readAll());
    QJsonArray jA = jDocument.array();
    for(int i = 0; i < jA.size(); ++i) {
        QJsonObject jO = jA.at(i).toObject();
        ui->editLog->insertPlainText(jO.value("name").toString() + '\n');
        ui->editLog->insertPlainText(jO.value("key").toString() + '\n');
    }
    reply->deleteLater();
}

void Dialog::connectionDialogAccepted() {
    this->_cData = cdlg->getData();
}

void Dialog::connectionDialogRejected() {
    cdlg->show();
}
