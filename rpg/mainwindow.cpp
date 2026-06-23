#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/header/visuals.h"
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
    // global::music->get_player()->setPosition(0);
    qInfo() << global::music->get_player()->duration() << global::music->get_player()->position() << global::music->get_player()->source();
}

void MainWindow::menu_settings() {

}

void MainWindow::menu_exit() {
    this->close();
}
