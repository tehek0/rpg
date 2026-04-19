#pragma once
#include <QTimer>
#include <QApplication>
#include "mainwindow.h"
// Тест глобального пространства имён
class player;
namespace global {
    inline player* player_;
    inline QTimer* timer = new QTimer;
    inline const short tick_timeout = 5;
    inline int argc;
    inline char** argv;
    inline QApplication a(argc, argv);
    inline MainWindow w;
}
