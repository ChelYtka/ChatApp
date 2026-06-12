#pragma once
#include <QDialog>
#include <QList>
#include "profile.h"

namespace Ui { class ProfileDialog; }

class ProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProfileDialog(QList<Profile> &profiles, QWidget *parent = nullptr);
    ~ProfileDialog();

    QString selectedProfileName() const { return m_selectedName; }

private slots:
    void onSelect();
    void onCreate();
    void onDelete();

private:
    void refreshList();

    Ui::ProfileDialog *ui;
    QList<Profile>    &m_profiles;
    QString            m_selectedName;
    int                m_nextId = 1;
};
