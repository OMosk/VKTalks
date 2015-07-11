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
    QList<User> getUsers(QList<int> userIds);
    QList<User> searchUsers(QString q, int count = 0, int offset = 0);
    QList<int> getFriendsRequests(bool out = false);
    bool addFriend(int userId);
    bool deleteFriend(int userId);
};

#endif // USERMANAGER_H
