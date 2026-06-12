#pragma once
#include <QString>
#include <QList>
#include "message.h"

class Chat {
public:
    Chat() = default;
    Chat(int id, const QString &name,
         const QString &participant1, const QString &participant2);

    int     id()           const { return m_id; }
    QString name()         const { return m_name; }
    QString participant1() const { return m_participant1; }
    QString participant2() const { return m_participant2; }

    QList<Message>       &messages()       { return m_messages; }
    const QList<Message> &messages() const { return m_messages; }

    bool hasParticipant(const QString &name) const;
    int  addMessage(const QString &profileName, const QString &text);
    bool removeMessage(int msgId);

private:
    int            m_id           = 0;
    QString        m_name;
    QString        m_participant1;
    QString        m_participant2;
    QList<Message> m_messages;
    int            m_nextMsgId    = 1;
};
