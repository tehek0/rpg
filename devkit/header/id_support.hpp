#pragma once
#include "../rpg/game/inc/json.hpp"
#include "QString"
using js = nlohmann::json;

namespace dev {

enum datatype {
    location = 0,
    sublocation = 1,
    quest = 4,
    enemy = 3,
    trader = 4,
    npc = 5,
    disp = 6,
    anim_disp = 7,
    item = 8,

};

const std::string datatypes_to_string[9] = {
    {"location"},
    {"sublocation"},
    {"enemy"},
    {"quest"},
    {"trader"},
    {"npc"},
    {"disp"},
    {"anim_disp"},
    {"item"},
    };

// вспомогательные функции
void add_to_total(dev::datatype type);
const QString generate_id(dev::datatype type);

// создать объест и загрузить дату о нём в соответствующий файл
void create_enemy(dev::datatype type);

// получить объект, основываясь на данном id
auto get_object(const unsigned int& id);

}
