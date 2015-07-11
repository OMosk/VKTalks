#ifndef LONGPOLLMANAGER_H
#define LONGPOLLMANAGER_H
#include <thread>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "message.h"


class Kernel;
namespace std { class thread; }

class LongPollManager : public QObject
{
    Q_OBJECT
    Kernel *kernel;
    QNetworkAccessManager *networkAccessManager;
    QUrl url;
    QNetworkRequest request;
    QNetworkReply *reply;


    QString server;
    QString key;
    qint64 ts;
    //bool runLoop = true;
    int runLoop = 5;

    void longPollLoop();
public:
    explicit LongPollManager(Kernel *kernel, bool useSsl);
    ~LongPollManager();
    void runLongPollLoop();

signals:
    void friendOnline(bool enteredOnline);
    void receivedMessage(Message message);

public slots:
    void longPollFinished();


};

#endif // LONGPOLLMANAGER_H
