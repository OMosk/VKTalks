#ifndef MESSAGE_H
#define MESSAGE_H
/*class QDateTime;
class QString;
template<class T> class QList;
*/
#include <QDateTime>
#include <QString>
#include <QList>

//struct Attachment;//Custom base class for all kind of vk attachments;
#include "attachment.h"

struct Message
{
    long long id = 0;
    long long userDialogId = 0;
    long chatId = 0;// for multidialog
    QList<long long> chatActiveIds;//for multidialog
    long long fromUserId = 0;
    QDateTime date;
    bool readState;
    bool out;
    QString title;
    QString body;
    QList<Attachment> attachments;
    QList<Message> forwardedMessages;
};

#endif // MESSAGE_H
