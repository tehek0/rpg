#pragma once
#include <QString>

namespace dev {
//В любом списке параметр, соответствующий данному типу объекта, находится под индекссом, указанном в datatypes
//Параметры для location всегда под индексом 0, sublocation 1 и тд
enum datatype {
    location = 0,
    sublocation = 1,
    quest = 2,
    enemy = 3,
    trader = 4,
    npc = 5,
    disp = 6,
    anim_disp = 7,
    item = 8,
    total = 9,

};
//Конвертит в строку (обычно требуется для json)
const std::string datatypes_to_string[datatype::total] = {
    "location",
    "sublocation",
    "quest",
    "enemy",
    "trader",
    "npc",
    "disp",
    "anim_disp",
    "item",
};
//Используемые типы данных (обычно используются при создании полей и чтения с них информации, чтобы понимать, каким типом что считать)
enum default_types {
    boolean,
    string,
    qstring,
    integer,
    short_t,
    double_t,
    u_short,
    u_integer,
    u_long_long,
    ammo_type,
    damage_type,
    item_subtypes,
    TODO,
};
}
