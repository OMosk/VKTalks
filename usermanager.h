#ifndef USERMANAGER_H
#define USERMANAGER_H
#include <QList>

#include "user.h"
class Kernel;
class UserManager
{
    Kernel *kernel;
public:
    UserManager(Kernel *kernel);
    QList<User> getFriends(long long userId);
    QList<int> getFriendsOnline(long long userId);
};

#endif // USERMANAGER_H
