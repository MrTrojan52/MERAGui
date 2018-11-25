#ifndef IDEVICE_H
#define IDEVICE_H
#include <QString>
#include <QObject>
class IDevice:public QObject {

public:
    virtual QString getValue() const = 0;
    virtual void setValue(QString val) = 0;
    virtual ~IDevice() = default;
};

#endif
