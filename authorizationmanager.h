#ifndef AUTHORIZATIONMANAGER_H
#define AUTHORIZATIONMANAGER_H

#include <QObject>
//#include "kernel.h"
class Kernel;
class AuthorizationManager : public QObject
{
    Q_OBJECT

    Kernel *kernel = NULL;

    long long userId = 0;
public:
    explicit AuthorizationManager(Kernel *kernel, QObject *parent = 0);
    QString getAccessToken(QString address, QString appId, QStringList accessRights, QString version);
    long getUserId(){return userId;}
    void logout();

signals:

public slots:

};

#endif // AUTHORIZATIONMANAGER_H
