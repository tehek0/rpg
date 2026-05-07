#include "devkitwindow.h"
#include "header/id_support.hpp"
#include "header/general.hpp"
#include <QString>
#include <fstream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DevkitWindow w;
    w.show();
    auto x = dev::throw_id(dev::datatype::enemy);
    qInfo() << x;
    x = dev::throw_id(dev::datatype::enemy);
    x = dev::throw_id(dev::datatype::enemy);
    x = dev::throw_id(dev::datatype::enemy);
    x = dev::throw_id(dev::datatype::enemy);
    dev::remove_id(dev::datatype::enemy, x - 12);
    dev::remove_id(dev::datatype::enemy, x);
    qInfo() << x;
    return a.exec();
}
