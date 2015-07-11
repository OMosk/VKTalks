#include "longpollmanager.h"
#include "kernel.h"

#include <thread>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QPair>
#include <QDebug>

LongPollManager::LongPollManager(Kernel *kernel, bool useSsl) :
    QObject(kernel)
{
    this->kernel = kernel;
    this->networkAccessManager = kernel->getNetworkAccessManager();

    QList<QPair<QString,QString> > parameters;
    parameters.append(qMakePair(QStringLiteral("use_ssl"), QString::number(useSsl)));
    QJsonObject serverData = kernel->callMethodViaGet(QStringLiteral("messages.getLongPollServer"), parameters)
                                                        .object().value("response").toObject();

    key = serverData.value(QStringLiteral("key")).toString();
    server = serverData.value(QStringLiteral("server")).toString();
    ts = serverData.value(QStringLiteral("ts")).toDouble();

    url = QUrl(QStringLiteral("https://") + server +
                  QStringLiteral("?act=a_check&key=") + key +
                  QStringLiteral("&ts=") + QString::number(ts) +
                  QStringLiteral("&wait=5&mode=2"));
    request = QNetworkRequest(url);
    qDebug() << url.toString();
}
LongPollManager::~LongPollManager()
{
    runLoop = false;
    if (reply) {
        reply->abort();
    }

}

void LongPollManager::runLongPollLoop()
{
    reply = networkAccessManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &LongPollManager::longPollFinished);
}

void LongPollManager::longPollLoop()
{
}
void LongPollManager::longPollFinished(){
    if(reply && runLoop){
        QByteArray rawOutput = reply->readAll();
        QJsonObject response = QJsonDocument::fromJson(rawOutput).object();
        ts = response.value(QStringLiteral("ts")).toDouble();

        url = QUrl(QStringLiteral("https://") + server +
                  QStringLiteral("?act=a_check&key=") + key +
                  QStringLiteral("&ts=") + QString::number(ts) +
                  QStringLiteral("&wait=5&mode=2"));
        request = QNetworkRequest(url);

        qDebug() << QString(QJsonDocument::fromJson(rawOutput).toJson());
        reply = networkAccessManager->get(request);
        connect(reply, &QNetworkReply::finished, this, &LongPollManager::longPollFinished);
    }
}
