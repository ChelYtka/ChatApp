#include <QApplication>
#include "src/storage.h"
#include "src/profiledialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ChatApp");

    // Загружаем данные
    QList<Profile> profiles;
    QList<Chat>    chats;
    Storage::load(profiles, chats);

    // Окно выбора профиля
    ProfileDialog profileDlg(profiles);
    if (profileDlg.exec() != QDialog::Accepted) {
        Storage::save(profiles, chats);
        return 0;
    }
    Storage::save(profiles, chats);

    // Главное окно
    MainWindow window(profiles, chats, profileDlg.selectedProfileName());
    window.resize(860, 560);
    window.show();

    return app.exec();
}
