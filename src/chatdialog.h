#pragma once
#include <QDialog>
#include <QList>
#include "profile.h"

namespace Ui { class ChatDialog; }

class ChatDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChatDialog(const QList<Profile> &profiles,
                        const QString        &currentProfile,
                        QWidget              *parent = nullptr);
    ~ChatDialog();

    QString partnerName() const;

private:
    Ui::ChatDialog *ui;
};
