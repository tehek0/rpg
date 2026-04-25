#pragma once
#include "character.h"
// TODO: класс битвы, всякие функции ходов игрока и врагов

class battle {
protected:
    player* player_;
    std::vector<enemy*> enemies;

};
