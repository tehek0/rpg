#include "devkitwindow.h"
<<<<<<< Updated upstream

=======
#include "header/id_support.hpp"
#include "header/general.hpp"
#include <QString>
>>>>>>> Stashed changes
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DevkitWindow w;
    w.show();

<<<<<<< Updated upstream
=======
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

>>>>>>> Stashed changes
    return a.exec();
}
