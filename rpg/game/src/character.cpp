#include "../header/character.h"
#include <QPushButton>

entity::entity(MainWindow* w, QString sprite_family){
    //создание технического объекта
    entity();
    // создание визуального компонента
    QPushButton* ent = new QPushButton(w);
    ent->setStyleSheet(QString("background-image: url(:/sprite/%1.jpg);").arg(sprite_family));
    ent->setGeometry(100,100,100,100);
    ent->setObjectName(_name);
    qInfo() << "[entity] constr";
}
