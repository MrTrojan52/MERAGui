#ifndef DEVICEFACTORY_H
#define DEVICEFACTORY_H
#include "IFactory.h"
class DeviceFactory : public IFactory {
public:
    ADevice* create(QJsonObject& obj) override;
};

#endif // DEVICEFACTORY_H
