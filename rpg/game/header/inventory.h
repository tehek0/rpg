#pragma once
#include "item.h"
#include <QObject>

/*

Схема именования полей классов:


Нижнее подчёркивание в начале названия приватного поля.

В методы, относящиеся к приватным полям, передаём те же названия, но без подчёркивания в начале
или схожие названия

EX:
char* _name;
void set_name(char* name);

Если поле имеет то же название, что и некий объект (пр: on_use _on_use;), то в методах передаём название с нижним подчёркиванием впереди

EX:

on_use _on_use;
void set_on_use(on_use on_use_);


Для публичных полей можно использовать произвольные названия

*/




struct full_armor {
    armor* head = nullptr;
    armor* body = nullptr;
    armor* legs = nullptr;
};

enum class inv_update_context {
    removed_item,
    added_item,
    refresh_stack
};

class inventory : public QObject {

    Q_OBJECT

signals:
    void trigger_update(unsigned int slot, inv_update_context context_);
protected:
    std::vector<item*> _items;
    full_armor _armor;
    weapon* _weapon = nullptr;
public:
    ~inventory();
    inventory() = default;
    std::vector<item*> get_items();
    void add_item(item* item_);
    item* get_item(unsigned int slot);
    size_t get_items_size();
    unsigned int get_slot(item* item_);
    void remove_item(unsigned int slot);
    void remove_item(unsigned int slot, unsigned int amount);
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
