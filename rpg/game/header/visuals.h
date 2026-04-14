#pragma once
#include <QString>
#include <mainwindow.h>
#include <QPushButton>
#include <QLabel>

//визуальные компоненты игровых объектов

class displayable : QPushButton {
    Q_OBJECT;
protected:
    QString _name;
    QString _sprite_family;
public:
    displayable() = default;
    displayable(MainWindow* w, bool clickable, QRect& coord_and_size, QString& sprite_family, QString& name) : QPushButton(w) {
            this->setStyleSheet(QString("border-image: url(:/%1.png);").arg(sprite_family));
            this->setGeometry(coord_and_size);
            this->setObjectName(name);
            if (!clickable) {
                this->setDisabled(true);
            }
            this->show();
    };

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
