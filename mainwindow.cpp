#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/profiledialog.h"
#include "src/chatdialog.h"
#include "src/messagebubble.h"
#include "src/storage.h"

#include <QVBoxLayout>
#include <QScrollBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTimer>

// ─────────────────────────────────────────────
MainWindow::MainWindow(QList<Profile> &profiles,
                       QList<Chat>    &chats,
                       const QString  &currentProfile,
                       QWidget        *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_profiles(profiles)
    , m_chats(chats)
    , m_currentProfile(currentProfile)
{
    ui->setupUi(this);
    applyStyles();

    // Растягивающий spacer в конец области пузырьков
    m_bubblesLayout = qobject_cast<QVBoxLayout *>(ui->scrollContents->layout());
    m_bubblesLayout->addStretch();

    ui->profileLabel->setText(QString("👤  %1").arg(m_currentProfile));
    refreshChatList();

    connect(ui->chatList,       &QListWidget::currentRowChanged, this, &MainWindow::onChatSelected);
    connect(ui->sendBtn,        &QPushButton::clicked,           this, &MainWindow::onSendMessage);
    connect(ui->deleteMsgBtn,   &QPushButton::clicked,           this, &MainWindow::onDeleteMessage);
    connect(ui->newChatBtn,     &QPushButton::clicked,           this, &MainWindow::onCreateChat);
    connect(ui->deleteChatBtn,  &QPushButton::clicked,           this, &MainWindow::onDeleteChat);
    connect(ui->switchProfileBtn,&QPushButton::clicked,          this, &MainWindow::onSwitchProfile);
    connect(ui->inputEdit,      &QLineEdit::returnPressed,       this, &MainWindow::onSendMessage);
    connect(ui->searchEdit,     &QLineEdit::textChanged,         this, &MainWindow::onSearchChanged);
}

MainWindow::~MainWindow() { delete ui; }

// ─────────────────────────────────────────────
//  Стили (современный, округлый)
// ─────────────────────────────────────────────
void MainWindow::applyStyles()
{
    // Всё приложение
    setStyleSheet(R"(
        QMainWindow {
            background: #F0F2F5;
        }

        /* ── Левая панель ── */
        QWidget#leftPanel {
            background: #FFFFFF;
            border-right: 1px solid #E0E0E0;
        }
        QLabel#profileLabel {
            font-size: 14px;
            font-weight: bold;
            color: #1A1A2E;
            padding: 4px 2px;
        }
        QPushButton#switchProfileBtn {
            background: transparent;
            color: #5C6BC0;
            border: 1px solid #C5CAE9;
            border-radius: 8px;
            padding: 5px 10px;
            font-size: 12px;
        }
        QPushButton#switchProfileBtn:hover {
            background: #EDE7F6;
        }
        QLineEdit#searchEdit {
            background: #F5F5F5;
            border: 1px solid #E0E0E0;
            border-radius: 16px;
            padding: 6px 12px;
            font-size: 12px;
            color: #555;
        }
        QListWidget#chatList {
            background: transparent;
            border: none;
            font-size: 13px;
        }
        QListWidget#chatList::item {
            padding: 10px 8px;
            border-radius: 10px;
            margin: 2px 0;
            color: #1A1A2E;
        }
        QListWidget#chatList::item:selected {
            background: #E8EAF6;
            color: #3949AB;
            font-weight: bold;
        }
        QListWidget#chatList::item:hover:!selected {
            background: #F5F5F5;
        }
        QPushButton#newChatBtn {
            background: #5C6BC0;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 7px 10px;
            font-size: 12px;
            font-weight: bold;
        }
        QPushButton#newChatBtn:hover { background: #3949AB; }
        QPushButton#deleteChatBtn {
            background: transparent;
            color: #E53935;
            border: 1px solid #FFCDD2;
            border-radius: 8px;
            padding: 7px 10px;
            font-size: 12px;
        }
        QPushButton#deleteChatBtn:hover { background: #FFEBEE; }

        /* ── Правая панель ── */
        QWidget#rightPanel {
            background: #F0F2F5;
        }
        QLabel#chatTitleLabel {
            font-size: 15px;
            font-weight: bold;
            color: #1A1A2E;
            background: #FFFFFF;
            border-radius: 12px;
            padding: 8px 16px;
        }
        QScrollArea {
            background: transparent;
            border: none;
        }
        QWidget#scrollContents {
            background: transparent;
        }
        QLineEdit#inputEdit {
            background: #FFFFFF;
            border: 1.5px solid #C5CAE9;
            border-radius: 20px;
            padding: 8px 16px;
            font-size: 13px;
            color: #1A1A2E;
        }
        QLineEdit#inputEdit:focus {
            border-color: #5C6BC0;
        }
        QPushButton#sendBtn {
            background: #5C6BC0;
            color: white;
            border: none;
            border-radius: 20px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: bold;
            min-width: 90px;
        }
        QPushButton#sendBtn:hover { background: #3949AB; }
        QPushButton#deleteMsgBtn {
            background: transparent;
            color: #E53935;
            border: 1.5px solid #FFCDD2;
            border-radius: 20px;
            padding: 8px 14px;
            font-size: 12px;
        }
        QPushButton#deleteMsgBtn:hover { background: #FFEBEE; }
    )");
}

// ─────────────────────────────────────────────
//  Список чатов (с фильтрацией по поиску)
// ─────────────────────────────────────────────
void MainWindow::refreshChatList(const QString &filter)
{
    int prevRow = ui->chatList->currentRow();
    QString prevName;
    if (auto *it = ui->chatList->currentItem()) prevName = it->text();

    ui->chatList->clear();
    for (const auto &c : m_chats) {
        if (!c.hasParticipant(m_currentProfile)) continue;
        if (!filter.isEmpty() &&
            !c.name().contains(filter, Qt::CaseInsensitive)) continue;
        ui->chatList->addItem(c.name());
    }

    // Восстанавливаем выделение
    bool restored = false;
    if (!prevName.isEmpty()) {
        for (int i = 0; i < ui->chatList->count(); ++i) {
            if (ui->chatList->item(i)->text() == prevName) {
                ui->chatList->setCurrentRow(i);
                restored = true;
                break;
            }
        }
    }
    if (!restored) {
        if (ui->chatList->count() > 0)
            ui->chatList->setCurrentRow(0);
        else {
            clearBubbles();
            ui->chatTitleLabel->setText("Выберите чат");
        }
    }
}

// ─────────────────────────────────────────────
//  Реальный индекс в m_chats по текущему пункту списка
// ─────────────────────────────────────────────
int MainWindow::realChatIndex() const
{
    auto *item = ui->chatList->currentItem();
    if (!item) return -1;
    QString name = item->text();
    for (int i = 0; i < m_chats.size(); ++i)
        if (m_chats[i].name() == name && m_chats[i].hasParticipant(m_currentProfile))
            return i;
    return -1;
}

// ─────────────────────────────────────────────
//  Очистка пузырьков
// ─────────────────────────────────────────────
void MainWindow::clearBubbles()
{
    // Удаляем все элементы кроме последнего stretch
    while (m_bubblesLayout->count() > 1)
        delete m_bubblesLayout->takeAt(0)->widget();
    m_selectedMsgId  = -1;
    m_selectedBubble = nullptr;
}

// ─────────────────────────────────────────────
//  Отображение сообщений пузырьками
// ─────────────────────────────────────────────
void MainWindow::refreshMessages()
{
    clearBubbles();

    int idx = realChatIndex();
    if (idx < 0) return;

    const Chat &chat = m_chats[idx];
    ui->chatTitleLabel->setText(chat.name());

    for (const auto &msg : chat.messages()) {
        bool isMine = (msg.profileName() == m_currentProfile);
        auto *bubble = new MessageBubble(
            msg.id(),
            msg.profileName(),
            msg.text(),
            msg.time().toString("hh:mm"),
            isMine,
            ui->scrollContents
            );
        // Вставляем перед stretch (последний элемент)
        m_bubblesLayout->insertWidget(m_bubblesLayout->count() - 1, bubble);
        connect(bubble, &MessageBubble::clicked, this, &MainWindow::onBubbleClicked);
    }

    // Прокрутка вниз после обновления layout
    QScrollBar *sb = ui->scrollArea->verticalScrollBar();
    QTimer::singleShot(30, sb, [sb]() { sb->setValue(sb->maximum()); });
}

// ─────────────────────────────────────────────
//  Слоты
// ─────────────────────────────────────────────
void MainWindow::onChatSelected(int /*row*/)
{
    refreshMessages();
}

void MainWindow::onSendMessage()
{
    QString text = ui->inputEdit->text().trimmed();
    if (text.isEmpty()) return;

    int idx = realChatIndex();
    if (idx < 0) {
        QMessageBox::information(this, "Нет чата", "Выберите или создайте чат.");
        return;
    }

    m_chats[idx].addMessage(m_currentProfile, text);
    ui->inputEdit->clear();
    refreshMessages();
}

void MainWindow::onDeleteMessage()
{
    if (m_selectedMsgId < 0) {
        QMessageBox::information(this, "Не выбрано",
                                 "Нажмите на сообщение, которое хотите удалить, затем нажмите «Удалить».");
        return;
    }
    int idx = realChatIndex();
    if (idx < 0) return;

    if (QMessageBox::question(this, "Удаление сообщения",
                              "Удалить выбранное сообщение?",
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) return;

    m_chats[idx].removeMessage(m_selectedMsgId);
    m_selectedMsgId  = -1;
    m_selectedBubble = nullptr;
    refreshMessages();
}

void MainWindow::onCreateChat()
{
    if (m_profiles.size() < 2) {
        QMessageBox::information(this, "Нет собеседников",
                                 "Создайте ещё хотя бы один профиль, чтобы начать чат.");
        return;
    }

    ChatDialog dlg(m_profiles, m_currentProfile, this);
    if (dlg.exec() != QDialog::Accepted) return;

    QString partner = dlg.partnerName();
    if (partner.isEmpty()) return;

    // Проверяем, существует ли уже чат между этими двумя
    for (const auto &c : m_chats) {
        if (c.hasParticipant(m_currentProfile) && c.hasParticipant(partner)) {
            QMessageBox::information(this, "Чат существует",
                                     QString("Чат с пользователем «%1» уже существует.").arg(partner));
            for (int i = 0; i < ui->chatList->count(); ++i) {
                if (ui->chatList->item(i)->text() == c.name()) {
                    ui->chatList->setCurrentRow(i);
                    break;
                }
            }
            return;
        }
    }

    int newId = 1;
    for (const auto &c : m_chats)
        if (c.id() >= newId) newId = c.id() + 1;

    QString chatName = QString("%1 — %2").arg(m_currentProfile, partner);
    m_chats.append(Chat(newId, chatName, m_currentProfile, partner));
    refreshChatList();

    for (int i = 0; i < ui->chatList->count(); ++i) {
        if (ui->chatList->item(i)->text() == chatName) {
            ui->chatList->setCurrentRow(i);
            break;
        }
    }
}

void MainWindow::onDeleteChat()
{
    int idx = realChatIndex();
    if (idx < 0) {
        QMessageBox::information(this, "Нет чата", "Выберите чат для удаления.");
        return;
    }
    if (QMessageBox::question(this, "Удаление чата",
                              QString("Удалить чат «%1» со всеми сообщениями?").arg(m_chats[idx].name()),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) return;

    m_chats.removeAt(idx);
    clearBubbles();
    ui->chatTitleLabel->setText("Выберите чат");
    refreshChatList();
}

void MainWindow::onSwitchProfile()
{
    Storage::save(m_profiles, m_chats);

    ProfileDialog dlg(m_profiles, this);
    if (dlg.exec() != QDialog::Accepted) return;

    m_currentProfile = dlg.selectedProfileName();
    ui->profileLabel->setText(QString("👤  %1").arg(m_currentProfile));
    Storage::save(m_profiles, m_chats);

    clearBubbles();
    ui->chatTitleLabel->setText("Выберите чат");
    ui->searchEdit->clear();
    refreshChatList();
}

void MainWindow::onSearchChanged(const QString &text)
{
    refreshChatList(text);
}

void MainWindow::onBubbleClicked(int msgId)
{
    // Снимаем выделение с предыдущего
    if (m_selectedBubble)
        m_selectedBubble->setSelected(false);

    // Находим новый пузырёк
    for (int i = 0; i < m_bubblesLayout->count() - 1; ++i) {
        auto *bubble = qobject_cast<MessageBubble *>(
            m_bubblesLayout->itemAt(i)->widget());
        if (bubble && bubble->msgId() == msgId) {
            bubble->setSelected(true);
            m_selectedBubble = bubble;
            break;
        }
    }
    m_selectedMsgId = msgId;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Storage::save(m_profiles, m_chats);
    event->accept();
}
