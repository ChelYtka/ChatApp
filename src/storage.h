#pragma once
#include <QList>
#include "profile.h"
#include "chat.h"

class Storage {
public:
    static void save(const QList<Profile> &profiles, const QList<Chat> &chats);
    static void load(QList<Profile> &profiles, QList<Chat> &chats);
private:
    static QString filePath();
};
