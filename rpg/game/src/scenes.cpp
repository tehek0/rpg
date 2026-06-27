#include "../header/scenes.h"
#include "../header/visuals.h"

game_scene::game_scene(QGraphicsScene* s, QWidget* p): QGraphicsView(s, p) {
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
void game_scene::add(QWidget* w) {
    w->setParent(this);
    this->scene()->addWidget(w);
    w->show();
}
void game_scene::add(displayable* display) {
    add(display->_disp);
}
game_scene::~game_scene() {
    this->scene()->deleteLater();
}
disposable_scene::disposable_scene(QGraphicsScene* s, QWidget* p): game_scene(s, p)
{}
void disposable_scene::hideEvent(QHideEvent *event) {
    QWidget::hideEvent(event);
    this->deleteLater();
    qInfo() << "scene deleted";
}
