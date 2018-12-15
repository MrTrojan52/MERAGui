#ifndef IFACTORY_H
#define IFACTORY_H
#include "Device/include/adevice.h"

class IFactory {
public:
    virtual ADevice* create(QJsonObject& obj) = 0;
    virtual ~IFactory() = default;
};


#endif
