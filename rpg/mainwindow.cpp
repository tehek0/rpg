#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/header/character.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton* ent = new QPushButton(this);
    ent->setStyleSheet(QString("background-image: url(:/testbox.jpg);"));
    ent->setGeometry(1000,100,100,100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

int i = 1;
void MainWindow::on_pushButton_clicked()
{
    entity(this, QRect(100,100,100,100), "testbox", QString("testbox_%1").arg("i"));
}

