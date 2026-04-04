#pragma once
#include <QString>
#include <vector>

#include "data/damage_types.h"
#include "data/ammo_types.h"
#include "data/armor_slots.h"
#include "data/equipment_bonuses.h"
#include "data/use_effects.h"

#include "item_requirements.h"




class item {
protected:
    QString _name;
    QString _desc;
    QString _asset;
    unsigned int _stack;
    unsigned int _max_stack_size;
    unsigned int _base_weight;
    unsigned int _base_cost;
    bool _sellable;
public:
    item() = default;
    item(const QString& name,
         const QString& desc,
         const QString& asset,
         unsigned int stack,
         unsigned int max_stack_size,
         unsigned int base_weight,
         unsigned int base_cost,
         bool sellable):
        _name(name), _desc(desc), _asset(asset),
        _stack(stack), _max_stack_size(max_stack_size),
        _base_weight(base_weight), _base_cost(base_cost),
        _sellable(sellable)
    {}
    item(item* copy):
        _name(copy->_name), _desc(copy->_desc), _asset(copy->_asset),
        _stack(copy->_stack), _max_stack_size(copy->_max_stack_size),
        _base_weight(copy->_base_weight), _base_cost(copy->_base_cost),
        _sellable(copy->_sellable)
    {}
    virtual ~item() = default;

    bool can_add(item& other);
    bool can_add(item& other, unsigned int count);
    void add();
    void add(unsigned int count);
    bool has_amount(unsigned int count);
    void remove();
    void remove(unsigned int count);
    QString get_name();
    QString get_desc();
    QString get_asset();
    unsigned int get_stack();
    unsigned int get_max_stack_size();
    unsigned int get_base_weight();
    unsigned int get_base_weight(unsigned int x);
    unsigned int get_total_weight();
    unsigned int get_base_cost();
    bool get_sellable();
    void set_name(const QString& name);
    void set_desc(const QString& desc);
    void set_asset(const QString& asset);
    void set_stack(unsigned int stack);
    void set_max_stack_size(unsigned int max_stack_size);
    void set_base_weight(unsigned int base_weight);
    void set_base_cost(unsigned int base_cost);
    void set_sellable(bool sellable);
};




class item_with_requirements : public item {
protected:
    item_requirements _requirements;
public:
    item_with_requirements() = default;
    item_with_requirements(const QString& name,
                           const QString& desc,
                           const QString& asset,
                           unsigned int stack,
                           unsigned int max_stack_size,
                           unsigned int base_weight,
                           unsigned int base_cost,
                           bool sellable,
                           item_requirements& requirements):
        item(name, desc, asset,
             stack, max_stack_size,
             base_weight, base_cost,
             sellable),
        _requirements(requirements)
    {}
    item_with_requirements(item_with_requirements* copy): item(copy),
        _requirements(copy->_requirements)
    {}
    virtual ~item_with_requirements() = default;
    item_requirements get_item_requirements();
    void set_item_requirements(item_requirements& requirements);
};





class weapon : public item_with_requirements {
protected:
    int _base_dmg;
    damage_type _damage_type;
    ammo_type _ammo_type;
    short _energy_cost;
public:
    weapon() = default;
    weapon(const QString& name,
           const QString& desc,
           const QString& asset,
           unsigned int stack,
           unsigned int max_stack_size,
           unsigned int base_weight,
           unsigned int base_cost,
           bool sellable,
           item_requirements& requirements,
           int base_dmg,
           damage_type damage_type_,
           ammo_type ammo_type_,
           short energy_cost):
        item_with_requirements(name, desc, asset,
                               stack, max_stack_size,
                               base_weight, base_cost,
                               sellable, requirements),
        _base_dmg(base_dmg),
        _damage_type(damage_type_),
        _ammo_type(ammo_type_),
        _energy_cost(energy_cost)
    {}
    weapon(weapon* copy): item_with_requirements(copy),
        _base_dmg(copy->_base_dmg),
        _damage_type(copy->_damage_type),
        _ammo_type(copy->_ammo_type),
        _energy_cost(copy->_energy_cost)
    {}
    int get_base_dmg();
    damage_type get_damage_type();
    ammo_type get_ammo_type();
    short get_energy_cost();
    void set_base_dmg(int base_dmg);
    void set_damage_type(damage_type damage_type_);
    void set_ammo_type(ammo_type ammo_type_);
    void set_energy_cost(short energy_cost);
};


class ammo : public item {
protected:
    int _base_dmg;
    ammo_type _ammo_type;
public:
    ammo() = default;
    ammo(const QString& name,
           const QString& desc,
           const QString& asset,
           unsigned int stack,
           unsigned int max_stack_size,
           unsigned int base_weight,
           unsigned int base_cost,
           bool sellable,
           int base_dmg,
           ammo_type ammo_type_):
        item(name, desc, asset,
             stack, max_stack_size,
             base_weight, base_cost,
             sellable),
        _base_dmg(base_dmg),
        _ammo_type(ammo_type_)
    {}
    ammo(ammo* copy): item(copy),
        _base_dmg(copy->_base_dmg),
        _ammo_type(copy->_ammo_type)
    {}
    int get_base_dmg();
    ammo_type get_ammo_type();
    void set_base_dmg(int base_dmg);
    void set_ammo_type(ammo_type ammo_type_);
};




struct armor_bonus {
    equipment_bonus bonus;
    int value;
};

class armor : public item_with_requirements {
protected:
    armor_slot _armor_slot;
    short _armor_points;
    armor_bonus _armor_bonus;
public:
    armor() = default;
    armor(const QString& name,
          const QString& desc,
          const QString& asset,
          unsigned int stack,
          unsigned int max_stack_size,
          unsigned int base_weight,
          unsigned int base_cost,
          bool sellable,
          item_requirements& requirements,
          armor_slot armor_slot_,
          short armor_points,
          armor_bonus armor_bonus_):
        item_with_requirements(name, desc, asset,
                               stack, max_stack_size,
                               base_weight, base_cost,
                               sellable, requirements),
        _armor_slot(armor_slot_),
        _armor_points(armor_points),
        _armor_bonus(armor_bonus_)
    {}
    armor(armor* copy): item_with_requirements(copy),
        _armor_slot(copy->_armor_slot),
        _armor_points(copy->_armor_points),
        _armor_bonus(copy->_armor_bonus)
    {}
    armor_slot get_armor_slot();
    short get_armor_points();
    armor_bonus get_armor_bonus();
    void set_armor_slot(armor_slot armor_slot_);
    void set_armor_points(short armor_points);
    void set_armor_bonus(armor_bonus armor_bonus_);
};






struct on_use {
    use_effect effect;
    int value;
};

class consumable : public item {
protected:
    on_use _on_use;
    unsigned short _uses_left;
    unsigned short _use_energy_cost;
public:
    consumable() = default;
    consumable(const QString& name,
               const QString& desc,
               const QString& asset,
               unsigned int stack,
               unsigned int max_stack_size,
               unsigned int base_weight,
               unsigned int base_cost,
               bool sellable,
               on_use on_use_,
               unsigned short uses_left,
               unsigned short use_energy_cost):
        item(name, desc, asset,
            stack, max_stack_size,
            base_weight, base_cost,
            sellable),
        _on_use(on_use_),
        _uses_left(uses_left),
        _use_energy_cost(use_energy_cost)
    {}
    consumable(consumable* copy): item(copy),
        _on_use(copy->_on_use),
        _uses_left(copy->_uses_left),
        _use_energy_cost(copy->_use_energy_cost)
    {}
    on_use get_on_use();
    unsigned short get_uses_left();
    unsigned short get_use_energy_cost();
    void set_on_use(on_use on_use_);
    void set_uses_left(unsigned short uses_left);
    void set_use_energy_cost(unsigned short use_energy_cost);
};
