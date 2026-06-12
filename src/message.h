#pragma once
#include <QString>
#include <QDateTime>

class Message {
public:
    Message() = default;
    Message(int id, const QString &profileName, const QString &text, const QDateTime &time);

    int       id()          const { return m_id; }
    QString   profileName() const { return m_profileName; }
    QString   text()        const { return m_text; }
    QDateTime time()        const { return m_time; }

private:
    int       m_id = 0;
    QString   m_profileName;
    QString   m_text;
    QDateTime m_time;
};
