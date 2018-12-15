#include "Factories/include/devicefactory.h"
#include "Device/include/switchdevice.h"
#include "Device/include/infodevice.h"
#include "Device/include/editdevice.h"

ADevice* DeviceFactory::create(QJsonObject& obj) {
    if(obj.find("type") != obj.end())
    {
        if(obj["type"] == "Переключаемое")
            return new SwitchDevice(obj);
        else if(obj["type"] == "Информационное")
            return new InfoDevice(obj);
        else if(obj["type"] == "Редактируемое")
            return new EditDevice(obj);
        else
            return nullptr;
    } else {
        return nullptr;
    }
}
