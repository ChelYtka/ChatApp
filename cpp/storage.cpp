#include "src/storage.h"
#include <QFile>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QString Storage::filePath()
{
    return QCoreApplication::applicationDirPath() + "/chat_data.json";
}

void Storage::save(const QList<Profile> &profiles, const QList<Chat> &chats)
{
    QJsonArray profilesArr;
    for (const auto &p : profiles) {
        QJsonObject o;
        o["id"]   = p.id();
        o["name"] = p.name();
        profilesArr.append(o);
    }

    QJsonArray chatsArr;
    for (const auto &c : chats) {
        QJsonObject co;
        co["id"]           = c.id();
        co["name"]         = c.name();
        co["participant1"] = c.participant1();
        co["participant2"] = c.participant2();

        QJsonArray msgsArr;
        for (const auto &m : c.messages()) {
            QJsonObject mo;
            mo["id"]          = m.id();
            mo["profileName"] = m.profileName();
            mo["text"]        = m.text();
            mo["time"]        = m.time().toString(Qt::ISODate);
            msgsArr.append(mo);
        }
        co["messages"] = msgsArr;
        chatsArr.append(co);
    }

    QJsonObject root;
    root["profiles"] = profilesArr;
    root["chats"]    = chatsArr;

    QFile file(filePath());
    if (file.open(QIODevice::WriteOnly))
        file.write(QJsonDocument(root).toJson());
}

void Storage::load(QList<Profile> &profiles, QList<Chat> &chats)
{
    profiles.clear();
    chats.clear();

    QFile file(filePath());
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) return;

    QJsonObject root = doc.object();

    for (const auto &v : root["profiles"].toArray()) {
        QJsonObject o = v.toObject();
        profiles.append(Profile(o["id"].toInt(), o["name"].toString()));
    }

    for (const auto &v : root["chats"].toArray()) {
        QJsonObject co = v.toObject();
        Chat chat(co["id"].toInt(), co["name"].toString(),
                  co["participant1"].toString(), co["participant2"].toString());
        for (const auto &mv : co["messages"].toArray()) {
            QJsonObject mo = mv.toObject();
            chat.messages().append(Message(
                mo["id"].toInt(),
                mo["profileName"].toString(),
                mo["text"].toString(),
                QDateTime::fromString(mo["time"].toString(), Qt::ISODate)
            ));
        }
        chats.append(chat);
    }
}
