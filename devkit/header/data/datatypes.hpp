#pragma once
#include <QString>

namespace dev {
//В любом списке параметр, соответствующий данному типу объекта, находится под индекссом, указанном в datatypes
//Параметры для location всегда под индексом 0, sublocation 1 и тд
enum datatype {
    boolean, // 0
    string, // 1
    qstring, // 2
    integer, // 3
    short_t, // 4
    double_t, // 5
    u_short, // 6
    u_integer, // 7
    u_long_long, // 8
    location, // 9
    sublocation, // 10
    quest, // 11
    enemy, // 12
    trade, // 13
    npc, // 14
    entity, // 15
    requirements, // 16
    item, // 17
    ammo_type, // 18
    damage_type, // 19
    inventory, // 20
    requirements_subtypes, // 21
    item_subtypes, // 22
    char_type,
    skill_type,
    total,
    TODO,
};
//Конвертит в строку (обычно требуется для json)
const std::string datatypes_to_string[datatype::total + 2] = {
    "boolean",
    "string",
    "qstring",
    "integer",
    "short_t",
    "double_t",
    "u_short",
    "u_integer",
    "u_long_long",
    "location",
    "sublocation",
    "quest",
    "enemy",
    "trader",
    "npc",
    "entity",
    "requirements",
    "item",
};
}
