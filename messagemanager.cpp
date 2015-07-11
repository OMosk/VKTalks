#include "messagemanager.h"

#include <QList>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDateTime>

#include "message.h"
#include "kernel.h"
#include "attachment.h"

MessageManager::MessageManager(Kernel *kernel)
{
    this->kernel = kernel;
}

QList<Message> MessageManager::getMessageByIds(QList<long long> messageIds)
{
    QString methodName("messages.getById");

    QString messageIdsString;
    for(auto it = messageIds.begin(); it != messageIds.end(); ++it){
        messageIdsString += QString::number(*it);
        if(it+1 != messageIds.end()) messageIdsString += ',';
    }

    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("message_ids"), messageIdsString));


    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    QJsonArray messagesJsonArray = response.object().value(QStringLiteral("response")).toObject().value(QStringLiteral("items")).toArray();

    QList<Message> result;
    for(QJsonValueRef messageJsonValue : messagesJsonArray){
        QJsonObject messageJsonObject = messageJsonValue.toObject().value(QStringLiteral("message")).toObject();

        Message message;
        message.id = messageJsonObject.value("id").toDouble();
        message.date = QDateTime::fromMSecsSinceEpoch((qint64) messageJsonObject.value("date").toDouble());
        message.out = static_cast<bool>(messageJsonObject.value("out").toInt());
        message.readState = static_cast<bool>(messageJsonObject.value("read_state").toInt());
        message.userDialogId = messageJsonObject.value("user_id").toDouble();
        message.body = messageJsonObject.value("body").toString();
        message.title = messageJsonObject.value("title").toString();

        message.chatId = messageJsonObject.value("chad_id").toDouble();

        QList<long long> chatActiveUsersList;
        QJsonArray chatActiveUsers = messageJsonObject.value("chat_active").toArray();
        for(QJsonValueRef activeUser: chatActiveUsers)
            chatActiveUsersList.append(activeUser.toDouble());

        message.chatActiveIds = chatActiveUsersList;

        result.append(message);
    }
//    qDebug() << QString(response.toJson());
    return result;

}


QList<Message> MessageManager::getDialogs(int count, int offset, bool unread, long long userId)
{
    QString methodName("messages.getDialogs");


    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("count"), QString::number(count)));
    parameters.append(qMakePair(QStringLiteral("offset"), QString::number(offset)));
    parameters.append(qMakePair(QStringLiteral("unread"), QString::number(static_cast<int>(unread))));
    if(userId > 0) parameters.append(qMakePair(QStringLiteral("user_id"), QString::number(userId)));

    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    QJsonArray messagesJsonArray = response.object().value(QStringLiteral("response")).toObject().value(QStringLiteral("items")).toArray();

    QList<Message> result;
    for(QJsonValueRef messageJsonValue : messagesJsonArray){
        QJsonObject messageJsonObject = messageJsonValue.toObject().value(QStringLiteral("message")).toObject();

        Message message;
        message.id = messageJsonObject.value("id").toDouble();
        message.date = QDateTime::fromMSecsSinceEpoch((qint64) messageJsonObject.value("date").toDouble());
        message.out = static_cast<bool>(messageJsonObject.value("out").toInt());
        message.readState = static_cast<bool>(messageJsonObject.value("read_state").toInt());
        message.userDialogId = messageJsonObject.value("user_id").toDouble();
        message.body = messageJsonObject.value("body").toString();
        message.title = messageJsonObject.value("title").toString();

        message.chatId = messageJsonObject.value("chad_id").toDouble();

        QList<long long> chatActiveUsersList;
        QJsonArray chatActiveUsers = messageJsonObject.value("chat_active").toArray();
        for(QJsonValueRef activeUser: chatActiveUsers)
            chatActiveUsersList.append(activeUser.toDouble());

        message.chatActiveIds = chatActiveUsersList;

        result.append(message);
    }
//    qDebug() << QString(response.toJson());
    return result;

}

QList<Message> MessageManager::getDialogHistory(long long userId, int count, int offset)
{
    return getHistory(userId, 0, count, offset, 0);
}

QList<Message> MessageManager::getGroupChatHistory(long long chatId, int count, int offset)
{
    return getHistory(0, chatId, count, offset, 0);
}

long long MessageManager::send(long long userId, QString domain, long long chatId, QList<long long> userIds, QString message, QList<Attachment> attachments, QList<long long> forwardedMessages)
{

    QString methodName("messages.send");

    QList<QPair<QString, QString> > parameters;
    if(userId > 0)
        parameters.append(qMakePair(QStringLiteral("user_id"), QString::number(userId)));
    else if(!domain.isEmpty())
        parameters.append(qMakePair(QStringLiteral("domain"), domain));
    else if(chatId > 0)
        parameters.append(qMakePair(QStringLiteral("domain"), domain));
    else if(!userIds.isEmpty()){
        QString userIdsString;
        for(auto it = userIds.begin(); it!=userIds.end(); ++it){
            userIdsString += QString::number(*it);
            if(it+1 != userIds.end()) userIdsString += ',';
        }
        parameters.append(qMakePair(QStringLiteral("user_ids"), userIdsString));
    }
    else throw std::invalid_argument("Wrong receiver arguments");

    if(message.isEmpty() && attachments.isEmpty() && forwardedMessages.isEmpty()) throw std::invalid_argument("Text can't be empty if there is no attachments");
    else{
        parameters.append(qMakePair(QStringLiteral("message"), message));

        QString attachmentsString;
        for(auto it = attachments.begin(); it!=attachments.end(); ++it){
            attachmentsString += it->getLink();
            if(it+1!=attachments.end()) attachmentsString += ',';
        }
        if(!attachmentsString.isEmpty())
            parameters.append(qMakePair(QStringLiteral("attachments"), attachmentsString));

        QString forwardedMessagesString;
        for(auto it = forwardedMessages.begin(); it != forwardedMessages.end(); ++it){
            forwardedMessagesString += QString::number(*it);
            if(it+1 != forwardedMessages.end()) forwardedMessagesString += ',';
        }
        if(!forwardedMessagesString.isEmpty())
            parameters.append(qMakePair(QStringLiteral("forward_messages"), forwardedMessagesString));
    }

    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    return static_cast<long long>(response.object().value(QStringLiteral("response")).toDouble());
}

QList<Message> MessageManager::getHistory(long long userId, long long chatId, int count, int offset, long long startMessageId)
{

    QString methodName("messages.getHistory");

    QList<QPair<QString, QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("count"), QString::number(count)));
    parameters.append(qMakePair(QStringLiteral("offset"), QString::number(offset)));

    if(startMessageId > 0) parameters.append(qMakePair(QStringLiteral("start_message_id"), QString::number(startMessageId)));

    if(userId > 0) parameters.append(qMakePair(QStringLiteral("user_id"), QString::number(userId)));
    if(chatId > 0) parameters.append(qMakePair(QStringLiteral("chat_id"), QString::number(chatId)));

    QJsonDocument response = kernel->callMethodViaGet(methodName, parameters);
    QJsonArray messagesJsonArray = response.object().value(QStringLiteral("response")).toObject().value(QStringLiteral("items")).toArray();

    QList<Message> result;
    for(QJsonValueRef messageJsonValue : messagesJsonArray){
        QJsonObject messageJsonObject = messageJsonValue.toObject();

        Message message;
        message.id = messageJsonObject.value("id").toDouble();
        message.date = QDateTime::fromMSecsSinceEpoch((qint64) messageJsonObject.value("date").toDouble());
        message.out = static_cast<bool>(messageJsonObject.value("out").toInt());
        message.readState = static_cast<bool>(messageJsonObject.value("read_state").toInt());
        message.userDialogId = messageJsonObject.value("user_id").toDouble();
        message.body = messageJsonObject.value("body").toString();
        message.title = messageJsonObject.value("title").toString();

        message.chatId = messageJsonObject.value("chad_id").toDouble();

        QList<long long> chatActiveUsersList;
        QJsonArray chatActiveUsers = messageJsonObject.value("chat_active").toArray();
        for(QJsonValueRef activeUser: chatActiveUsers)
            chatActiveUsersList.append(activeUser.toDouble());

        message.chatActiveIds = chatActiveUsersList;

        result.append(message);
    }
//    qDebug() << QString(response.toJson());
    return result;
}
