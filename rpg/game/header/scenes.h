#pragma once
#include <QGraphicsView>
class displayable;

class game_scene: public QGraphicsView {
protected:
    void drawBackground(QPainter* painter, const QRectF& rect);
    void wheelEvent(QWheelEvent *event);
public:
    QPixmap background;
    game_scene(QGraphicsScene* s = new QGraphicsScene(), QWidget* p = nullptr);
    void add(QWidget* w);
    void add(displayable* display);
    void set_background(const QString& path);
    virtual ~game_scene();
};

class disposable_scene: public game_scene {
public:
    disposable_scene(QGraphicsScene* s = new QGraphicsScene(), QWidget* p = nullptr);
    void hideEvent(QHideEvent *event);
};
