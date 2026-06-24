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
    { //requirement
        {"subtype", "required"},
        {},
        {dev::datatype::requirement_subtypes, dev::datatype::u_integer},
     },
    {//inventory
        {},
        {},
        {},
     },
    { //item
        {"subtype", "name","desc","asset","max_stack_size","base_weight","base_cost","sellablle"},
        {},
        {dev::datatype::item_subtypes, dev::datatype::qstring, dev::datatype::qstring, dev::datatype::qstring,
        dev::datatype::u_integer, dev::datatype::u_integer,dev::datatype::u_integer, dev::datatype::boolean},
    },
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
    {// requirement
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
    {//item requirement
     {"min_level", "requirements"},
     {},
     {dev::datatype::u_short, dev::datatype::requirement}
     },
},
    {
        {{},{},{},},
},
    {// item's
     {},//none
     { //weapon
      {"base_dmg", "damage_type", "ammo_type", "energy_cost", "requirement"},
      {},
      {dev::datatype::integer, dev::datatype::damage_type, dev::datatype::ammo_type, dev::datatype::integer, dev::datatype::requirement},
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
};
