#include "../header/character.h"
#include "../header/visuals.h"

entity::entity(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name){ //через делегирующий конструктор 2 он запускать не может, поэтому так

    //создание технического объекта
    entity();
    // создание визуального компонента
    clickable(w, coord_and_size, sprite_family, name);

    qInfo() << "[entity] constr";
}
