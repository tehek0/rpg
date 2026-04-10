#pragma once
#include "inventory.h"
// Тест глобального пространства имён
namespace global {

    inline inventory* global_inventory = new inventory;

}
