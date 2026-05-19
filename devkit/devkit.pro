QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    item_dialog.cpp \
    main.cpp \
    devkitwindow.cpp \
    src/create_object.cpp \
    src/id_support.cpp \
    src/object_dialogs.cpp

HEADERS += \
    devkitwindow.h \
    header/create_object.hpp \
    header/data/object_dialog_templates.hpp \
    header/data/datatypes.hpp \
    header/data/general.hpp \
    header/id_support.hpp \
    header/object_dialogs.hpp \
    item_dialog.h

FORMS += \
    devkitwindow.ui \
    item_dialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
