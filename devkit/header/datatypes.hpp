#pragma once
#include <QString>

namespace dev {
const short amount_of_datatypes = 9;

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

};

const std::string datatypes_to_string[amount_of_datatypes] = {
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

//Файлы с информацией
const std::string paths[amount_of_datatypes] = {
    "../../../data/location.json",
    "./../../data/sublocation.json",
    "../../../data/quest.json",
    "../../../data/enemy.json",
    "../../../data/trader.json",
    "../../../data/npc.json",
    "../../../data/disp.json",
    "../../../data/anim_disp.json",
    "../../../data/item.json",

};
}
