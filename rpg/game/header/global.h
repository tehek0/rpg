#pragma once
#include <QTimer>
// Тест глобального пространства имён
class player;
namespace global {
    inline player* player_;
    inline QTimer* timer = new QTimer;
    inline const short tick_timeout = 5;
}
