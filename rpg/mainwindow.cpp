#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/header/visuals.h"
#include "game/header/config.h"
#include "game/header/ui.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>

#include "game/header/character.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    global::timer->start(global::tick_timeout);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::menu_play() {

    std::vector<enemy*> test_enemies;
    enemy* one = new enemy();
    enemy* two = new enemy();
    enemy* three = new enemy();
    one->set_size_class(entity_size_class::small);
    two->set_size_class(entity_size_class::tiny);
    three->set_size_class(entity_size_class::medium);
    one->set_max_health(40);
    two->set_max_health(10);
    three->set_max_health(40);
    one->set_health(20);
    two->set_health(4);
    three->set_health(40);
    test_enemies.emplace_back(one);
    test_enemies.emplace_back(two);
    test_enemies.emplace_back(three);
    qInfo() << one->get_health();
    qInfo() << two->get_health();
    qInfo() << three->get_health();

    if (hub_scene == nullptr) {
        hub_scene = throw_hub_scene();
    }
    if (battle_scene == nullptr) {
        battle_scene = throw_battle_scene(global::player_, test_enemies);
    }
    switch_to_scene(hub_scene);

}

void MainWindow::open_settings() {
    game_scene* scene_ = throw_settings_scene();
    switch_to_scene(scene_);
}

void MainWindow::change_volume(double* source, float value) {
    *source = value / 100;
    change_cfg(global::master_volume, global::sfx_volume, global::music_volume);
    global::music->change_volume(global::master_volume * global::music_volume);
}

void MainWindow::menu_exit() {
    this->close();
}

void MainWindow::switch_to_scene(game_scene *scene_) {
    current_scene->hide();
    current_scene = scene_;
    current_scene->show();
}

void MainWindow::draw_destination_line(QGraphicsLineItem*& line, game_scene* map_) {
    line = new QGraphicsLineItem();
    line->setPen(QPen(QBrush(QColor(Qt::red)), 4, Qt::DashLine, Qt::RoundCap));
    line->setZValue(1);
    map_->scene()->addItem(line);
}
