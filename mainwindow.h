#pragma once
#include <QMainWindow>
#include <QList>
#include "src/profile.h"
#include "src/chat.h"

namespace Ui { class MainWindow; }
class MessageBubble;
class QVBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QList<Profile> &profiles,
               QList<Chat>    &chats,
               const QString  &currentProfile,
               QWidget        *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onChatSelected(int row);
    void onSendMessage();
    void onDeleteMessage();
    void onCreateChat();
    void onDeleteChat();
    void onSwitchProfile();
    void onSearchChanged(const QString &text);
    void onBubbleClicked(int msgId);

private:
    void applyStyles();
    void refreshChatList(const QString &filter = {});
    void refreshMessages();
    void clearBubbles();
    int  realChatIndex() const;   // индекс в m_chats по текущему пункту списка

    Ui::MainWindow *ui;
    QList<Profile> &m_profiles;
    QList<Chat>    &m_chats;
    QString         m_currentProfile;

    QVBoxLayout    *m_bubblesLayout   = nullptr;
    int             m_selectedMsgId   = -1;
    MessageBubble  *m_selectedBubble  = nullptr;
};
