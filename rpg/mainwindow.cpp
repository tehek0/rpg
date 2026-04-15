#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/header/character.h"
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
    connect(ent->_disp, &QPushButton::clicked, this, [this]{OnEntClicked();});
    on_screen.emplace_back(ent->_disp);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int ii = 30;
void MainWindow::on_pushButton_clicked()
{
    QRect v(100+ii,100,100,100);
    QString s = "icon_inv_collect_shrimp";
    QString n = "test";
    ii += 10;
    displayable* ent = new displayable(this,true,v,s,n);
    connect(ent->_disp, &QPushButton::clicked, this, [this]{OnEntClicked();});
    on_screen.emplace_back(ent->_disp);
}

void MainWindow::OnEntClicked() {
    qInfo() << "Кнопка нажимается";
}

void MainWindow::on_map_b_clicked()
{
    centralWidget()->setStyleSheet("background: url(:/map.jpg); background-position: center;");
    for(size_t i = 0; i < on_screen.size(); ++i) {
        on_screen[i]->show();
    }

}

void MainWindow::on_inventory_b_clicked()
{
    centralWidget()->setStyleSheet("background: url(:/testbkg.jpg); background-position: center;");
    for(size_t i = 0; i < on_screen.size(); ++i) {
        on_screen[i]->hide();
    }
}

