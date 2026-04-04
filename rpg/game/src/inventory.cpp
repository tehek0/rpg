#include "../header/inventory.h"

namespace inventory {
~inventory() {
    for (size_t i = 0; i < _items.size(); ++i) {
        delete _items[i];
    }
}

std::Vector<item*> get_items() {
    return _items;
}

void add_item(item* item_) {
    for (size_t i = 0; i < _items.size(); ++i) {
        if (_items[i]->can_add(item_)) {
            _items[i]->add(item_->get_stack());
            delete item_;
            return;
        } else if (_items[i]->can_add(item_,_items[i]->get_max_stack_size() - _items[i]->get_stack())) {
            item_->set_stack(item_->get_stack() - (_items[i]->get_max_stack_size() - _items[i]->get_stack()));
            _items[i]->set_stack(_items[i]->get_max_stack_size());
            continue;
        }
    }
    _items.reserve(_items.size() + 1);
    _items.emplace_back(item_);
}

item* get_item(unsigned int slot) {
    if (slot >= _items.size()) {
        throw std::exception("[inventory::get_item] reached end of vector");
    }
    return _items[slot];
}

void remove_item(unsigned int slot) {
    if (slot >= _items.size()) {
        throw std::exception("[inventory::remove_item] reached end of vector");
    }
    delete _items[i];
    _items.erase(_items.begin() + slot);
    _items.shrink_to_fit();
}

void equip_weapon(weapon* weapon_) {
    _weapon = weapon_;
}

void equip_weapon(item* not_suitable) {
    return;
}

void equip_armor_head(armor* armor_) {
    if (armor_->get_armor_slot() != armor_slot::head)
        return;

    _armor.head = armor_;
}

void equip_armor_head(item* not_suitable) {
    return;
}

void equip_armor_body(armor* armor_) {
    if (armor_->get_armor_slot() != armor_slot::body)
        return;

    _armor.body = armor_;
}

void equip_armor_body(item* not_suitable) {
    return;
}

void equip_armor_legs(armor* armor_) {
    if (armor_->get_armor_slot() != armor_slot::legs)
        return;

    _armor.legs = armor_;
}

void equip_armor_legs(item* not_suitable) {
    return;
}

armor* get_armor_head() {
    return _armor.head;
}

armor* get_armor_body() {
    return _armor.body;
}

armor* get_armor_legs() {
    return _armor.legs;
}

short get_total_armor() {
    return _armor.head->get_armor_points() + _armor.body->get_armor_points() + _armor.legs->get_armor_points();
}
}
