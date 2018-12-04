#ifndef ADAFRUITCONNECTIONMANAGER_H
#define ADAFRUITCONNECTIONMANAGER_H
#include "ConnectionManager/include/AdafruitConnectionManager.h"
#include <QObject>
#include <QMessageBox>
QString AdafruitConnectionManager::getBaseFeedUrl() {
    return cData.Username + "/feeds/";
}

QString AdafruitConnectionManager::getGeneralFeedUrl() {
    return cData.Username + "/feeds/+";
}

std::map<QString, std::vector<AvailableDeviceInfo>> AdafruitConnectionManager::getDevicesList() {
        std::map<QString, std::vector<AvailableDeviceInfo>> availableDevicesByGroup;
        QUrl url("http://" + cData.Host  + "/api/v2/" + cData.Username + "/feeds?X-AIO-Key=" + cData.Password);
        QNetworkAccessManager manager;
        QNetworkRequest request;
        request.setUrl(url);
        QEventLoop loop;

        QObject::connect(&manager,SIGNAL(finished(QNetworkReply*)),&loop,SLOT(quit()));

        QNetworkReply *reply = manager.get(request);
        loop.exec();

        if(reply->error() != QNetworkReply::NoError)
        {
            QMessageBox::critical(nullptr ,"Ошибка", "Невозможно получить список устройств!\nПроверьте данные для подключения!");
            return availableDevicesByGroup;
        }
        QJsonDocument jDocument = QJsonDocument::fromJson(reply->readAll());
        QJsonArray jA = jDocument.array();
        for(int i = 0; i < jA.size(); ++i) {
            QJsonObject jO = jA.at(i).toObject();
            QString group = jO.value("group").toObject().value("name").toString();
            availableDevicesByGroup[group].emplace_back(jO.value("name").toString(), jO.value("key").toString());
        }



        reply->deleteLater();
        return availableDevicesByGroup;

}

QString AdafruitConnectionManager::getLastValueUrl(ADevice* device) {
    return ("http://" + cData.Host + "/api/v2/" + cData.Username + "/feeds/" + device->getFeed() + "/data/retain/?X-AIO-Key=" + cData.Password);
}

#endif
