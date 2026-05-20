#pragma once
#include "../create_object.hpp"

dev::object_data templates[dev::datatype::total + 1] = {
    {
        {"subtype", "name", "asset"},
        {},
        {dev::default_types::TODO, dev::default_types::qstring, dev::default_types::qstring},
    },
    {{},{},{},},
    {{},{},{},},
    {{},{},{},},
    {{},{},{},},
    {{},{},{},},
    {// entity
        {"name", "asset", "inventory"},
        {},
        {dev::default_types::qstring, dev::default_types::qstring, dev::default_types::inventory},
    },
    {{},{},{},},
    { //item
        {"subtype", "name","desc","asset","max_stack_size","base_weight","base_cost","sellablle"},
        {},
        {dev::default_types::item_subtypes, dev::default_types::qstring, dev::default_types::qstring, dev::default_types::qstring,
        dev::default_types::u_integer, dev::default_types::u_integer,dev::default_types::u_integer, dev::default_types::boolean},
    },
    {{},{},{},},
};

const std::vector<dev::object_data> subtypes_template[dev::datatype::total + 1] = {
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
    {
     {{},{},{},},
},
    {
     {{},{},{},},
},
    {// item's
     {},//none
     { //weapon
      {"base_dmg", "damage_type", "ammo_type", "energy_cost", "requirements"},
      {},
      {dev::default_types::integer, dev::default_types::damage_type, dev::default_types::ammo_type, dev::default_types::integer, dev::default_types::requirements},
      },
     { //ammo
      {"base_dmg", "ammo_type"},
      {},
      {dev::default_types::integer, dev::default_types::ammo_type},
      },
     { //armor
      {"armor_slot", "armor_points", "armor_bonus"},
      {},
      {dev::default_types::TODO, dev::default_types::u_short, dev::default_types::TODO},
      },
     { //consumable
     {"on_use", "uses_left", "use_energy_cost"},
      {},
      {dev::default_types::TODO, dev::default_types::short_t, dev::default_types::TODO},
      },
},
    {
        {{},{},{},},
},
};
const dev::object_data substructures_template[dev::default_types::TODO - dev::default_types::u_long_long] {
    {//requirements
     {},{},{},
    },
    {//inventory
        {"item_id"},
        {},
        {dev::default_types::u_long_long},
    },
};
