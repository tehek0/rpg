#pragma once
#include <QString>
#include <mainwindow.h>

//визуальные компоненты игровых объектов

//некликабельные картинки
class displayable {
protected:
    QString _name;
    QString _sprite_family;
public:
    displayable() = default;
    displayable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name);
    virtual ~displayable() = default;
};

//кликабельные картинки
class clickable : displayable{
public:
    clickable() = default;
    clickable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name);
};
