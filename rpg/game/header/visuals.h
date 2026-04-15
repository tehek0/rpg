#pragma once
#include <QString>
#include <mainwindow.h>
#include <QPushButton>
#include <QLabel>

//визуальные компоненты игровых объектов

class displayable {
protected:
    QString _name;
    QString _sprite_family;

public:
    QPushButton* _disp;

    displayable() = default;
    displayable(MainWindow* w, bool clickable, QRect& coord_and_size, QString& sprite_family, QString& name) : displayable(){
        QPushButton* disp = new QPushButton();
            disp->setStyleSheet(QString("border-image: url(:/%1.png);").arg(sprite_family));
            disp->setGeometry(coord_and_size);
            disp->setObjectName(name);
            disp->setParent(w);
            if (!clickable) {
                disp->setDisabled(true);
            }
            disp->show();
            _disp = disp;
    }

    virtual ~displayable() = default;
};
//кликабельные картинки
// class clickable : displayable{
// protected:
//     QPushButton* _clk;
// public:
//     clickable() = default;
//     clickable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name);
//     ~clickable() {delete _clk;}
// };

//TO DO - blink; move_interpolation
