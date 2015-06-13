#include "authorizationmanager.h"
#include "kernel.h"

#include <QWebView>
#include <QUrl>
#include <QEventLoop>
#include <QRegularExpression>

#include <stdexcept>
AuthorizationManager::AuthorizationManager(Kernel *kernel, QObject *parent) :
    QObject(parent)
{
    this->kernel = kernel;
    this->userId = kernel->settingsManager.getAuthorizationParameter("user_id").toLongLong();
}

QString AuthorizationManager::getAccessToken(QString address, QString appId, QStringList accessRights, QString version)
{
// https://oauth.vk.com/authorize?
// client_id=APP_ID&
// scope=PERMISSIONS&
// redirect_uri=REDIRECT_URI&
// display=DISPLAY&
// v=API_VERSION&
// response_type=token
//    QUrl authUrl(QStringLiteral("https://oauth.vk.com/authorize?") +
//                 QStringLiteral("client_id="))
    QUrl authUrl(address + QStringLiteral("?client_id=") + appId
                 + QStringLiteral("&scope=") + accessRights.join(',')
                 + QStringLiteral("&redirect_uri=https://oauth.vk.com/blank.html")
                 + QStringLiteral("&display=page")
                 + QStringLiteral("&v=") + version
                 + QStringLiteral("&response_type=token"));
    QWebView *view = new QWebView();
    view->load(authUrl);
    view->show();

    QEventLoop pause;
    QString urlString;
    QObject::connect(view, SIGNAL(urlChanged(QUrl)), &pause, SLOT(quit()));
    for(;;){
        pause.exec();
        urlString = view->url().toString();
        if(urlString.contains(QStringLiteral("error")) || urlString.contains(QStringLiteral("access_token")))
            break;
    }

    view->close();
    delete view;

    QString token;

    QRegularExpression regexp(QStringLiteral(".*#access_token=([a-z0-9]*)&.*=(\\d*)&.*=(\\d*)"));
    QRegularExpressionMatch match = regexp.match(urlString);
    if(match.hasMatch()){
        token = match.captured(1);
        userId = match.captured(3).toInt();
    }else{
        QRegularExpression errorHandlingRegExp(QStringLiteral(".*error_description=([\\w+\\.]*)"));
        match = errorHandlingRegExp.match(urlString);
        if(match.hasMatch()){
            throw std::runtime_error(match.captured().toStdString().c_str());
        }else{
            throw std::runtime_error("Unknown error while acquiring access token");
        }
    }
    kernel->settingsManager.setAuthorizationParameter("user_id", userId);
    return token;
}

void AuthorizationManager::logout()
{

}
