#pragma once
#include <QString>

namespace dev {
//В любом списке параметр, соответствующий данному типу объекта, находится под индекссом, указанном в datatypes
//При добавлении нового типа важно не смещать крайние. Например:
//Если добавляется новый линейный тип, он должетн быть после bool и до location
//Это важно для коррекности работы проверок is_linear и is_struct у info_field
enum datatype {
    erased, //Для окна удаления
    boolean, // ОСОБЫЙ
    string, // ЛИНЕЙНЫЕ
    qstring,
    integer,
    short_t,
    double_t,
    u_short, // *линейные unsigned
    u_integer,
    u_long_long,
    location, // СТРУКТУРЫ
    sublocation,
    quest,
    enemy,
    trade,
    npc,
    entity,
    requirement,
    inventory,
    armor_bonus,
    on_use,
    item, //КОНЕЦ СТРУКТУР
    ammo_type, // вот тут enum, которые как "всё остальное"
    damage_type,
    char_type,
    skill_type,
    equipment_bonus,
    armor_slot,
    use_effect,
    requirement_subtypes, //ПОДТИПЫ
    item_subtypes,
    total, // Категория вне
    TODO,
};
//Конвертит в строку (обычно требуется для json)
const std::string datatypes_to_string[datatype::total] = {
    "erased",
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
    "requirement",
    "inventory",
    "armor_bonus",
    "item",
};

// Проверк линейности. Линейным считается тип, для записи которого используются просто QLineEdit (это int, u_int, string и тд. Всё, что не bool и не кастомное)
bool is_type_linear(int type_index);
//Проверка, является ли тип структурой с кастомом внутри себя. Всё, что не линейное и не enum. Массивы
bool is_type_struct(int type_index);
//Проверка, если это подтип. Нужно для упрощение switch d fill_combo_box_data при создании info field
bool is_type_sybtype(int type_index);

bool is_type_unsigned(int type_index);
}
