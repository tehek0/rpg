#pragma once
#include <QGraphicsView>

class game_scene: public QGraphicsView {
public:
    game_scene(QGraphicsScene* s = new QGraphicsScene(), QWidget* p = nullptr): QGraphicsView(s, p)
    {}
    void add(QWidget* w) {
        w->setParent(this);
        this->scene()->addWidget(w);
        w->show();
    }
};

class disposable_scene: public game_scene {
    disposable_scene(QGraphicsScene* s = new QGraphicsScene(), QWidget* p = nullptr): game_scene(s, p)
    {}
    void hide() {
        delete this;
    }
};
