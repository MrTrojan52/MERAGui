#ifndef ADEVICE_H
#define ADEVICE_H
#include "IDevice.h"
class ADevice : public IDevice {
private:
    QString _name;
    QString _value;
    QString _group;
    QString _feed;
    QString _triggerPhrase;
public:
    ADevice(QString name, QString value, QString group, QString feed):_name(name), _value(value), _group(group), _feed(feed) {}
    virtual QString getValue() const override;
    virtual QString getName() const;
    virtual QString getGroup() const;
    virtual QString getFeed() const;
    virtual void setTriggerPhrase(QString new_phrase);
    virtual void setValue(QString new_value) override;

};

#endif // ADEVICE_H
