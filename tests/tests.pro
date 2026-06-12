QT       += core testlib
QT       -= gui

TARGET   = tst_chatapp
TEMPLATE = app
CONFIG  += c++17 console
CONFIG  -= app_bundle

# Подключаем исходники основного приложения
INCLUDEPATH += $$PWD/../../

SOURCES += \
    tst_chatapp.cpp \
    ../cpp/message.cpp \
    ../cpp/profile.cpp \
    ../cpp/chat.cpp

HEADERS += \
    ../src/message.h \
    ../src/profile.h \
    ../src/chat.h
