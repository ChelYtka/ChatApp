#include "src/chat.h"
#include <QDateTime>

Chat::Chat(int id, const QString &name,
           const QString &participant1, const QString &participant2)
    : m_id(id), m_name(name),
      m_participant1(participant1), m_participant2(participant2)
{}

bool Chat::hasParticipant(const QString &name) const
{
    return m_participant1 == name || m_participant2 == name;
}

int Chat::addMessage(const QString &profileName, const QString &text)
{
    int newId = m_nextMsgId;
    for (const auto &m : m_messages)
        if (m.id() >= newId) newId = m.id() + 1;
    m_nextMsgId = newId + 1;

    m_messages.append(Message(newId, profileName, text, QDateTime::currentDateTime()));
    return newId;
}

bool Chat::removeMessage(int msgId)
{
    for (int i = 0; i < m_messages.size(); ++i) {
        if (m_messages[i].id() == msgId) {
            m_messages.removeAt(i);
            return true;
        }
    }
    return false;
}
