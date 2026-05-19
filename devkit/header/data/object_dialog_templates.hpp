#pragma once
#include "../create_object.hpp"

dev::object_data templates[dev::datatype::total + 1] = {
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    { //item
     {"subtype", "name","desc","asset","max_stack_size","base_weight","base_cost","sellablle"},
     {},
     {dev::default_types::item_subtypes, dev::default_types::qstring, dev::default_types::qstring, dev::default_types::qstring,
      dev::default_types::u_integer, dev::default_types::u_integer,dev::default_types::u_integer, dev::default_types::boolean},
     },
    {},
};

const std::vector<dev::object_data> subtypes_template[dev::datatype::total + 1] = {
    { //location's
     {{},{},{},},
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
      {dev::default_types::integer, dev::default_types::damage_type, dev::default_types::ammo_type, dev::default_types::integer, dev::default_types::TODO},
      },
     { //ammo
      {"base_dmg", "ammo_type"},
      {},
      {dev::default_types::integer, dev::default_types::ammo_type},
      },
     { //armor
      {"armor_slot", "armor_points", "armor_bonus"},
      {},
      {dev::default_types::TODO, dev::default_types::u_short, dev::default_types::u_short},
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
