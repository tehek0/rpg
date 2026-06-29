#pragma once
#include <QPushButton>
#include "../inc/alias.h"
#include <QString>
// TODO: класс для чтения и хранения всех данных
class map_widget;
class player;
class item;
class ammo;
class armor;
class consumable;
class weapon;
class item_requirements;
class base_requirement;
class entity_stats;
class entity_level;
class armor_bonus;
class on_use;

void critical_error(const QString& message);

void saves_init(int amount_of_saves = 3);
void save_map(map_widget* map, int slot);
bool load_map(map_widget* map, int slot);
void save_player(player* player_, int slot);
bool load_player(player* player_, int slot);

json json_from_entity_stats(entity_stats stats);
entity_stats entity_stats_from_json(json j);
json json_from_entity_level(entity_level _entity_level);
entity_level entity_level_from_json(json j);

json json_from_item(item* item_);
void add_ammo_keys(json& j, ammo* ammo_);
void add_armor_keys(json& j, armor* armor_);
void add_consumable_keys(json& j, consumable* consumable_);
void add_weapon_keys(json& j, weapon* weapon_);
void add_requirements_keys(json& j, item_requirements* requirements);

item* item_from_json(json j);
item* item_from_id(unsigned long long id);
item_requirements* requirements_from_json(json j);
item_requirements* requirements_from_id(unsigned long long id);
base_requirement* single_requirement_from_json(json j);
base_requirement* single_requirement_from_id(unsigned long long id);
armor_bonus armor_bonus_from_json(json j);
armor_bonus armor_bonus_from_id(unsigned long long id);
on_use on_use_from_json(json j);
on_use on_use_from_id(unsigned long long id);

class slot: public QPushButton {

};
