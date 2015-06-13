#include "usermanager.h"
#include "kernel.h"
#include "user.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QList>
#include <QString>
#include <QDebug>
UserManager::UserManager(Kernel *kernel)
{
    this->kernel = kernel;
}
QList<User> UserManager::getFriends(long long userId)
{
    QString methodName("friends.get");
    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("user_id"), QString::number(userId)));
    parameters.append(qMakePair(QStringLiteral("order"), QStringLiteral("name")));
    parameters.append(qMakePair(QStringLiteral("fields"), QStringLiteral("id,first_name,last_name,photo_100,online")));

    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    QJsonArray friendsJsonArray = response.object().value(QStringLiteral("response")).toArray();

    QList<User> result;
    for(QJsonValueRef friendJsonValue : friendsJsonArray){
        QJsonObject friendJsonObject = friendJsonValue.toObject();
        User user;
        user.id = friendJsonObject.value("user_id").toVariant().toLongLong();
        user.firstName = friendJsonObject.value("first_name").toString();
        user.lastName = friendJsonObject.value("last_name").toString();
        user.photoUrl = friendJsonObject.value("photo_100").toString();
        user.online = static_cast<bool>(friendJsonObject.value("online").toInt());

        result.append(user);
    }
//    qDebug() << QString(response.toJson());
    return result;
}
QList<int> UserManager::getFriendsOnline(long long userId){
    QString methodName("friends.getOnline");
    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("user_id"), QString::number(userId)));
    parameters.append(qMakePair(QStringLiteral("order"), QStringLiteral("hint")));

    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    QJsonArray friendsOnlineJsonArray = response.object().value(QStringLiteral("response")).toArray();

    QList<int> result;
    for(QJsonValueRef friendOnline : friendsOnlineJsonArray)
        result.append(friendOnline.toInt());
    qDebug() << QString(response.toJson());
    return result;
}
