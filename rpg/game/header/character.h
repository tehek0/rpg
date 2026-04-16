#pragma once
#include "inventory.h"
#include "data/interaction_return_value_types.h"
#include "data/trait_types.h"
#include "data/enemy_trait_types.h"
#include <QObject>
#include "visuals.h"
#include "quest.h"
#include "location.h"
#include <mainwindow.h>

// TODO: общий класс сущности, наследники - игрок (но можно и не наследовать на самом деле), нпс (с наследниками торговец и враг)


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




// Базовый наследуемый класс для запихивания в вектор
struct interaction_type {
    virtual ~interaction_type() = default;
    interaction_return_value_type expected_return_value;
    virtual int run() = 0;
};

// Персонаж говорит в диалоговом окне
struct tell_line: public interaction_type {
    tell_line(const QString& line_): line(line_)
    {}
    QString line;
    interaction_return_value_type expected_return_value = interaction_return_value_type::tell_line;
    int run();
};

// Базовый выбор без проверок. Виртуальная функция всегда возвращает true
struct dialogue_choice {
    dialogue_choice(const QString& choice_, int jump_to_tree_): choice(choice_), jump_to_tree(jump_to_tree_)
    {}
    dialogue_choice(const QString& choice_, int jump_to_tree_, int jump_to_if_fail_): choice(choice_), jump_to_tree(jump_to_tree_), jump_to_if_fail(jump_to_if_fail_)
    {}
    virtual ~dialogue_choice() = default;
    QString choice;
    int jump_to_tree;
    // Это поле не используется здесь, но используется в наследниках
    int jump_to_if_fail = 0;
    virtual bool check();
};

// Каким-то образом надо придумать, как запихнуть в эти проверки статы игрока. Возможно, сработает глобальный неймспейс
struct char_check_choice : public dialogue_choice {
    char_check_choice(const QString& choice_, int jump_to_tree_, int jump_to_if_fail_, char_type type_, int required_): dialogue_choice(choice_, jump_to_tree_, jump_to_if_fail_), type(type_), required(required_)
    {}
    char_type type;
    int required;
    bool check();
};

struct skill_check_choice : public dialogue_choice {
    skill_check_choice(const QString& choice_, int jump_to_tree_, int jump_to_if_fail_, skill_type type_, int required_): dialogue_choice(choice_, jump_to_tree_, jump_to_if_fail_), type(type_), required(required_)
    {}
    skill_type type;
    int required;
    bool check();
};

// Возвращаемое значение здесь - переход на другую ветвь дерева в классе interactable

struct give_choice: public interaction_type {
    std::vector<dialogue_choice*> choices;
    interaction_return_value_type expected_return_value = interaction_return_value_type::give_choice;
    int run();
    template<typename... Args>
    give_choice(Args&&... args) {
        static_assert((std::is_constructible_v<dialogue_choice*, Args&&> && ...));
        (choices.push_back(std::forward<Args>(args)), ...);
    }
    ~give_choice();
};

// Обманываем компилятор
class enemy;

// Здесь о возврашемом значении можно не париться
struct start_battle: public interaction_type {
    std::vector<enemy*> enemies;
    interaction_return_value_type expected_return_value = interaction_return_value_type::start_battle;
    int run();
    start_battle(enemy* single_enemy) {
        enemies.reserve(1);
        enemies.emplace_back(single_enemy);
    }
    start_battle(enemy* first_enemy, enemy* second_enemy) {
        enemies.reserve(2);
        enemies.emplace_back(first_enemy);
        enemies.emplace_back(second_enemy);
    }
    start_battle(enemy* first_enemy, enemy* second_enemy, enemy* third_enemy) {
        enemies.reserve(3);
        enemies.emplace_back(first_enemy);
        enemies.emplace_back(second_enemy);
        enemies.emplace_back(third_enemy);
    }
};

// Обманываем не только компилятор, но и себя
class trader;

// Используем указатель на торговца, чтобы в функции run вызыать у него что-то
struct start_trade: public interaction_type {
    trader* trade_with;
    interaction_return_value_type expected_return_value = interaction_return_value_type::start_trade;
    int run();
    start_trade(trader* trader_): trade_with(trader_)
    {}
};

// Несмотря на название, это не дерево, а веточка
struct interaction_tree {
    std::vector<interaction_type*> interactions;
    int returned_value;
    unsigned int progress = 0;
    int run_tree();
    template<typename... Args>
    interaction_tree(Args&&... args) {
        static_assert((std::is_constructible_v<interaction_type*, Args&&> && ...));
        (interactions.emplace_back(std::forward<Args>(args)), ...);
    }
    ~interaction_tree();
};

// Собственно класс, который будут наследовать сущности. Этот класс в свою очередь наследует QObject, может быть пригодится для сигналов, если нет - уберём
class interactable {
public:
    interactable() = default;
    std::vector<interaction_tree*> interaction_trees;
    unsigned int selected_interaction_tree = 0;
    virtual ~interactable();
    void execute();
    // В реализации execute():
    // interaction_trees[selected_interaction_tree ].run_tree()
    // *обработка в зависимости от зачений в структурах*

};



struct entity_stats {

    short strength;
    short agility;
    short endurance;
    short intelligence;
    short luck;

    short guns;
    short big_guns;
    short unarmed;
    short science;
    short speech;
    short barter;
    short survival;

};



struct entity_level {

    unsigned int level;
    unsigned int experiecne;
    unsigned int scaling;

};

class entity: public interactable, public displayable {
protected:
    inventory _inventory;
    // name и sprite family поля придут с displayable
public:
    entity() = default;
    entity(MainWindow* w, QPoint& coord, QSize& size, QString& sprite_family, QString& name)
        : displayable(w,true,coord,size,sprite_family,name) {
    };
    virtual ~entity() = default;

    inventory get_inventory();
    QString get_name();
    QString get_sprite_family();
    void set_inventory(inventory& inventory_);
    void set_name(QString& name);
    void set_sprite_family(QString& sprite_family);
};

class living_entity: public entity {
protected:
    entity_stats _entity_stats;
    entity_level _entity_level;
    int _max_health;
    int _health;
    int _base_armor;
    int _total_armor;
    int _money;
public:
    virtual ~living_entity() = default;
    entity_stats get_entity_stats();
    short get_entity_stat(char_type type_);
    short get_entity_stat(skill_type type_);
    entity_level get_entity_level();
    int get_max_health();
    int get_health();
    int get_base_armor();
    int get_total_armor();
    int get_money();
    void set_entity_stats(entity_stats& entity_stats_);
    void set_entity_level(entity_level& entity_level_);
    void set_max_health(int max_health);
    void set_health(int health);
    void set_base_armor(int base_armor);
    void set_total_armor(int total_armor);
    void regen_health(int amount);
    void damage_health(int amount);
    void set_money(int money);
    void add_money(int amount);
    bool transaction(living_entity* other, int amount);
};


class player: public living_entity {
protected:
    float _max_weight;
    float _weight;
    int _max_energy;
    trait _trait;
public:
    std::vector<quest> quests;
    location* current_location;
    float get_max_weight();
    float get_weight();
    int get_max_energy();
    trait get_trait();
    void set_max_weight(float max_weight);
    void set_weight(float weight);
    void set_max_energy(int max_energy);
    void set_trait(trait trait_);
    void apply_equipment_bonuses();
    bool add_item(item* item_);
};

struct enemy_traits {
    enemy_aggression aggression;
    enemy_battle_style battle_style;
    enemy_family family;
    enemy_social social;
};

class enemy: public living_entity {
protected:
    enemy_traits _enemy_traits;
    int _base_dmg;
    bool _delete_after_battle = true;
public:
    enemy_traits get_enemy_traits();
    int get_base_dmg();
    bool get_delete_after_battle();
    void set_enemy_traits(enemy_traits enemy_traits_);
    void set_base_dmg(int base_dmg);
    void set_delete_after_battle(bool delete_after_battle);
};

struct offer {
    item* item_;
    int base_price;
    int stock;
    ~offer();
};

class trader: public living_entity {
protected:
    std::vector<offer> _offers;
    float _price_coefficient;
public:
    std::vector<offer> get_offers();
    float get_price_coefficient();
    bool buy(unsigned int id, unsigned int amount);
    bool sell(item* item_);
    bool sell(item* item_, unsigned int amount);
};
