#include "Device/include/adevice.h"
#include <QNetworkReply>
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
    _manager = new QNetworkAccessManager;
    connect(_manager, &QNetworkAccessManager::finished, this, [this](QNetworkReply* reply) {
        if(reply->error() == QNetworkReply::NoError)
        {
            QString values(reply->readAll());
            this->setValueFromBack(values.split(',')[0]);
            delete reply;
        }
    });
    _manager->get(QNetworkRequest(QUrl(url)));
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

QString ADevice::resolveVariables(QString phrase) {
    phrase.replace("#name", getName());
    phrase.replace("#value", getValue());
    phrase.replace("#group", getGroup());
    phrase.replace("#feed", getFeed());
    return phrase;
}

void ADevice::setValueFromBack(QString val) {
    setValue(val);
}

void ADevice::setDeleteAction(QAction* action) {
   if(delete_action)
       delete_action->deleteLater();
   delete_action = action;
}

QAction* ADevice::getDeleteAction() {
    return delete_action;
}

void ADevice::setTTSEngine(QString engine) {
    if(tts)
        delete tts;
    tts = new QTextToSpeech(engine);

}

void ADevice::setTTSVoice(QVoice voice) {
    if(tts)
        tts->setVoice(voice);
}

void ADevice::say(QString sentence) {
    tts->say(sentence);
}
