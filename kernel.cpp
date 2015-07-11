#include "kernel.h"

#include "settingsmanager.h"
#include "authorizationmanager.h"
#include "usermanager.h"
#include "messagemanager.h"
#include "longpollmanager.h"


#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QEventLoop>

#include <stdexcept>
Kernel::Kernel(QObject *parent) :
    QObject(parent)
{
    networkAccessManager = new QNetworkAccessManager(this);
    authManager = new AuthorizationManager(this);
    userManager = new UserManager(this);
    messageManager = new MessageManager(this);

    logger = new QMessageLogger("kernel.log", 17, "Kernel::Kernel");
    logger->debug("App started");

    this->accessToken = settingsManager.getAuthorizationParameter("access_token").toString();
    if(accessToken.isNull() || accessToken.isEmpty()){
        try{
            this->accessToken = authManager->getAccessToken("https://oauth.vk.com/authorize", "4732380", QStringLiteral("friends,messages,offline").split(','), "5.34");
            settingsManager.setAuthorizationParameter("access_token", accessToken);
        }
        catch(std::runtime_error e){
            logger->fatal(e.what());
        }
    }

    longPollManager = new LongPollManager(this, true);
}
Kernel::~Kernel(){
    delete authManager;
    delete logger;
    delete userManager;
    delete messageManager;
    delete longPollManager;
}

void Kernel::setAccessToken(QString accessToken)
{
    this->accessToken = accessToken;
}

QJsonDocument Kernel::callMethodViaGet(QString method, QList< QPair<QString, QString> > parameters)
{
    parameters.append(qMakePair(QStringLiteral("access_token"), accessToken));
    parameters.append(qMakePair(QStringLiteral("v"), QStringLiteral("5.34")));
//    QUrl parametersUrl;
//    parametersUrl.setQuery(QUrlQuery(parameters));
    QUrl url;
    url.setScheme(QStringLiteral("https"));
    url.setHost(QStringLiteral("api.vk.com"));
    url.setPath(QStringLiteral("/method/") + method);

    QUrlQuery query;
    query.setQueryItems(parameters);

    url.setQuery(query);
    qDebug() << url.toString();
    QNetworkRequest request(url);
//    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    QNetworkReply *reply;
    reply = networkAccessManager->get(request);
    QEventLoop pause;
    QObject::connect(reply, SIGNAL(finished()), &pause, SLOT(quit()));
    pause.exec();
    //qDebug() << QString(reply->readAll());
    QJsonParseError err;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll()/*, &err*/);

    qDebug() << QString(jsonResponse.toJson());
    //qDebug() << err.errorString();
    return jsonResponse;
}

QJsonDocument Kernel::loadUrl(QString urlString)
{
    QUrl url(urlString);
    QNetworkRequest request(url);
    QNetworkReply *reply = networkAccessManager->get(request);
    QEventLoop pause;
    QObject::connect(reply, SIGNAL(finished()), &pause, SLOT(quit()));
    pause.exec();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
    return jsonResponse;
}
