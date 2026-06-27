#pragma once
#include <QGraphicsView>
class displayable;

class game_scene: public QGraphicsView {
public:
    game_scene(QGraphicsScene* s = new QGraphicsScene(), QWidget* p = nullptr);
    void add(QWidget* w);
    void add(displayable* display);
    virtual ~game_scene();
};

class disposable_scene: public game_scene {
public:
    disposable_scene(QGraphicsScene* s = new QGraphicsScene(), QWidget* p = nullptr);
    void hideEvent(QHideEvent *event);
};
