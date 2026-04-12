#pragma once
#include <QString>
#include <mainwindow.h>
#include <QPushButton>
#include <QLabel>

//визуальные компоненты игровых объектов

//некликабельные картинки
class displayable {
protected:
    QString _name;
    QString _sprite_family;
    QLabel* _disp;
public:
    displayable() = default;
    displayable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name);
    virtual ~displayable() {delete _disp;}

    void blink();

};

//кликабельные картинки
class clickable : displayable{
protected:
    QPushButton* _clk;
public:
    clickable() = default;
    clickable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name);
    ~clickable() {delete _clk;}
};

//TO DO - blink; move_interpolation
