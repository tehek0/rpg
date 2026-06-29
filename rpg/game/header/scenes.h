#pragma once
#include <QGraphicsView>
class displayable;

enum mouse_task {
    none,
    target_selection
};

class game_scene: public QGraphicsView {
protected:
    void drawBackground(QPainter* painter, const QRectF& rect);
public:
    mouse_task _task;
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

class battle_scene: public game_scene {
public:
    battle_scene(QGraphicsScene* s = new QGraphicsScene(), QWidget* p = nullptr) : game_scene(s, p) {};
public slots:
    void target_selection_clicked();
};