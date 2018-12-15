#ifndef AVAILABLEDEVICEINFO_H
#define AVAILABLEDEVICEINFO_H
#include <QString>
class AvailableDeviceInfo {
private:
    QString m_name;
    QString m_feed;
public:
    AvailableDeviceInfo(QString name, QString feed):m_name(name), m_feed(feed) {}
    void setName(QString name) {
        m_name = name;
    }
    QString getName() {
        return m_name;
    }

    void setFeed(QString feed) {
        m_feed = feed;
    }

    QString getFeed() {
        return m_feed;
    }
};

#endif // AVAILABLEDEVICEINFO_H
