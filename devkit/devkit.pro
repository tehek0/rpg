QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    devkitwindow.cpp \
    src/create_object.cpp \
    src/custom_window.cpp \
    src/datatypes.cpp \
    src/id_support.cpp \
    src/info_fields.cpp \
    src/map_construct.cpp \
    src/object_dialogs.cpp \
    src/read_object.cpp

HEADERS += \
    devkitwindow.h \
    header/create_object.hpp \
    header/custom_window.h \
    header/data/object_dialog_templates.hpp \
    header/data/datatypes.hpp \
    header/data/general.hpp \
    header/id_support.hpp \
    header/inc/json.hpp \
    header/info_fields.hpp \
    header/map_construct.hpp \
    header/object_dialogs.hpp \
    header/read_object.hpp

FORMS += \
    devkitwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
