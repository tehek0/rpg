#include "../header/item.h"

// Проверка возможности объединить предметы в один. Используется поле _asset как временное решение (расчёт на то, что спрайты у всех предметов будут уникальными)
bool item::can_add(item &other) {
    if (_asset != other._asset || _stack + other._stack > _max_stack_size)
        return false;
    return true;
}

// Та же проверка, но ограничиваем количество нового предмета. В классе inventory используется для заполнения до полного стака
bool item::can_add(item &other, unsigned int count) {
    if (_asset != other._asset || _stack + count > _max_stack_size || other._stack < count)
        return false;
    return true;
}

// Инкремент стака предмета. Проверки на переполнение не происходит, так что использовать с осторожностью
void item::add() {
    _stack++;
}

// Добавление count предметов в стак. Проверки на переполнение не происходит, так что использовать с осторожностью
void item::add(unsigned int count) {
    _stack += count;
}

// Красивая проверка наличия count и больше предметов. Можно заменить на (item.get_stack() >= count), но так некрасиво
bool item::has_amount(unsigned int count) {
    if (_stack >= count)
        return true;
    return false;
}

// Декремент стака предмета. Проверки на равенство нулю не происходит, так что использовать с осторожностью
void item::remove() {
    _stack--;
}

// Удаление count предметов из стака. Проверки не происходит, так что использовать с осторожностью
void item::remove(unsigned int count) {
    _stack -= count;
}

// Дальше всё само собой разумеется

QString item::get_name() {
    return _name;
}

QString item::get_desc() {
    return _desc;
}

QString item::get_asset() {
    return _asset;
}

unsigned int item::get_stack() {
    return _stack;
}

unsigned int item::get_max_stack_size() {
    return _max_stack_size;
}

unsigned int item::get_base_weight() {
    return _base_weight;
}

unsigned int item::get_base_weight(unsigned int x) {
    return x * _base_weight;
}

unsigned int item::get_total_weight() {
    return _stack * _base_weight;
}

unsigned int item::get_base_cost() {
    return _base_cost;
}

bool item::get_sellable() {
    return _sellable;
}

void item::set_name(const QString& name) {
    _name = name;
}

void item::set_desc(const QString& desc) {
    _desc = desc;
}

void item::set_asset(const QString& asset) {
    _asset = asset;
}

void item::set_stack(unsigned int stack) {
    _stack = stack;
}

void item::set_max_stack_size(unsigned int max_stack_size) {
    _max_stack_size = max_stack_size;
}

void item::set_base_weight(unsigned int base_weight) {
    _base_weight = base_weight;
}

void item::set_base_cost(unsigned int base_cost) {
    _base_cost = base_cost;
}

void item::set_sellable(bool sellable) {
    _sellable = sellable;
}



item_requirements item_with_requirements::get_item_requirements() {
    return _requirements;
}

void item_with_requirements::set_item_requirements(item_requirements& requirements) {
    _requirements = requirements;
}



int weapon::get_base_dmg() {
    return _base_dmg;
}

damage_type weapon::get_damage_type() {
    return _damage_type;
}

ammo_type weapon::get_ammo_type() {
    return _ammo_type;
}

short weapon::get_energy_cost() {
    return _energy_cost;
}

void weapon::set_base_dmg(int base_dmg) {
    _base_dmg = base_dmg;
}

void weapon::set_damage_type(damage_type damage_type_) {
    _damage_type = damage_type_;
}

void weapon::set_ammo_type(ammo_type ammo_type_) {
    _ammo_type = ammo_type_;
}

void weapon::set_energy_cost(short energy_cost) {
    _energy_cost = energy_cost;
}



int ammo::get_base_dmg() {
    return _base_dmg;
}

ammo_type ammo::get_ammo_type() {
    return _ammo_type;
}

void ammo::set_base_dmg(int base_dmg) {
    _base_dmg = base_dmg;
}

void ammo::set_ammo_type(ammo_type ammo_type_) {
    _ammo_type = ammo_type_;
}


armor_slot armor::get_armor_slot() {
    return _armor_slot;
}

short armor::get_armor_points() {
    return _armor_points;
}

armor_bonus armor::get_armor_bonus() {
    return _armor_bonus;
}

void armor::set_armor_slot(armor_slot armor_slot_) {
    _armor_slot = armor_slot_;
}

void armor::set_armor_points(short armor_points) {
    _armor_points = armor_points;
}

void armor::set_armor_bonus(armor_bonus armor_bonus_) {
    _armor_bonus = armor_bonus_;
}


on_use consumable::get_on_use() {
    return _on_use;
}

unsigned short consumable::get_uses_left() {
    return _uses_left;
}

unsigned short consumable::get_use_energy_cost() {
    return _use_energy_cost;
}

void consumable::set_on_use(on_use on_use_) {
    _on_use = on_use_;
}

void consumable::set_uses_left(unsigned short uses_left) {
    _uses_left = uses_left;
}

void consumable::set_use_energy_cost(unsigned short use_energy_cost) {
    _use_energy_cost = use_energy_cost;
}

