#pragma once
#include "item.h"

struct full_armor {
    armor* head = nullptr;
    armor* body = nullptr;
    armor* legs = nullptr;
};

class inventory {
protected:
    std::vector<item*> _items;
    full_armor _armor;
    weapon* _weapon;
public:
    ~inventory();
    std::vector<item*> get_items();
    void add_item(item* item_);
    item* get_item(unsigned int slot);
    void remove_item(unsigned int slot);
    void equip_weapon(weapon* weapon_);
    void equip_weapon(item* not_suitable);
    void equip_armor_head(armor* armor_);
    void equip_armor_head(item* not_suitable);
    void equip_armor_body(armor* armor_);
    void equip_armor_body(item* not_suitable);
    void equip_armor_legs(armor* armor_);
    void equip_armor_legs(item* not_suitable);
    armor* get_armor_head();
    armor* get_armor_body();
    armor* get_armor_legs();
    short get_total_armor();
};
