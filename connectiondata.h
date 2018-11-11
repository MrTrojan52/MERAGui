#ifndef CONNECTIONDATA_H
#define CONNECTIONDATA_H
#include <QString>
struct ConnectionData {
    QString Host;
    QString Port;
    QString Username;
    QString Password;
    ConnectionData() = default;
    ConnectionData(QString host, QString port, QString user, QString pass ): Host(host), Port(port), Username(user), Password(pass) {}
};

#endif // CONNECTIONDATA_H
