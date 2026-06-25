#pragma once
#include <QGraphicsView>

class game_scene: public QGraphicsView {
public:
    game_scene(QGraphicsScene* s = new QGraphicsScene(), QWidget* p = nullptr): QGraphicsView(s, p)
    {
        this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    void add(QWidget* w) {
        w->setParent(this);
        this->scene()->addWidget(w);
        w->show();
    }
    virtual ~game_scene() {
        this->scene()->deleteLater();
    }
};

class disposable_scene: public game_scene {
public:
    disposable_scene(QGraphicsScene* s = new QGraphicsScene(), QWidget* p = nullptr): game_scene(s, p)
    {}
    void hideEvent(QHideEvent *event) {
        QWidget::hideEvent(event);
        this->deleteLater();
        qInfo() << "scene deleted";
    }
};
