#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/header/character.h"
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QRect v(1000,100,100,100);
    QString s = "icon_inv_collect_shrimp";
    QString n = "test";
    displayable* ent = new displayable(this,true,v,s,n);
    connect(ent, &QPushButton::clicked, this, [this]{OnEntClicked();});

}

MainWindow::~MainWindow()
{
    delete ui;
}

int i = 1;
void MainWindow::on_pushButton_clicked()
{
    entity();
}

void MainWindow::OnEntClicked() {
    qInfo() << "Кнопка нажимается";
}
