#include "Device/include/adevice.h"

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
