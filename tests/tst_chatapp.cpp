#include <QtTest>
#include "../src/profile.h"
#include "../src/message.h"
#include "../src/chat.h"

// ══════════════════════════════════════════════════════════════════════════
//  Класс тестов ChatApp
//  Запуск: собрать проект tests/tests.pro и выполнить tst_chatapp
// ══════════════════════════════════════════════════════════════════════════

class TestChatApp : public QObject
{
    Q_OBJECT

private slots:

    // ── Тест 1: Создание профиля ──────────────────────────────────────────
    void test_Profile_Creation();

    // ── Тест 2: Создание чата и проверка участников ───────────────────────
    void test_Chat_Creation_And_Participants();

    // ── Тест 3: Добавление сообщений и уникальность id ───────────────────
    void test_Chat_AddMessage_And_UniqueIds();

    // ── Тест 4: Удаление существующего сообщения ─────────────────────────
    void test_Chat_RemoveMessage_Existing();

    // ── Тест 5: Удаление несуществующего сообщения возвращает false ───────
    void test_Chat_RemoveMessage_NonExistent();

    // ── Тест 6: hasParticipant — положительный и отрицательный случай ─────
    void test_Chat_HasParticipant();

    // ── Тест 7: Отправитель сообщения совпадает с указанным профилем ──────
    void test_Message_SenderName();

    // ── Тест 8: После удаления сообщения размер списка уменьшается ────────
    void test_Chat_MessageCount_AfterRemove();
};

// ══════════════════════════════════════════════════════════════════════════
//  Реализации тестов
// ══════════════════════════════════════════════════════════════════════════

// Тест 1
void TestChatApp::test_Profile_Creation()
{
    Profile p(1, "Алиса");

    QCOMPARE(p.id(),   1);
    QCOMPARE(p.name(), QString("Алиса"));
}

// Тест 2
void TestChatApp::test_Chat_Creation_And_Participants()
{
    Chat chat(1, "Алиса — Боб", "Алиса", "Боб");

    QCOMPARE(chat.id(),           1);
    QCOMPARE(chat.name(),         QString("Алиса — Боб"));
    QCOMPARE(chat.participant1(), QString("Алиса"));
    QCOMPARE(chat.participant2(), QString("Боб"));
}

// Тест 3
void TestChatApp::test_Chat_AddMessage_And_UniqueIds()
{
    Chat chat(1, "Тест", "Алиса", "Боб");

    int id1 = chat.addMessage("Алиса", "Привет");
    int id2 = chat.addMessage("Боб",   "Привет!");
    int id3 = chat.addMessage("Алиса", "Как дела?");

    // Все id должны быть уникальны
    QVERIFY(id1 != id2);
    QVERIFY(id2 != id3);
    QVERIFY(id1 != id3);

    // Список сообщений должен содержать 3 элемента
    QCOMPARE(chat.messages().size(), 3);
}

// Тест 4
void TestChatApp::test_Chat_RemoveMessage_Existing()
{
    Chat chat(1, "Тест", "Алиса", "Боб");
    int id = chat.addMessage("Алиса", "Удали меня");

    bool result = chat.removeMessage(id);

    QVERIFY(result);                            // метод вернул true
    QCOMPARE(chat.messages().size(), 0);        // список пуст
}

// Тест 5
void TestChatApp::test_Chat_RemoveMessage_NonExistent()
{
    Chat chat(1, "Тест", "Алиса", "Боб");
    chat.addMessage("Алиса", "Сообщение");

    // Пытаемся удалить сообщение с несуществующим id
    bool result = chat.removeMessage(9999);

    QVERIFY(!result);                           // метод должен вернуть false
    QCOMPARE(chat.messages().size(), 1);        // список не изменился
}

// Тест 6
void TestChatApp::test_Chat_HasParticipant()
{
    Chat chat(1, "Алиса — Боб", "Алиса", "Боб");

    QVERIFY(chat.hasParticipant("Алиса"));      // участник 1 — есть
    QVERIFY(chat.hasParticipant("Боб"));        // участник 2 — есть
    QVERIFY(!chat.hasParticipant("Виктор"));    // посторонний — нет
    QVERIFY(!chat.hasParticipant(""));          // пустая строка — нет
}

// Тест 7
void TestChatApp::test_Message_SenderName()
{
    Chat chat(1, "Тест", "Алиса", "Боб");
    chat.addMessage("Алиса", "Тестовое сообщение");

    const Message &msg = chat.messages().first();

    QCOMPARE(msg.profileName(), QString("Алиса"));
    QCOMPARE(msg.text(),        QString("Тестовое сообщение"));
    QVERIFY(msg.time().isValid());              // время должно быть валидным
}

// Тест 8
void TestChatApp::test_Chat_MessageCount_AfterRemove()
{
    Chat chat(1, "Тест", "Алиса", "Боб");

    int id1 = chat.addMessage("Алиса", "Первое");
    int id2 = chat.addMessage("Боб",   "Второе");
    int id3 = chat.addMessage("Алиса", "Третье");
    (void)id2; (void)id3;

    QCOMPARE(chat.messages().size(), 3);

    chat.removeMessage(id1);
    QCOMPARE(chat.messages().size(), 2);

    // Проверяем, что оставшиеся сообщения — правильные
    QCOMPARE(chat.messages().at(0).text(), QString("Второе"));
    QCOMPARE(chat.messages().at(1).text(), QString("Третье"));
}

// ══════════════════════════════════════════════════════════════════════════

QTEST_APPLESS_MAIN(TestChatApp)
#include "tst_chatapp.moc"
