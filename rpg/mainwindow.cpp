#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/header/character.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPushButton* ent = new QPushButton(this);
    ent->setStyleSheet(QString("background-image: url(:/sprite/testbox.jpg);"));
    ent->setGeometry(1000,100,100,100);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    entity(this,"testbox");
}

