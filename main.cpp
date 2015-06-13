#include "mainwindow.h"
#include "kernel.h"
#include "user.h"
//#include "authorizationmanager.h"
#include <QApplication>
#include <QDebug>
#include <QList>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kernel *kernel = new Kernel(&a);
    MainWindow w;
    w.show();

    auto list = kernel->userManager->getFriendsOnline(kernel->authManager->getUserId());
    for(int &id : list)
        qDebug() << id;
//    auto friends = kernel->userManager->getFriends();
//    for(const auto &_friend: friends){
//        qDebug() << _friend.id << _friend.firstName << _friend.lastName << _friend.photoUrl << _friend.online;
//    }
    return a.exec();
}
