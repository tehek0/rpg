QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    game/src/character.cpp \
    game/src/inventory.cpp \
    game/src/item.cpp \
    game/src/item_requirements.cpp \
    game/src/visuals.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    game/header/battle.h \
    game/header/character.h \
    game/header/data/ammo_types.h \
    game/header/data/armor_slots.h \
    game/header/data/char_types.h \
    game/header/data/damage_types.h \
    game/header/data/enemy_trait_types.h \
    game/header/data/equipment_bonuses.h \
    game/header/data/interaction_return_value_types.h \
    game/header/data/quest_triggers.h \
    game/header/data/skill_types.h \
    game/header/data/trait_types.h \
    game/header/data/use_effects.h \
    game/header/global.h \
    game/header/inventory.h \
    game/header/item.h \
    game/header/item_requirements.h \
    game/header/location.h \
    game/header/quest.h \
    game/header/save.h \
    game/header/visuals.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    game/assets/pictures/res.qrc

DISTFILES += \
    assets/pictures/backg_map.jpg.kra \
    assets/pictures/map.jpg \
    game/assets/pictures/backg_map.jpg.kra \
    game/assets/pictures/map.jpg
