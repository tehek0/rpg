#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/header/character.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton* ent = new QPushButton(this);
    ent->setStyleSheet(QString("border-image: url(:/icon_inv_collect_shrimp.png);"));
    ent->setGeometry(1000,100,100,100);
    ent->connect(ent, &QPushButton::clicked, this, &MainWindow::OnEntClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

int i = 1;
void MainWindow::on_pushButton_clicked()
{
    displayable disp(this, QRect(100,100,100,100), "testbox", QString("testbox_%1").arg("i"));
    //disp.blink();
}

void MainWindow::OnEntClicked() {
    qInfo() << "Кнопка нажимается";
}
