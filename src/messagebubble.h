#pragma once
#include <QWidget>

class QLabel;

// Виджет одного сообщения: метка «имя  время» + цветной пузырёк с текстом.
// Своё сообщение — выравнивается вправо, чужое — влево.
class MessageBubble : public QWidget
{
    Q_OBJECT
public:
    explicit MessageBubble(int msgId,
                           const QString &sender,
                           const QString &text,
                           const QString &timeStr,
                           bool isMine,
                           QWidget *parent = nullptr);

    int  msgId()    const { return m_msgId; }
    void setSelected(bool selected);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked(int msgId);

private:
    int     m_msgId;
    bool    m_isMine;
    QLabel *m_textLabel = nullptr;

    // Стиль пузырька
    QString normalStyle()    const;
    QString selectedStyle()  const;
};
