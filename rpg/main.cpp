#include "mainwindow.h"
#include <QApplication>
//#include <cstdlib>
//#include <ctime>
//#include "game/header/inventory.h"
//#include "game/header/global.h"

int main(int argc, char *argv[])
{

    //std::srand(std::time(0));
    QApplication a(argc, argv);
    MainWindow w;
    //w.showFullScreen();
    w.show();
    return a.exec();
}
