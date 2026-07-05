#pragma once
#include "../create_object.hpp"

dev::object_data templates[dev::datatype::total + 1] = {
    {   {"item", "item_requirement", "requirement", "armor_bonus", "on_use"},
        {},
        {dev::datatype::item, dev::datatype::item_requirement, dev::datatype::requirement, dev::datatype::armor_bonus, dev::datatype::on_use}
    },
    {}, {}, {}, {}, {}, {}, {}, {}, {},//bool- linear типы
    { //location
        {"subtype", "name", "asset"},
        {},
        {dev::datatype::TODO, dev::datatype::qstring, dev::datatype::qstring},
    },
    {}, //map
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
        {"recieve", "offer"},
        {},
        {dev::datatype::item, dev::datatype::item},
},
    { //npc
        {},{},{},
},
    {// entity
        {"name", "asset", "inventory"},
        {},
        {dev::datatype::qstring, dev::datatype::qstring, dev::datatype::TODO},
    },
    { //item_requirement
        {"min_level", "requirements"},
        {},
        {dev::datatype::u_short, dev::datatype::requirement},
     },
    {//requirement
        {"subtype"},
        {},
        {dev::datatype::requirement_subtypes},
     },
    {//armor_bonus
        {"bonus", "value"},
        {},
        {dev::datatype::equipment_bonus, dev::datatype::integer},
     },
    {//on_use
        {"use_effects", "value"},
        {},
        {dev::datatype::use_effect, dev::datatype::integer},
     },
    { //item
        {"subtype", "name","desc","asset","max_stack_size","base_weight","base_cost","sellable"},
        {},
        {dev::datatype::item_subtypes, dev::datatype::qstring, dev::datatype::qstring, dev::datatype::qstring,
        dev::datatype::u_integer, dev::datatype::double_t,dev::datatype::u_integer, dev::datatype::boolean},
    },
};

const std::vector<dev::object_data> subtypes_template[dev::datatype::total + 1] = {
    {},
    {}, {}, {}, {}, {}, {}, {}, {}, {}, //bool - linear типы
    { //location's
        {}, //none
     {
        {},
        {},
        {},
        },
},
    {}, //map
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
    {//item requirement
},
    {//requirement
    { //char requirement
      {"required", "type"},
      {},
      {dev::datatype::u_integer, dev::datatype::char_type},
      },
    {//skill requirement
      {"required", "type"},
      {},
      {dev::datatype::u_integer, dev::datatype::skill_type},
    },
},
    {},
    {},
    {// item's
     {},//none
     { //weapon
      {"base_dmg", "damage_type", "ammo_type", "energy_cost", "item_requirement"},
      {},
      {dev::datatype::u_integer, dev::datatype::damage_type, dev::datatype::ammo_type, dev::datatype::u_integer, dev::datatype::item_requirement},
      },
     { //ammo
      {"base_dmg", "ammo_type"},
      {},
      {dev::datatype::u_integer, dev::datatype::ammo_type},
      },
     { //armor
      {"armor_slot", "armor_points", "armor_bonus", "item_requirement"},
      {},
      {dev::datatype::armor_slot, dev::datatype::u_short, dev::datatype::armor_bonus, dev::datatype::item_requirement},
      },
     { //consumable
     {"on_use", "uses_left", "energy_cost"},
      {},
      {dev::datatype::on_use, dev::datatype::u_short, dev::datatype::integer},
      },
     {},
},
};
