#include "../header/visuals.h"
#include <QLabel>
#include <QPushButton>
#include <chrono>
#include <thread>

displayable::displayable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name){
    QLabel* disp = new QLabel(w);
    disp->setStyleSheet(QString("border-image: url(:/%1.png);").arg(sprite_family));
    disp->setGeometry(coord_and_size);
    disp->setObjectName(name);
    disp->show();
    _disp = disp;
};

clickable::clickable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name) {
    QPushButton* clk = new QPushButton(w);
    clk->setStyleSheet(QString("background-image: url(:/%1.png);").arg(sprite_family));
    clk->setGeometry(coord_and_size);
    clk->setObjectName(name);
    clk->show();
}

// void displayable::blink() {
//     _disp->hide();
//     std::this_thread::sleep_for(std::chrono::milliseconds(400));
//     _disp->show();
//     std::this_thread::sleep_for(std::chrono::milliseconds(400));
//     _disp->hide();
//     std::this_thread::sleep_for(std::chrono::milliseconds(400));
//     _disp->show();
//     std::this_thread::sleep_for(std::chrono::milliseconds(400));
//     _disp->hide();
//     std::this_thread::sleep_for(std::chrono::milliseconds(400));
//     _disp->show();
//     std::this_thread::sleep_for(std::chrono::milliseconds(400));
// }
