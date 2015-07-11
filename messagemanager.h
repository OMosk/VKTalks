#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H
class QString;
class Kernel;
struct Message;
struct Attachment;
template<class T> class QList;

class MessageManager
{
    Kernel *kernel;
    QList<Message> getHistory(long long userId, long long chatId, int count, int offset, long long startMessageId);
public:
    MessageManager(Kernel *kernel);

    QList<Message> getMessageByIds(QList<long long> messageIds);

    QList<Message> getDialogs(int count, int offset, bool unread, long long userId);
    QList<Message> getDialogHistory(long long userId, int count, int offset);

    QList<Message> getGroupChatHistory(long long chatId, int count, int offset);

    long long send(long long userId, QString domain, long long chatId, QList<long long> userIds, QString message,
                   QList<Attachment> attachments, QList<long long> forwardedMessages);
//    QList<Message> getHistory()
};

#endif // MESSAGEMANAGER_H
