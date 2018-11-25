#include "Device/include/adevice.h"
#include <QNetworkReply>
#include <QEventLoop>
#include <QObject>
QString ADevice::getValue() const {
    return _value;
}

void ADevice::setValue(QString new_value) {
    this->_value = new_value;
}


QString ADevice::getName() const {
    return _name;
}

QString ADevice::getGroup() const {
    return _group;
}

QString ADevice::getFeed() const {
    return _feed;
}

void ADevice::setTriggerPhrase(QString new_phrase) {
    this->_triggerPhrase = new_phrase;
}

QString ADevice::getTriggerPhrase() {
    return _triggerPhrase;
}


void ADevice::fromJsonObject(QJsonObject& obj) {
    _name = obj["name"].toString();
    _group = obj["group"].toString();
    _feed = obj["topic"].toString();
    _needRecognize = obj["needRecognition"].toBool();
    _triggerPhrase = obj["triggerPhrase"].toString();
    _needResponse = obj["needResponse"].toBool();
    _responsePhrase = obj["responsePhrase"].toString();

}

void ADevice::setResponsePhrase(QString rPhrase) {
    _responsePhrase = rPhrase;
}

QString ADevice::getResponsePhrase() {
    return _responsePhrase;
}

bool ADevice::needRecognize() {
    return _needRecognize;
}

bool ADevice::needResponse() {
    return _needResponse;
}

QJsonObject ADevice::toJsonObject() {
    QJsonObject jObj;
    jObj["type"] = getType();
    jObj["name"] = _name;
    jObj["group"] = _group;
    jObj["topic"] = _feed;
    jObj["triggerphrase"] = _triggerPhrase;
    jObj["responsePhrase"] = _responsePhrase;
    jObj["needRecognition"] = _needRecognize;
    jObj["needResponse"] = _needResponse;
    return jObj;
}

void ADevice::setLastValueFromUrl(QString url) {
    QEventLoop loop;
    QNetworkAccessManager manager;
    QObject::connect(&manager,SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(url)));
    loop.exec();
    qDebug() << url;
    if(reply->error() == QNetworkReply::NoError) {
        QString values(reply->readAll());
        setValue(values.split(',')[0]);
    }

    delete reply;
}

void ADevice::setMqttClient(QMqttClient* client) {
    _mclient = client;
}

QMqttClient* ADevice::getMqttClient() {
    return _mclient;
}

void ADevice::setFeedBaseUrl(QString url) {
    _feedBaseUrl = url;
}

QString ADevice::getFeedBaseUrl() {
    return _feedBaseUrl;
}
