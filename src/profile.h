#pragma once
#include <QString>

class Profile {
public:
    Profile() = default;
    Profile(int id, const QString &name);

    int     id()   const { return m_id; }
    QString name() const { return m_name; }

private:
    int     m_id = 0;
    QString m_name;
};
