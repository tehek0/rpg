#include "devkitwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DevkitWindow w;
    w.show();

    return a.exec();
}
