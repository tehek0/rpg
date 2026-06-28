#include "../header/scenes.h"
#include "../header/visuals.h"
#include <QGraphicsProxyWidget>
game_scene::game_scene(QGraphicsScene* s, QWidget* p): QGraphicsView(s, p) {
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
void game_scene::add(QWidget* w) {
    w->setParent(this);
    QGraphicsProxyWidget* p_widget = this->scene()->addWidget(w);
    p_widget->setZValue(0);
    w->show();
}
void game_scene::add(displayable* display) {
    add(display->_disp);
}
void game_scene::set_background(const QString& path) {
    background = QPixmap(path);
    update();
}
void game_scene::drawBackground(QPainter *painter, const QRectF &rect) {
    Q_UNUSED(rect);

    if (!background.isNull()) {
        painter->drawPixmap(sceneRect(), background, background.rect());
    }
}
void game_scene::wheelEvent(QWheelEvent *event) {
    event->ignore();
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
