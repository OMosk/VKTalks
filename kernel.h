#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QMessageLogger>

#include "customtypes.h"
#include "settingsmanager.h"
#include "authorizationmanager.h"
#include "usermanager.h"
class Kernel : public QObject
{
    Q_OBJECT
    QString accessToken;

    QNetworkAccessManager *networkAccessManager = NULL;

    QMessageLogger *logger;
public:
    explicit Kernel(QObject *parent = 0);
    ~Kernel();
    void setAccessToken(QString accessToken);
    QJsonDocument callMethodViaGet(QString method, QList< QPair<QString, QString> > parameters);

    SettingsManager settingsManager;
    AuthorizationManager *authManager;
    UserManager *userManager;

signals:

public slots:

};

#endif // KERNEL_H
