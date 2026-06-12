#include "src/profiledialog.h"
#include "ui_profiledialog.h"
#include <QInputDialog>
#include <QMessageBox>

ProfileDialog::ProfileDialog(QList<Profile> &profiles, QWidget *parent)
    : QDialog(parent), ui(new Ui::ProfileDialog), m_profiles(profiles)
{
    ui->setupUi(this);

    for (const auto &p : m_profiles)
        if (p.id() >= m_nextId) m_nextId = p.id() + 1;

    refreshList();

    connect(ui->selectBtn, &QPushButton::clicked,            this, &ProfileDialog::onSelect);
    connect(ui->createBtn, &QPushButton::clicked,            this, &ProfileDialog::onCreate);
    connect(ui->deleteBtn, &QPushButton::clicked,            this, &ProfileDialog::onDelete);
    connect(ui->profileList, &QListWidget::itemDoubleClicked,this, &ProfileDialog::onSelect);
}

ProfileDialog::~ProfileDialog() { delete ui; }

void ProfileDialog::refreshList()
{
    ui->profileList->clear();
    for (const auto &p : m_profiles)
        ui->profileList->addItem(p.name());
}

void ProfileDialog::onSelect()
{
    auto *item = ui->profileList->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Профиль не выбран", "Пожалуйста, выберите профиль из списка.");
        return;
    }
    m_selectedName = item->text();
    accept();
}

void ProfileDialog::onCreate()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Создать профиль",
                                         "Имя профиля:", QLineEdit::Normal, "", &ok).trimmed();
    if (!ok || name.isEmpty()) return;

    for (const auto &p : m_profiles) {
        if (p.name() == name) {
            QMessageBox::warning(this, "Ошибка", "Профиль с таким именем уже существует.");
            return;
        }
    }
    m_profiles.append(Profile(m_nextId++, name));
    refreshList();
    ui->profileList->setCurrentRow(ui->profileList->count() - 1);
}

void ProfileDialog::onDelete()
{
    auto *item = ui->profileList->currentItem();
    if (!item) {
        QMessageBox::warning(this, "Не выбрано", "Выберите профиль для удаления.");
        return;
    }
    if (QMessageBox::question(this, "Удаление",
            QString("Удалить профиль «%1»?").arg(item->text()),
            QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) return;

    QString name = item->text();
    for (int i = 0; i < m_profiles.size(); ++i) {
        if (m_profiles[i].name() == name) { m_profiles.removeAt(i); break; }
    }
    refreshList();
}
