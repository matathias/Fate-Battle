#-------------------------------------------------
#
# Project created by QtCreator 2016-04-09T22:36:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FateBattle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Servant.cpp \
    Skill.cpp \
    Weapon.cpp \
    Debuff.cpp \
    GameState.cpp \
    NoblePhantasm.cpp \
    PlayField.cpp

HEADERS  += mainwindow.h \
    Debuff.h \
    GameState.h \
    NoblePhantasm.h \
    PlayField.h \
    Servant.h \
    Skill.h \
    Weapon.h \
    Game.h

FORMS    += mainwindow.ui
