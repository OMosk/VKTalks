#ifndef GROUPCHAT_H
#define GROUPCHAT_H
class QString;
template<class T> class QList;

struct GroupChat
{
    int id;
    QString type;
    QString title;
    int adminId;
    QList<int> users;
};

#endif // GROUPCHAT_H
