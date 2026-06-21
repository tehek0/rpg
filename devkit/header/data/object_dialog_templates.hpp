#pragma once
#include "../create_object.hpp"

dev::object_data templates[dev::datatype::total + 1] = {
    {}, {}, {}, {}, {}, {}, {}, {}, {}, //bool - linear типы
    { //location
        {"subtype", "name", "asset"},
        {},
        {dev::datatype::TODO, dev::datatype::qstring, dev::datatype::qstring},
    },
    { //sublocation
        {},{},{},}
    ,
    { //quest
        {},{},{},}
    ,
    { //enemy
        {},{},{},}
    ,
    { //trade
        {},{},{},
},
    { //npc
        {},{},{},
},
    {// entity
        {"name", "asset", "inventory"},
        {},
        {dev::datatype::qstring, dev::datatype::qstring, dev::datatype::inventory},
    },
    { //requirements
        {"subtype","min_level", "required"},
        {},
        {dev::datatype::requirements_subtypes, dev::datatype::u_short, dev::datatype::u_integer},
     },
    { //item
        {"subtype", "name","desc","asset","max_stack_size","base_weight","base_cost","sellablle"},
        {},
        {dev::datatype::item_subtypes, dev::datatype::qstring, dev::datatype::qstring, dev::datatype::qstring,
        dev::datatype::u_integer, dev::datatype::u_integer,dev::datatype::u_integer, dev::datatype::boolean},
    },
    {{},{},{},},
};

const std::vector<dev::object_data> subtypes_template[dev::datatype::total + 1] = {
    {}, {}, {}, {}, {}, {}, {}, {}, {}, //bool - linear типы
    { //location's
        {}, //none
     { //desert
        {""},
        {},
        {},
        },
},
    { //sublocation's
     {{},{},{},},
},
    { //etc
    {
     {},
     {},
     {},
     },
},
    {
     {{},{},{},},
},
    {
     {{},{},{},},
},
    {
     {{},{},{},},
},
    {
     {{},{},{},},
},
    {// requirements
    {},//none = base requirement
    { //char requirement
     {"type"},
     {},
     {dev::datatype::char_type},
    },
    {//skill requirement
     {"type"},
     {},
     {dev::datatype::skill_type},
     },
},
    {// item's
     {},//none
     { //weapon
      {"base_dmg", "damage_type", "ammo_type", "energy_cost", "requirements"},
      {},
      {dev::datatype::integer, dev::datatype::damage_type, dev::datatype::ammo_type, dev::datatype::integer, dev::datatype::requirements},
      },
     { //ammo
      {"base_dmg", "ammo_type"},
      {},
      {dev::datatype::integer, dev::datatype::ammo_type},
      },
     { //armor
      {"armor_slot", "armor_points", "armor_bonus"},
      {},
      {dev::datatype::TODO, dev::datatype::u_short, dev::datatype::TODO},
      },
     { //consumable
     {"on_use", "uses_left", "use_energy_cost"},
      {},
      {dev::datatype::TODO, dev::datatype::short_t, dev::datatype::TODO},
      },
},
    {{{},{},{},},
},
};
