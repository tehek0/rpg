#include "../header/inventory.h"
#include "../header/global.h"
#include "../header/character.h"
#include "../header/message.h"
// Избегаем утечку памяти
inventory::~inventory() {
    for (size_t i = 0; i < _items.size(); ++i) {
        delete _items[i];
    }
}

std::vector<item*> inventory::get_items() {
    return _items;
}

// Добавление предмета
// TODO: изменение общего веса
void inventory::add_item(item* item_) {
    // Пробегаем по вектору в поисках предмета, к которому можно присосаться
    emit change_weight(item_->get_base_weight() * item_->get_stack());
    for (size_t i = 0; i < _items.size(); ++i) {
        // Не имеет смысла искать, если максимальный стак = 1
        if (item_->get_max_stack_size() == 1) {
            break;
        }
        if (_items[i]->can_add(item_) == true) {
            // Нашли предмет, к которому можно полностью добавить всё количество без переполнения
            _items[i]->add(item_->get_stack());
            // Избавляемся от переданного предмета
            delete item_;
            emit trigger_update(i, inv_update_context::refresh_stack);
            return;
        } else if (_items[i]->can_add(item_,_items[i]->get_max_stack_size() - _items[i]->get_stack())) {
            // Может, можно хотя бы кусочек добавить? Можно? Круто.
            item_->set_stack(item_->get_stack() - (_items[i]->get_max_stack_size() - _items[i]->get_stack()));
            _items[i]->set_stack(_items[i]->get_max_stack_size());
            emit trigger_update(i, inv_update_context::refresh_stack);
            continue;
        }
    }
    _items.reserve(_items.size() + 1);
    _items.emplace_back(item_);
    emit trigger_update(_items.size() - 1, inv_update_context::added_item);
}

item* inventory::get_item(unsigned int slot) {
    if (slot >= _items.size()) {
        throw std::exception("[inventory::get_item] reached end of vector");
    }
    return _items[slot];
}
item* inventory::get_equal_item(item *item_) {
    for (auto const itm: _items) {
        if (*(itm) == item_) {
            return itm;
        }
    }
    return nullptr;
}
size_t inventory::get_items_size() {
    return _items.size();
}

bool inventory::has_item(item *item_) {
    for (auto const itm: _items) {
        if (itm == item_) {
            return true;
        }
    }
    return false;
}

unsigned int inventory::get_slot(item* item_) {
    unsigned int slot = 0;
    for (auto const itm : _items) {
        if (itm == item_)
            return slot;
        ++slot;
    }
}

// TODO: изменение общего веса
void inventory::remove_item(unsigned int slot) {
    if (slot >= _items.size()) {
        throw std::exception("[inventory::remove_item] reached end of vector");
    }
    if (is_equipped(_items[slot])) {
        deequip(_items[slot]);
    }
    emit change_weight(_items[slot]->get_base_weight() * _items[slot]->get_stack() * (-1));
    delete _items[slot];
    _items.erase(_items.begin() + slot);
    _items.shrink_to_fit();
    emit trigger_update(slot, inv_update_context::removed_item);
}

void inventory::remove_item(unsigned int slot, unsigned int amount) {
    if (slot >= _items.size()) {
        throw std::exception("[inventory::remove_item] reached end of vector");
    }
    if (amount >= _items[slot]->get_stack()) {
        if (is_equipped(_items[slot])) {
            deequip(_items[slot]);
        }
        emit change_weight(_items[slot]->get_base_weight() * _items[slot]->get_stack() * (-1));
        delete _items[slot];
        _items.erase(_items.begin() + slot);
        _items.shrink_to_fit();
        emit trigger_update(slot, inv_update_context::removed_item);
        return;
    }
    emit change_weight(_items[slot]->get_base_weight() * amount * (-1));
    _items[slot]->remove(amount);
    emit trigger_update(slot, inv_update_context::refresh_stack);
}

// Пускаем любое оружие, другие предметы идут в функцию ниже
// TODO: проверка требований
void inventory::equip_weapon(weapon* weapon_) {
    if (!this->has_item(weapon_))
        return;
    if (!weapon_->get_item_requirements()->match_all(global::player_->get_total_stats())) {
        new screen_message(QString("> Оружие \"%1\" не экипировано: Требования не удовлетворены").arg(weapon_->get_name()), 300, 100, 20);
        return;
    }
    if (_weapon != nullptr) {
        emit trigger_update(this->get_slot(_weapon), inv_update_context::deequipped);
    }
    if (_equipped_ammo != nullptr) {
        if (_equipped_ammo->get_ammo_type() != weapon_->get_ammo_type())
            deequip(_equipped_ammo);
    }
    _weapon = weapon_;
    emit trigger_update(this->get_slot(weapon_), inv_update_context::equipped);
}
// TODO: предупреждение о неподходящем слоте
void inventory::equip_weapon(item* not_suitable) {
    if (not_suitable->is_weapon_type()) {
        equip_weapon(static_cast<weapon*>(not_suitable));
    }
}

void inventory::equip_ammo(ammo* ammo_) {
    if (!this->has_item(ammo_))
        return;
    if (_weapon == nullptr) {
        new screen_message(QString("> Боезапас \"%1\" не экипирован: Нет экипированного оружия").arg(ammo_->get_name()), 300, 100, 20);
        return;
    }
    if (_weapon->get_ammo_type() == ammo_type::none) {
        new screen_message(QString("> Боезапас \"%1\" не экипирован: Экипированному оружию не нужны боеприпасы").arg(ammo_->get_name()), 300, 100, 20);
        return;
    }
    if (_weapon->get_ammo_type() != ammo_->get_ammo_type()) {
        new screen_message(QString("> Боезапас \"%1\" не экипирован: Неподходящий тип боеприпасов").arg(ammo_->get_name()), 300, 100, 20);
        return;
    }
    if (_equipped_ammo != nullptr) {
        emit trigger_update(this->get_slot(_equipped_ammo), inv_update_context::deequipped);
    }
    _equipped_ammo = ammo_;
    emit trigger_update(this->get_slot(ammo_), inv_update_context::equipped);
}
// TODO: предупреждение о неподходящем слоте
void inventory::equip_ammo(item* not_suitable) {
    if (not_suitable->is_ammo_type()) {
        equip_weapon(static_cast<ammo*>(not_suitable));
    }
}

void inventory::equip_consumable(consumable* consumable_) {
    if (!this->has_item(consumable_))
        return;
    if (_equipped_consumable != nullptr) {
        emit trigger_update(this->get_slot(_equipped_consumable), inv_update_context::deequipped);
    }
    _equipped_consumable = consumable_;
    emit trigger_update(this->get_slot(consumable_), inv_update_context::equipped);
}
// TODO: предупреждение о неподходящем слоте
void inventory::equip_consumable(item* not_suitable) {
    if (not_suitable->is_consumable_type()) {
        equip_weapon(static_cast<consumable*>(not_suitable));
    }
}

// Пускаем любую броню и проверяем, можно ли надеть на голову, другие предметы идут в функцию ниже
// TODO: проверка требований
// TODO: бонусы экипировки
void inventory::equip_armor_head(armor* armor_) {
    if (!this->has_item(armor_))
        return;
    if (!armor_->get_item_requirements()->match_all(global::player_->get_total_stats())) {
        new screen_message(QString("> Шлем \"%1\" не экипирован: Требования не удовлетворены").arg(armor_->get_name()), 300, 100, 20);
        return;
    }

    if (armor_->get_armor_slot() != armor_slot::head)
        return;
    if (_armor.head != nullptr) {
        emit trigger_update(this->get_slot(_armor.head), inv_update_context::deequipped);
    }
    _armor.head = armor_;
    emit trigger_update(this->get_slot(armor_), inv_update_context::equipped);
}
// TODO: предупреждение о неподходящем слоте
void inventory::equip_armor_head(item* not_suitable) {
    if (not_suitable->is_armor_type()) {
        equip_armor_head(static_cast<armor*>(not_suitable));
    }
}

// Пускаем любую броню и проверяем, можно ли надеть на тело, другие предметы идут в функцию ниже
// TODO: проверка требований
// TODO: бонусы экипировки
void inventory::equip_armor_body(armor* armor_) {
    if (!this->has_item(armor_))
        return;
    if (!armor_->get_item_requirements()->match_all(global::player_->get_total_stats())) {
        new screen_message(QString("> Нагрудник \"%1\" не экипирован: Требования не удовлетворены").arg(armor_->get_name()), 300, 100, 20);
        return;
    }
    if (armor_->get_armor_slot() != armor_slot::body)
        return;
    if (_armor.body != nullptr) {
        emit trigger_update(this->get_slot(_armor.body), inv_update_context::deequipped);
    }
    _armor.body = armor_;
    emit trigger_update(this->get_slot(armor_), inv_update_context::equipped);
}
// TODO: предупреждение о неподходящем слоте
void inventory::equip_armor_body(item* not_suitable) {
    if (not_suitable->is_armor_type()) {
        equip_armor_body(static_cast<armor*>(not_suitable));
    }
}

// Пускаем любую броню и проверяем, можно ли надеть на ноги, другие предметы идут в функцию ниже
// TODO: проверка требований
// TODO: бонусы экипировки
void inventory::equip_armor_legs(armor* armor_) {
    if (!this->has_item(armor_))
        return;
    if (!armor_->get_item_requirements()->match_all(global::player_->get_total_stats())) {
        new screen_message(QString("> Поножи \"%1\" не экипированы: Требования не удовлетворены").arg(armor_->get_name()), 300, 100, 20);
        return;
    }
    if (armor_->get_armor_slot() != armor_slot::legs)
        return;
    if (_armor.legs != nullptr) {
        emit trigger_update(this->get_slot(_armor.legs), inv_update_context::deequipped);
    }
    _armor.legs = armor_;
    emit trigger_update(this->get_slot(armor_), inv_update_context::equipped);
}
// TODO: предупреждение о неподходящем слоте
void inventory::equip_armor_legs(item* not_suitable) {
    if (not_suitable->is_armor_type()) {
        equip_armor_legs(static_cast<armor*>(not_suitable));
    }
}

bool inventory::is_equipped(item* item_) {
    return (item_ == _armor.head || item_ == _armor.body || item_ == _armor.legs || item_ == _weapon || item_ == _equipped_ammo || item_ == _equipped_consumable);
}

void inventory::deequip(item* item_) {
    if (!has_item(item_) || !is_equipped(item_))
        return;
    if (item_->is_weapon_type()) {
        _weapon = nullptr;
    } else if (item_->is_ammo_type()) {
        _equipped_ammo = nullptr;
    } else if (item_->is_consumable_type()) {
        _equipped_consumable = nullptr;
    } else if (item_->is_armor_type()) {
        deequip_armor(static_cast<armor*>(item_));
    } else {
        return;
    }
    emit trigger_update(get_slot(item_), inv_update_context::deequipped);
}

void inventory::deequip_armor(armor *armor_) {
    switch(armor_->get_armor_slot()) {
        case armor_slot::head: {
            _armor.head = nullptr;
            break;
        }
        case armor_slot::body: {
            _armor.body = nullptr;
            break;
        }
        case armor_slot::legs: {
            _armor.legs = nullptr;
            break;
        }
        default: {
            return;
        }
    }
}

void inventory::equip(item *item_) {
    if (!has_item(item_) || is_equipped(item_)) {
        return;
    }
    if (item_->is_weapon_type()) {
        equip_weapon(static_cast<weapon*>(item_));
    } else if (item_->is_ammo_type()) {
        equip_ammo(static_cast<ammo*>(item_));
    } else if (item_->is_consumable_type()) {
        equip_consumable(static_cast<consumable*>(item_));
    } else if (item_->is_armor_type()) {
        equip_armor(static_cast<armor*>(item_));
    } else {
        return;
    }
}

void inventory::equip_unconditional(item *item_) {
    if (!has_item(item_) || is_equipped(item_)) {
        return;
    }
    if (item_->is_weapon_type()) {
        if (_weapon != nullptr) {
            emit trigger_update(this->get_slot(_weapon), inv_update_context::deequipped);
        }
        _weapon = static_cast<weapon*>(item_);
        emit trigger_update(this->get_slot(_weapon), inv_update_context::equipped);
    } else if (item_->is_ammo_type()) {
        if (_equipped_ammo != nullptr) {
            emit trigger_update(this->get_slot(_equipped_ammo), inv_update_context::deequipped);
        }
        _equipped_ammo = static_cast<ammo*>(item_);
        emit trigger_update(this->get_slot(_equipped_ammo), inv_update_context::equipped);
    } else if (item_->is_consumable_type()) {
        if (_equipped_consumable != nullptr) {
            emit trigger_update(this->get_slot(_equipped_consumable), inv_update_context::deequipped);
        }
        _equipped_consumable = static_cast<consumable*>(item_);
        emit trigger_update(this->get_slot(_equipped_consumable), inv_update_context::equipped);
    } else if (item_->is_armor_type()) {
        switch(static_cast<armor*>(item_)->get_armor_slot()) {
        case armor_slot::head: {
            if (_armor.head != nullptr) {
                emit trigger_update(this->get_slot(_armor.head), inv_update_context::deequipped);
            }
            _armor.head = static_cast<armor*>(item_);
            emit trigger_update(this->get_slot(item_), inv_update_context::equipped);
            break;
        }
        case armor_slot::body: {
            if (_armor.body != nullptr) {
                emit trigger_update(this->get_slot(_armor.body), inv_update_context::deequipped);
            }
            _armor.body = static_cast<armor*>(item_);
            emit trigger_update(this->get_slot(item_), inv_update_context::equipped);
            break;
        }
        case armor_slot::legs: {
            if (_armor.legs != nullptr) {
                emit trigger_update(this->get_slot(_armor.legs), inv_update_context::deequipped);
            }
            _armor.legs = static_cast<armor*>(item_);
            emit trigger_update(this->get_slot(item_), inv_update_context::equipped);
            break;
        }
        default: {
            return;
        }
        }
    } else {
        return;
    }
}

void inventory::equip_armor(armor *armor_) {
    switch(armor_->get_armor_slot()) {
    case armor_slot::head: {
        equip_armor_head(armor_);
        break;
    }
    case armor_slot::body: {
        equip_armor_body(armor_);
        break;
    }
    case armor_slot::legs: {
        equip_armor_legs(armor_);
        break;
    }
    default: {
        return;
    }
    }
}

armor* inventory::get_armor_head() {
    return _armor.head;
}

armor* inventory::get_armor_body() {
    return _armor.body;
}

armor* inventory::get_armor_legs() {
    return _armor.legs;
}

weapon* inventory::get_weapon() {
    return _weapon;
}

ammo* inventory::get_equipped_ammo() {
    return _equipped_ammo;
}

consumable* inventory::get_equipped_consumable() {
    return _equipped_consumable;
}

short inventory::get_total_armor() {
    short head = 0;
    if (_armor.head != nullptr) {
        head = _armor.head->get_armor_points();
    }
    short body = 0;
    if (_armor.body != nullptr) {
        body = _armor.body->get_armor_points();
    }
    short legs = 0;
    if (_armor.legs != nullptr) {
        legs = _armor.legs->get_armor_points();
    }
    return head + body + legs;
}

