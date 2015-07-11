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
//    qDebug() << QString(response.toJson());
    return result;
}
QList<User> UserManager::getUsers(QList<int> userIds)
{
    QString methodName("users.get");

    QString userIdsString;
    for(auto it = userIds.begin(); it != userIds.end(); ++it){
        userIdsString += QString::number(*it);
        if(it!=userIds.end()-1)
            userIdsString += ',';
    }

    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("user_ids"), userIdsString));
    parameters.append(qMakePair(QStringLiteral("fields"), QStringLiteral("id,first_name,last_name,photo_100,online")));

    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    QJsonArray usersJsonArray = response.object().value(QStringLiteral("response")).toArray();

    QList<User> result;
    for(QJsonValueRef userJsonValue : usersJsonArray){
        QJsonObject userJsonObject = userJsonValue.toObject();
        User user;
        user.id = userJsonObject.value("uid").toVariant().toLongLong();
        user.firstName = userJsonObject.value("first_name").toString();
        user.lastName = userJsonObject.value("last_name").toString();
        user.photoUrl = userJsonObject.value("photo_100").toString();
        user.online = static_cast<bool>(userJsonObject.value("online").toInt());

        result.append(user);
    }
//    qDebug() << QString(response.toJson());
    return result;

}
QList<User> UserManager::searchUsers(QString q, int count, int offset)
{
    QString methodName("users.search");

    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("q"), q));

    if(count > 0)
        parameters.append(qMakePair(QStringLiteral("count"), QString::number(count)));
    if(offset > 0)
        parameters.append(qMakePair(QStringLiteral("q"), QString::number(offset)));

    parameters.append(qMakePair(QStringLiteral("fields"), QStringLiteral("id,first_name,last_name,photo_100,online")));

    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    QJsonArray usersJsonArray = response.object().value(QStringLiteral("response")).toArray();

//    int resultsFound = usersJsonArray.first().toInt();
    usersJsonArray.removeFirst();

    QList<User> result;
    for(QJsonValueRef userJsonValue : usersJsonArray){
        QJsonObject userJsonObject = userJsonValue.toObject();
        User user;
        user.id = userJsonObject.value("uid").toVariant().toLongLong();
        user.firstName = userJsonObject.value("first_name").toString();
        user.lastName = userJsonObject.value("last_name").toString();
        user.photoUrl = userJsonObject.value("photo_100").toString();
        user.online = static_cast<bool>(userJsonObject.value("online").toInt());

        result.append(user);
    }
//    qDebug() << QString(response.toJson());
    return result;

}
QList<int> UserManager::getFriendsRequests(bool out)
{
    QString methodName("friends.getRequests");
    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("out"), QString::number(static_cast<int>(out))));

    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    QJsonArray friendsOnlineJsonArray = response.object().value(QStringLiteral("response")).toObject().value(QStringLiteral("items")).toArray();

    QList<int> result;
    for(QJsonValueRef friendOnline : friendsOnlineJsonArray)
        result.append(friendOnline.toInt());
//    qDebug() << QString(response.toJson());
    return result;
}

bool UserManager::addFriend(int userId)
{
    QString methodName("friends.add");
    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("user_id"), QString::number(userId)));

    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    int statusCode = response.object().value(QStringLiteral("response")).toInt();

    if(statusCode == 1 || statusCode == 2 || statusCode == 4)
        return true;
    else
        return false;
//    qDebug() << QString(response.toJson());
}

bool UserManager::deleteFriend(int userId)
{
    QString methodName("friends.delete");
    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("user_id"), QString::number(userId)));

    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    int statusCode = response.object().value(QStringLiteral("response")).toObject().value(QStringLiteral("success")).toInt();

    return statusCode;
}
