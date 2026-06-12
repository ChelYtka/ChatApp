QT       += core gui widgets

TARGET   = ChatApp
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main.cpp \
    cpp/message.cpp \
    cpp/profile.cpp \
    cpp/chat.cpp \
    cpp/storage.cpp \
    mainwindow.cpp \
    cpp/profiledialog.cpp \
    cpp/chatdialog.cpp \
    cpp/messagebubble.cpp

HEADERS += \
    src/message.h \
    src/profile.h \
    src/chat.h \
    src/storage.h \
    mainwindow.h \
    src/profiledialog.h \
    src/chatdialog.h \
    src/messagebubble.h

FORMS += \
    forms/mainwindow.ui \
    forms/profiledialog.ui \
    forms/chatdialog.ui
