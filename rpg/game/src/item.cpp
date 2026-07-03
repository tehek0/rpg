#include "../header/item.h"
#include "../header/enum_translation.h"
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

double item::get_base_weight() {
    return _base_weight;
}

double item::get_base_weight(unsigned int x) {
    return x * _base_weight;
}

double item::get_total_weight() {
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

void item::set_base_weight(double base_weight) {
    _base_weight = base_weight;
}

void item::set_base_cost(unsigned int base_cost) {
    _base_cost = base_cost;
}

void item::set_sellable(bool sellable) {
    _sellable = sellable;
}

QString item::html_name() {
    QString html_name = QString("<p><center><font size=\"5\">%1</font></center><\p>").arg(_name);
    return html_name;
}

QString item::html_desc() {
    QString html_desc = QString("<p><center><font size=\"4\">%1</font></center><\p>").arg(_desc);
    return html_desc;
}

QString item::html_weight() {
    double total_weight = get_total_weight();
    QString html_weight = QString("<p><img src=\"assets:/pictures/ui_weight_icon.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\"><font size=\"2\"> Вес: %1%2</font><\p>").arg(_base_weight).arg((total_weight == _base_weight ? QString("") : QString(" (%1)").arg(total_weight)));
    return html_weight;
}

QString item::html_cost() {
    unsigned int total_cost = _stack * _base_cost;
    QString html_cost = QString("<p><img src=\"assets:/pictures/ui_base_cost_icon.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\"><font size=\"2\"> Стоимость: %1%2</font></p>").arg(_base_cost).arg((total_cost == _base_cost ? QString("") : QString(" (%1)").arg(total_cost)));
    return html_cost;
}

QString item::get_tooltip_text() {
    QString tooltip = html_name() + html_desc() + html_weight() + html_cost();
    return tooltip;
}

bool item::is_item_with_requirements() {
    return false;
}
bool item::is_ammo_type() {
    return false;
}
bool item::is_armor_type() {
    return false;
}
bool item::is_consumable_type() {
    return false;
}
bool item::is_weapon_type() {
    return false;
}

// Операторы равенства и неравенства для обычных предметов. Сравнивают всё, кроме количества. В наследованных класах переопределяются.
bool item::operator==(item* other) {
    if (this->is_ammo_type())
        return *(static_cast<ammo*>(this)) == other;
    if (this->is_armor_type())
        return *(static_cast<armor*>(this)) == other;
    if (this->is_consumable_type())
        return *(static_cast<consumable*>(this)) == other;
    if (this->is_weapon_type())
        return *(static_cast<weapon*>(this)) == other;


    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable)
        return false;

    return true;
}

bool item::operator!=(item* other) {
    if (this->is_ammo_type())
        return *(static_cast<ammo*>(this)) != other;
    if (this->is_armor_type())
        return *(static_cast<armor*>(this)) != other;
    if (this->is_consumable_type())
        return *(static_cast<consumable*>(this)) != other;
    if (this->is_weapon_type())
        return *(static_cast<weapon*>(this)) != other;

    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable)
        return true;

    return false;
}



item_requirements* item_with_requirements::get_item_requirements() {
    return _requirements;
}

void item_with_requirements::set_item_requirements(item_requirements* requirements) {
    _requirements = requirements;
}

QString item_with_requirements::html_requirements() {
    QString str_insert = QString("");
    if (_requirements->min_level != 0) {
        str_insert += QString(" Уровень %1, ").arg(_requirements->min_level);
    }
    for (base_requirement* req_ptr : _requirements->item_requirements_ptrs) {
        str_insert += req_ptr->text_requirement();
        str_insert += QString(", ");
    }
    if (str_insert.isEmpty())
        return str_insert;

    str_insert.removeLast().removeLast();
    QString html_requirements = QString("<p><center><font size=\"3\">Требования: %1</font></center><\p>").arg(str_insert);
    return html_requirements;
}

item_with_requirements::~item_with_requirements() {
    delete _requirements;
}

bool item_with_requirements::is_item_with_requirements() {
    return true;
}

int has_base_dmg::get_base_dmg() {
    return _base_dmg;
}

damage_type weapon::get_damage_type() {
    return _damage_type;
}

ammo_type has_ammo_type::get_ammo_type() {
    return _ammo_type;
}

short has_energy_cost::get_energy_cost() {
    return _energy_cost;
}

void has_base_dmg::set_base_dmg(int base_dmg) {
    _base_dmg = base_dmg;
}

void weapon::set_damage_type(damage_type damage_type_) {
    _damage_type = damage_type_;
}

void has_ammo_type::set_ammo_type(ammo_type ammo_type_) {
    _ammo_type = ammo_type_;
}

void has_energy_cost::set_energy_cost(short energy_cost) {
    _energy_cost = energy_cost;
}

QString has_base_dmg::html_base_dmg() {
    QString str_insert = QString("Базовый урон: %1").arg(_base_dmg);
    QString html_base_dmg = QString("<p><img src=\"assets:/pictures/ui_damage_icon.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\"><font size=\"2\"> %1</font></p>").arg(str_insert);
    return html_base_dmg;
}

QString weapon::html_base_dmg() {
    QString str_insert = QString("Базовый урон: %1").arg(_base_dmg);
    if (_damage_type != damage_type::non_specified) {
        str_insert += QString(" (%1)").arg(damage_type_to_str(_damage_type));
    }
    QString html_base_dmg = QString("<p><img src=\"assets:/pictures/ui_damage_icon.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\"><font size=\"2\"> %1</font></p>").arg(str_insert);
    return html_base_dmg;
}

QString has_ammo_type::html_ammo_type() {
    if (_ammo_type == ammo_type::none) {
        return "";
    }
    QString html_ammo_type = QString("<p><img src=\"assets:/pictures/ui_ammo_icon.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\"><font size=\"2\"> Тип боезапаса: %1</font></p>").arg(ammo_type_to_str(_ammo_type));
    return html_ammo_type;
}

QString has_energy_cost::html_energy_cost() {
    QString html_energy_cost = QString("<p><img src=\"assets:/pictures/ui_energy_cost_icon.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\"><font size=\"2\"> Стоимость хода: %1</font></p>").arg(_energy_cost);
    return html_energy_cost;
}

QString weapon::get_tooltip_text() {
    return html_name() + html_desc() + html_requirements() + html_base_dmg() + html_ammo_type() + html_energy_cost() + html_weight() + html_cost();
}

bool weapon::is_weapon_type() {
    return true;
}

bool weapon::operator==(item* other) {
    if (!other->is_weapon_type())
        return false;

    return *this == static_cast<weapon*>(other);
}

bool weapon::operator!=(item* other) {
    if (!other->is_weapon_type())
        return true;

    return *this != static_cast<weapon*>(other);
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

QString ammo::get_tooltip_text() {
    return html_name() + html_desc() + html_base_dmg() + html_ammo_type() + html_weight() + html_cost();
}


bool ammo::is_ammo_type() {
    return true;
}

bool ammo::operator==(item* other) {
    if (!other->is_ammo_type())
        return false;

    return *this == static_cast<ammo*>(other);
}

bool ammo::operator!=(item* other) {
    if (!other->is_ammo_type())
        return true;

    return *this != static_cast<ammo*>(other);
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

QString value_to_colored_text(int value) {
    return (value >= 0 ? QString("<font color=\"#0CCC3F\">+%1</font>").arg(value) : QString("<font color=\"#CC0C0C\">%1</font>").arg(value));
}

QString armor_bonus::text_armor_bonus() {
    QString str = equipment_bonus_to_str(bonus).arg(value_to_colored_text(value));
    return str;
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

QString armor::html_armor_points() {
    QString str_insert = QString("Броня: ") + armor_slot_to_str(_armor_slot) + ", " + value_to_colored_text(_armor_points);
    QString html_armor_points = QString("<p><img src=\"assets:/pictures/ui_armor_icon.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\"><font size=\"2\"> %1</font></p>").arg(str_insert);
    return html_armor_points;
}

QString armor::html_armor_bonus() {
    QString str_insert = QString("Когда надето: %1").arg(_armor_bonus.text_armor_bonus());
    QString html_armor_bonus = QString("<p><img src=\"assets:/pictures/ui_armor_bonus_icon.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\"><font size=\"2\"> %1</font></p>").arg(str_insert);
    return html_armor_bonus;
}

QString armor::get_tooltip_text() {
    return html_name() + html_desc() + html_requirements() + html_armor_points() + html_armor_bonus() + html_weight() + html_cost();
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


bool armor::is_armor_type() {
    return true;
}

bool armor::operator==(item* other) {
    if (!other->is_armor_type())
        return false;

    return *this == static_cast<armor*>(other);
}

bool armor::operator!=(item* other) {
    if (!other->is_armor_type())
        return true;

    return *this != static_cast<armor*>(other);
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


QString on_use::text_use_effect() {
    QString str = use_effect_to_str(effect).arg(value_to_colored_text(value));
    return str;
}

QString consumable::html_on_use() {
    QString str_insert = QString("Когда использовано: %1 (Осталось: %2)").arg(_on_use.text_use_effect()).arg(_uses_left);
    QString html_on_use = QString("<p><img src=\"assets:/pictures/ui_use_effect_icon.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\"><font size=\"2\"> %1</font></p>").arg(str_insert);
    return html_on_use;
}

QString consumable::get_tooltip_text() {
    return html_name() + html_desc() + html_on_use() + html_energy_cost() + html_weight() + html_cost();
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

void consumable::set_on_use(on_use on_use_) {
    _on_use = on_use_;
}

void consumable::set_uses_left(unsigned short uses_left) {
    _uses_left = uses_left;
}

bool consumable::is_consumable_type() {
    return true;
}

bool consumable::operator==(item* other) {
    if (!other->is_consumable_type())
        return false;

    return *this == static_cast<consumable*>(other);
}

bool consumable::operator!=(item* other) {
    if (!other->is_consumable_type())
        return true;

    return *this != static_cast<consumable*>(other);
}

bool consumable::operator==(consumable* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable || this->_on_use != other->_on_use || this->_uses_left != other->_uses_left || this->_energy_cost != other->_energy_cost)
        return false;

    return true;
}

bool consumable::operator!=(consumable* other) {
    if (this->_name != other->_name || this->_desc != other->_desc || this->_asset != other->_asset || this->_max_stack_size != other->_max_stack_size || this->_base_weight != other->_base_weight || this->_base_cost != other->_base_cost || this->_sellable != other->_sellable || this->_on_use != other->_on_use || this->_uses_left != other->_uses_left || this->_energy_cost != other->_energy_cost)
        return true;

    return false;
}
