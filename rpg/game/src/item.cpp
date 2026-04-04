#include "../header/item.h"

namespace item {
bool can_add(item &other) {
    if (_asset != other._asset || _stack + other._stack > _max_stack_size)
        return false;
    return true;
}

bool can_add(item &other, unsigned int count) {
    if (_asset != other._asset || _stack + count > _max_stack_size || other._stack < count)
        return false;
    return true;
}

void add() {
    _stack++;
}

void add(unsigned int count) {
    _stack += count;
}

bool has_amount(unsigned int count) {
    if (_stack >= count)
        return true;
    return false;
}

void remove() {
    _stack--;
}

void remove(unsigned int count) {
    _stack -= count;
}

QString get_name() {
    return _name;
}

QString get_desc() {
    return _desc;
}

QString get_asset() {
    return _asset;
}

unsigned int get_stack() {
    return _stack;
}

unsigned int get_max_stack_size() {
    return _max_stack_size;
}

unsigned int get_base_weight() {
    return _base_weight;
}

unsigned int get_base_weight(unsigned int x) {
    return x * _base_weight;
}

unsigned int get_total_weight() {
    return _stack * _base_weight;
}

unsigned int get_base_cost() {
    return _base_cost;
}

bool get_sellable() {
    return _sellable;
}

void set_name(const QString& name) {
    _name = name;
}

void set_desc(const QString& desc) {
    _desc = desc;
}

void set_asset(const QString& asset) {
    _asset = asset;
}

void set_stack(unsigned int stack) {
    _stack = stack;
}

void set_max_stack_size(unsigned int max_stack_size) {
    _max_stack_size = max_stack_size;
}

void set_base_weight(unsigned int base_weight) {
    _base_weight = base_weight;
}

void set_base_cost(unsigned int base_cost) {
    _base_cost = base_cost;
}

void set_sellable(bool sellable) {
    _sellable = sellable;
}
}

namespace item_with_requirements {
item_requirements get_item_requirements() {
    return _requirements;
}

void set_item_requirements(item_requirements& requirements) {
    _requirements = requirements;
}
}

namespace weapon {
int get_base_dmg() {
    return _base_dmg;
}

damage_type get_damage_type() {
    return _damage_type;
}

ammo_type get_ammo_type() {
    return _ammo_type;
}

short get_energy_cost() {
    return _energy_cost;
}

void set_base_dmg(int base_dmg) {
    _base_dmg = base_dmg;
}

void set_damage_type(damage_type damage_type_) {
    _damage_type = damage_type_;
}

void set_ammo_type(ammo_type ammo_type_) {
    _ammo_type = ammo_type_;
}

void set_energy_cost(short energy_cost) {
    _energy_cost = energy_cost;
}
}

namespace ammo {
int get_base_dmg() {
    return _base_dmg;
}

ammo_type get_ammo_type() {
    return _ammo_type;
}

void set_base_dmg(int base_dmg) {
    _base_dmg = base_dmg;
}

void set_ammo_type(ammo_type ammo_type_) {
    _ammo_type = ammo_type_;
}
}

namespace armor {
armor_slot get_armor_slot() {
    return _armor_slot;
}

short get_armor_points() {
    return _armor_points;
}

armor_bonus get_armor_bonus() {
    return _armor_bonus;
}

void set_armor_slot(armor_slot armor_slot_) {
    _armor_slot = armor_slot_;
}

void set_armor_points(short armor_points) {
    _armor_point = armor_points;
}

void set_armor_bonus(armor_bonus armor_bonus_) {
    _armor_bonus = armor_bonus_;
}
}

namespace consumable {
on_use get_on_use() {
    return _on_use;
}

unsigned short get_uses_left() {
    return _uses_left();
}

unsigned short get_use_energy_cost() {
    return _use_energy_cost;
}

void set_on_use(on_use on_use_) {
    _on_use = on_use_;
}

void set_uses_left(unsigned short uses_left) {
    _uses_left = uses_left;
}

void set_use_energy_cost(unsigned short use_energy_cost) {
    _use_energy_cost = use_energy_cost;
}
}
