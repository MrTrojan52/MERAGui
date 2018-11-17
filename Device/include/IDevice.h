#ifndef IDEVICE_H
#define IDEVICE_H
#include <QString>

class IDevice {
public:
    virtual QString getValue() const = 0;
    virtual void setValue(QString val) = 0;
    virtual ~IDevice() = default;
};

#endif
