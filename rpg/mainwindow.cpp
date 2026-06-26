#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/header/visuals.h"
#include "game/header/config.h"
#include "game/header/ui.h"
#include <QPushButton>

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
    if (hub_scene == nullptr) {
        hub_scene = throw_hub_scene();
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
