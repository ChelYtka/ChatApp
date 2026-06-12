#include "src/messagebubble.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

MessageBubble::MessageBubble(int msgId,
                             const QString &sender,
                             const QString &text,
                             const QString &timeStr,
                             bool isMine,
                             QWidget *parent)
    : QWidget(parent), m_msgId(msgId), m_isMine(isMine)
{
    // ── Метка «имя  время» ───────────────────
    auto *metaLabel = new QLabel;
    metaLabel->setTextFormat(Qt::PlainText);
    metaLabel->setStyleSheet("color: #888888; font-size: 11px;");
    metaLabel->setAlignment(isMine ? Qt::AlignRight : Qt::AlignLeft);
    if (isMine)
        metaLabel->setText(QString("%1   %2").arg(timeStr, sender));
    else
        metaLabel->setText(QString("%1   %2").arg(sender, timeStr));

    // ── Пузырёк с текстом ────────────────────
    m_textLabel = new QLabel(text);
    m_textLabel->setWordWrap(true);
    m_textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_textLabel->setStyleSheet(normalStyle());
    m_textLabel->setAlignment(isMine ? Qt::AlignRight : Qt::AlignLeft);
    m_textLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // ── Внутренний столбец (метка + пузырёк) ─
    auto *inner = new QVBoxLayout;
    inner->setContentsMargins(0, 0, 0, 0);
    inner->setSpacing(3);
    inner->addWidget(metaLabel);
    inner->addWidget(m_textLabel);

    auto *innerWidget = new QWidget;
    innerWidget->setLayout(inner);
    innerWidget->setMaximumWidth(460);

    // ── Внешняя строка (выравнивание) ─────────
    auto *row = new QHBoxLayout(this);
    row->setContentsMargins(8, 2, 8, 2);
    row->setSpacing(0);

    if (isMine) {
        row->addStretch();
        row->addWidget(innerWidget);
    } else {
        row->addWidget(innerWidget);
        row->addStretch();
    }

    setCursor(Qt::PointingHandCursor);
}

// ── Стили ────────────────────────────────────

QString MessageBubble::normalStyle() const
{
    if (m_isMine)
        return "background: #DCF8C6;"
               "border: none;"
               "border-radius: 18px 18px 4px 18px;"
               "padding: 10px 14px;"
               "font-size: 13px;"
               "color: #1a1a1a;";
    else
        return "background: #FFFFFF;"
               "border: 1px solid #E0E0E0;"
               "border-radius: 18px 18px 18px 4px;"
               "padding: 10px 14px;"
               "font-size: 13px;"
               "color: #1a1a1a;";
}

QString MessageBubble::selectedStyle() const
{
    return "background: #FFE082;"
           "border: 1.5px solid #FFB300;"
           "border-radius: 18px;"
           "padding: 10px 14px;"
           "font-size: 13px;"
           "color: #1a1a1a;";
}

void MessageBubble::setSelected(bool selected)
{
    m_textLabel->setStyleSheet(selected ? selectedStyle() : normalStyle());
}

void MessageBubble::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    emit clicked(m_msgId);
}
