#include "../header/visuals.h"
#include <QLabel>
#include <QPushButton>

// displayable* displayable::create(MainWindow* w, bool clickable, QRect& coord_and_size, QString& sprite_family, QString& name){
//     displayable* disp = new displayable();
//     disp->setParent(w);
//     disp->setStyleSheet(QString("border-image: url(:/%1.png);").arg(sprite_family));
//     disp->setGeometry(coord_and_size);
//     disp->setObjectName(name);
//     if (!clickable) {
//         disp->setDisabled(true);
//     }
//     disp->show();
//     return disp;
//     delete disp;
// };

// clickable::clickable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name) {
//     QPushButton* clk = new QPushButton(w);
//     clk->setStyleSheet(QString("background-image: url(:/%1.png);").arg(sprite_family));
//     clk->setGeometry(coord_and_size);
//     clk->setObjectName(name);
//     clk->show();
// }
