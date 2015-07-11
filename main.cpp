#include "mainwindow.h"
#include "kernel.h"
#include "user.h"
#include "message.h"
#include "messagemanager.h"
#include "authorizationmanager.h"
#include "longpollmanager.h"
#include "attachment.h"

#include <QApplication>
//#include <qapplication.h>
#include <QDebug>
#include <QList>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kernel *kernel = new Kernel(&a);
    MainWindow w;
    w.show();




    kernel->longPollManager->runLongPollLoop();
//    QList<Message> dialogs = kernel->messageManager->getDialogHistory(kernel->authManager->getUserId(), 5, 0);//kernel->messageManager->getDialogs(10, 0, 0, 0);

//    for(const Message &message: dialogs)
//        qDebug() << message.body << message.out;
//    qDebug() << kernel->messageManager->send(kernel->authManager->getUserId(), QString(), 0, QList<long long>(), "Test message", QList<Attachment>(), QList<long long>());





    return a.exec();
}
