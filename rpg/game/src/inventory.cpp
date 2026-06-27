#include "../header/inventory.h"

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

size_t inventory::get_items_size() {
    return _items.size();
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
        delete _items[slot];
        _items.erase(_items.begin() + slot);
        _items.shrink_to_fit();
        emit trigger_update(slot, inv_update_context::removed_item);
        return;
    }
    _items[slot]->remove(amount);
    emit trigger_update(slot, inv_update_context::refresh_stack);
}

// Пускаем любое оружие, другие предметы идут в функцию ниже
// TODO: проверка требований
void inventory::equip_weapon(weapon* weapon_) {
    _weapon = weapon_;
}
// TODO: предупреждение о неподходящем слоте
void inventory::equip_weapon(item* not_suitable) {
    return;
}

// Пускаем любую броню и проверяем, можно ли надеть на голову, другие предметы идут в функцию ниже
// TODO: проверка требований
// TODO: бонусы экипировки
void inventory::equip_armor_head(armor* armor_) {
    if (armor_->get_armor_slot() != armor_slot::head)
        return;

    _armor.head = armor_;
}
// TODO: предупреждение о неподходящем слоте
void inventory::equip_armor_head(item* not_suitable) {
    return;
}

// Пускаем любую броню и проверяем, можно ли надеть на тело, другие предметы идут в функцию ниже
// TODO: проверка требований
// TODO: бонусы экипировки
void inventory::equip_armor_body(armor* armor_) {
    if (armor_->get_armor_slot() != armor_slot::body)
        return;

    _armor.body = armor_;
}
// TODO: предупреждение о неподходящем слоте
void inventory::equip_armor_body(item* not_suitable) {
    return;
}

// Пускаем любую броню и проверяем, можно ли надеть на ноги, другие предметы идут в функцию ниже
// TODO: проверка требований
// TODO: бонусы экипировки
void inventory::equip_armor_legs(armor* armor_) {
    if (armor_->get_armor_slot() != armor_slot::legs)
        return;

    _armor.legs = armor_;
}
// TODO: предупреждение о неподходящем слоте
void inventory::equip_armor_legs(item* not_suitable) {
    return;
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

short inventory::get_total_armor() {
    return _armor.head->get_armor_points() + _armor.body->get_armor_points() + _armor.legs->get_armor_points();
}

