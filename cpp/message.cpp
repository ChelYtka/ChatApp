#include "src/message.h"

Message::Message(int id, const QString &profileName, const QString &text, const QDateTime &time)
    : m_id(id), m_profileName(profileName), m_text(text), m_time(time)
{}
