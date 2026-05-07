QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    devkitwindow.cpp \
    src/id_support.cpp

HEADERS += \
    devkitwindow.h \
    header/datatypes.hpp \
    header/general.hpp \
    header/id_support.hpp

FORMS += \
    devkitwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    data/anim_disp.json \
    data/anim_disp_ids.txt \
    data/disp.json \
    data/disp_ids.txt \
    data/enemy.json \
    data/enemy_ids.txt \
    data/item.json \
    data/item_ids.txt \
    data/location.json \
    data/location_ids.txt \
    data/npc.json \
    data/npc_ids.txt \
    data/quest.json \
    data/quest_ids.txt \
    data/sublocation.json \
    data/sublocation_ids.txt \
    data/trader.json \
    data/trader_ids.txt
