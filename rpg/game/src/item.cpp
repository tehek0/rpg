#include "../header/item.h"

// Проверка возможности объединить предметы в один.
bool item::can_add(item* other) {
    if (*this != other || _stack + other->_stack > _max_stack_size)
        return false;
    return true;
}

// Та же проверка, но ограничиваем количество нового предмета. В классе inventory используется для заполнения до полного стака
bool item::can_add(item* other, unsigned int count) {
    if (*this != other || _stack + count > _max_stack_size || other->_stack < count)
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

float item::get_base_weight() {
    return _base_weight;
}

float item::get_base_weight(unsigned int x) {
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

void item::set_base_weight(float base_weight) {
    _base_weight = base_weight;
}

void item::set_base_cost(unsigned int base_cost) {
    _base_cost = base_cost;
}

void item::set_sellable(bool sellable) {
    _sellable = sellable;
}

// Операторы равенства и неравенства для обычных предметов. Сравнивают всё, кроме количества. В наследованных класах переопределяются.
bool item::operator==(item* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable)
        return false;

    return true;
}

bool item::operator!=(item* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable)
        return true;

    return false;
}



item_requirements item_with_requirements::get_item_requirements() {
    return _requirements;
}

void item_with_requirements::set_item_requirements(item_requirements& requirements) {
    _requirements = requirements;
}


bool item_with_requirements::operator==(item* other) {
    return false;
}

bool item_with_requirements::operator!=(item* other) {
    return true;
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

bool weapon::operator==(weapon* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable || this->_requirements != other->_requirements || this->_base_dmg != other->_base_dmg || this->_ammo_type != other->_ammo_type || this->_damage_type != other->_damage_type || this->_energy_cost != other->_energy_cost)
        return false;

    return true;
}

bool weapon::operator!=(weapon* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable || this->_requirements != other->_requirements || this->_base_dmg != other->_base_dmg || this->_ammo_type != other->_ammo_type || this->_damage_type != other->_damage_type || this->_energy_cost != other->_energy_cost)
        return true;

    return false;
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

bool ammo::operator==(item* other) {
    return false;
}

bool ammo::operator!=(item* other) {
    return true;
}

bool ammo::operator==(ammo* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable || this->_base_dmg != other->_base_dmg || this->_ammo_type != other->_ammo_type)
        return false;

    return true;
}

bool ammo::operator!=(ammo* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable || this->_base_dmg != other->_base_dmg || this->_ammo_type != other->_ammo_type)
        return true;

    return false;
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

bool armor_bonus::operator==(armor_bonus& other) {
    if (this->bonus != other.bonus || this->value != other.value)
        return false;

    return true;
}

bool armor_bonus::operator!=(armor_bonus& other) {
    if (this->bonus != other.bonus || this->value != other.value)
        return true;

    return false;
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

bool armor::operator==(armor* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable || this->_requirements != other->_requirements || this->_armor_slot != other->_armor_slot || this->_armor_points != other->_armor_points || this->_armor_bonus != other->_armor_bonus)
        return false;

    return true;
}

bool armor::operator!=(armor* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable || this->_requirements != other->_requirements || this->_armor_slot != other->_armor_slot || this->_armor_points != other->_armor_points || this->_armor_bonus != other->_armor_bonus)
        return true;

    return false;
}


on_use consumable::get_on_use() {
    return _on_use;
}

bool on_use::operator==(on_use& other) {
    if (this->effect != other.effect || this->value != other.value)
        return false;

    return true;
}

bool on_use::operator!=(on_use& other) {
    if (this->effect != other.effect || this->value != other.value)
        return true;

    return false;
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

bool consumable::operator==(item* other) {
    return false;
}

bool consumable::operator!=(item* other) {
    return true;
}

bool consumable::operator==(consumable* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable || this->_on_use != other->_on_use || this->_uses_left != other->_uses_left || this->_use_energy_cost != other->_use_energy_cost)
        return false;

    return true;
}

bool consumable::operator!=(consumable* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable || this->_on_use != other->_on_use || this->_uses_left != other->_uses_left || this->_use_energy_cost != other->_use_energy_cost)
        return true;

    return false;
}
