#pragma once
#include <QString>

namespace dev {
//В любом списке параметр, соответствующий данному типу данных, находится под индекссом, указанном в datatypes
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
}
