#include "../header/visuals.h"
#include <QLabel>
#include <QPushButton>

displayable::displayable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name){
    QLabel* disp = new QLabel(w);
    disp->setStyleSheet(QString("background-image: url(:/%1.jpg);").arg(sprite_family));
    disp->setGeometry(coord_and_size);
    disp->setObjectName(name);
    disp->show();
};

clickable::clickable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name) {
    QPushButton* clk = new QPushButton(w);
    clk->setStyleSheet(QString("background-image: url(:/%1.jpg);").arg(sprite_family));
    clk->setGeometry(coord_and_size);
    clk->setObjectName(name);
    clk->show();
}
