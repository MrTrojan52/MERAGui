#ifndef ADEVICE_H
#define ADEVICE_H
#include "IDevice.h"
#include <QJsonObject>
#include <QLayout>
#include <QNetworkAccessManager>
#include <QtMqtt>
class ADevice : public IDevice {
private:
    QString _name;
    QString _value;
    QString _group;
    QString _feed;
    bool _needRecognize;
    bool _needResponse;
    QString _triggerPhrase;
    QString _responsePhrase;
    QMqttClient* _mclient = nullptr;
    QString _feedBaseUrl;
public:
    //ADevice(QString name, QString value, QString group, QString feed):_name(name), _value(value), _group(group), _feed(feed) {}
    ADevice(QJsonObject obj) {
        fromJsonObject(obj);
    }

    virtual QString getValue() const override;
    virtual QString getName() const;
    virtual QString getGroup() const;
    virtual QString getFeed() const;
    virtual void setTriggerPhrase(QString new_phrase);
    virtual QString getTriggerPhrase();
    virtual void checkTrigger(QString tPhrase) = 0;
    virtual void setResponsePhrase(QString rPhrase);
    virtual QString getResponsePhrase();
    virtual void setValue(QString new_value) override;
    virtual void fromJsonObject(QJsonObject& obj);
    virtual QJsonObject toJsonObject();
    virtual QString getType() = 0;
    virtual void insertWidgetsIntoLayout(QLayout* layout) = 0;
    virtual void setLastValueFromUrl(QString url);
    virtual void setMqttClient(QMqttClient* client);
    virtual QMqttClient* getMqttClient();
    virtual void setFeedBaseUrl(QString url);
    virtual QString getFeedBaseUrl();
    bool needRecognize();
    bool needResponse();
};

#endif // ADEVICE_H
