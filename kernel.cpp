#include "kernel.h"

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

}
Kernel::~Kernel(){
    delete authManager;
    delete logger;
    delete userManager;
}

void Kernel::setAccessToken(QString accessToken)
{
    this->accessToken = accessToken;
}

QJsonDocument Kernel::callMethodViaGet(QString method, QList< QPair<QString, QString> > parameters)
{
    parameters.append(qMakePair(QStringLiteral("access_token"), accessToken));
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
    //qDebug() << err.errorString();
    return jsonResponse;
}
