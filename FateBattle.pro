#-------------------------------------------------
#
# Project created by QtCreator 2016-04-09T22:36:00
#
#-------------------------------------------------

QT       += core gui
qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl): QT += opengl

CONFIG += staticlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FateBattle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Servant.cpp \
    Skill.cpp \
    Debuff.cpp \
    GameState.cpp \
    PlayField.cpp \
    Servants/servantsaberclaymore.cpp \
    Servants/servantsaberkatana.cpp \
    Servants/servantlancerlance.cpp \
    Servants/servantlancerhalberd.cpp \
    Servants/servantlancernaginata.cpp \
    Servants/servantarcherbow.cpp \
    Servants/servantarcherpistols.cpp \
    Servants/servantarcherprojectiles.cpp \
    Servants/servantcasterelemental.cpp \
    Servants/servantcasternecromancy.cpp \
    Servants/servantcastersummoner.cpp \
    Servants/servantberserkeraxe.cpp \
    Servants/servantberserkerclub.cpp \
    Servants/servantberserkerflail.cpp \
    Servants/servantriderpegasus.cpp \
    Servants/servantriderwyvern.cpp \
    Servants/servantriderchariot.cpp \
    Servants/servantassassindagger.cpp \
    Servants/servantassassinshuriken.cpp \
    Servants/servantavengersai.cpp \
    Servants/servantavengerscythe.cpp \
    playfieldsquare.cpp \
    view.cpp \
    Servants/servanttest.cpp \
    logger.cpp \
    Servants/servantsaber.cpp \
    Servants/servantlancer.cpp \
    Servants/servantarcher.cpp \
    Servants/servantcaster.cpp \
    Servants/servantberserker.cpp \
    Servants/servantrider.cpp \
    Servants/servantassassin.cpp \
    Servants/servantavenger.cpp \
    Servants/bossgil.cpp \
    Servants/bossrin.cpp \
    Servants/bossdallas.cpp \
    Servants/bossyuu.cpp \
    Servants/bossmika.cpp \
    Servants/servantarcherminion.cpp \
    Servants/servantcasterminion.cpp \
    Servants/servantsaberminion.cpp

HEADERS  += mainwindow.h \
    Debuff.h \
    GameState.h \
    PlayField.h \
    Servant.h \
    Skill.h \
    Servants/servantsaberclaymore.h \
    Servants/servantsaberkatana.h \
    Servants/servantlancerlance.h \
    Servants/servantlancerhalberd.h \
    Servants/servantlancernaginata.h \
    Servants/servantarcherbow.h \
    Servants/servantarcherpistols.h \
    Servants/servantarcherprojectiles.h \
    Servants/servantcasterelemental.h \
    Servants/servantcasternecromancy.h \
    Servants/servantcastersummoner.h \
    Servants/servantberserkeraxe.h \
    Servants/servantberserkerclub.h \
    Servants/servantberserkerflail.h \
    Servants/servantriderpegasus.h \
    Servants/servantriderwyvern.h \
    Servants/servantriderchariot.h \
    Servants/servantassassindagger.h \
    Servants/servantassassinshuriken.h \
    Servants/servantavengersai.h \
    Servants/servantavengerscythe.h \
    playfieldsquare.h \
    view.h \
    datatypes.h \
    Servants/servanttest.h \
    allservants.h \
    logger.h \
    Servants/servantsaber.h \
    Servants/servantlancer.h \
    Servants/servantarcher.h \
    Servants/servantcaster.h \
    Servants/servantberserker.h \
    Servants/servantrider.h \
    Servants/servantassassin.h \
    Servants/servantavenger.h \
    Servants/bossgil.h \
    Servants/bossrin.h \
    Servants/bossdallas.h \
    Servants/bossyuu.h \
    Servants/bossmika.h \
    Servants/servantarcherminion.h \
    Servants/servantcasterminion.h \
    Servants/servantsaberminion.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

DISTFILES += \
    qt4logo.png \
    zoomin.png \
    zoomout.png \
    ServantIcons/SaberTest.png \
    ServantIcons/Axe Berserker.png \
    ServantIcons/Bow Archer.png \
    ServantIcons/Chariot Rider.png \
    ServantIcons/Claymore Saber.png \
    ServantIcons/Club Berserker.png \
    ServantIcons/Dagger Assassin.png \
    ServantIcons/Elemental Caster.png \
    ServantIcons/Flail Berserker.png \
    ServantIcons/Halberd Lancer.png \
    ServantIcons/Katana Saber.png \
    ServantIcons/Lance Lancer.png \
    ServantIcons/Naginata Lancer.png \
    ServantIcons/Necromancer Caster.png \
    ServantIcons/Pegasus Rider.png \
    ServantIcons/Pistol Archer.png \
    ServantIcons/Projectile Archer.png \
    ServantIcons/Saber Test.png \
    ServantIcons/Sai Avenger.png \
    ServantIcons/Scythe Avenger.png \
    ServantIcons/Shuriken Assassin.png \
    ServantIcons/Summoner Caster.png \
    ServantIcons/Wyvern Rider.png \
    Bin/TeamIcons/Alpha.png \
    Bin/TeamIcons/Omega.png \
    Bin/TeamIcons/Psi.png
