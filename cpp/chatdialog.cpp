#include "src/chatdialog.h"
#include "ui_chatdialog.h"
#include <QMessageBox>

ChatDialog::ChatDialog(const QList<Profile> &profiles,
                       const QString        &currentProfile,
                       QWidget              *parent)
    : QDialog(parent), ui(new Ui::ChatDialog)
{
    ui->setupUi(this);

    for (const auto &p : profiles)
        if (p.name() != currentProfile)
            ui->partnerCombo->addItem(p.name());

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        if (ui->partnerCombo->currentText().isEmpty()) {
            QMessageBox::warning(this, "Нет собеседников",
                "Создайте ещё один профиль, чтобы начать чат.");
            return;
        }
        accept();
    });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ChatDialog::~ChatDialog() { delete ui; }

QString ChatDialog::partnerName() const
{
    return ui->partnerCombo->currentText();
}
