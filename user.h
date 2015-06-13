#ifndef USER_H
#define USER_H
#include <QString>
struct User{
    unsigned long long id;
    QString firstName;
    QString lastName;
    QString photoUrl;
    bool online;
};

#endif // USER_H
