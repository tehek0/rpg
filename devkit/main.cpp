#include "devkitwindow.h"
#include "header/id_support.hpp"
#include <QString>
#include <QApplication>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    DevkitWindow w;
    w.show();

    // id debug
    // auto x = dev::throw_id(dev::datatype::enemy);
    // qInfo() << x;
    // x = dev::throw_id(dev::datatype::enemy);
    // x = dev::throw_id(dev::datatype::enemy);
    // x = dev::throw_id(dev::datatype::enemy);
    // x = dev::throw_id(dev::datatype::enemy);
    // dev::remove_id(dev::datatype::enemy, x - 12);
    // dev::remove_id(dev::datatype::enemy, x);
    // qInfo() << x;

    return a.exec();
}
