#pragma once
#include "inventory.h"
#include "data/interaction_return_value_types.h"
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
virtual interaction_return_value_type expected_return_value;
virtual int run();
};

// Персонаж говорит в диалоговом окне
struct tell_line: public interaction_type {
QString line;
interaction_return_value_type expected_return_value = interaction_return_value_type::tell_line;
int run();
};

// Базовый выбор без проверок. Виртуальная функция всегда возвращает true
struct dialogue_choice {
QString choice;
int jump_to_tree;
virtual bool check();
};

// Каким-то образом надо придумать, как запихнуть в эти проверки статы игрока. Возможно, сработает глобальный неймспейс
struct char_check_choice : public dialogue_choice {
char_type type;
int required;
bool check();
};

struct skill_check_choice : public dialogue_choice {
skill_type type;
int required;
bool check();
};

// Возвращаемое значение здесь - переход на другую ветвь дерева в классе interactable
struct give_choice: public interaction_type {
std::vector<dialogue_choice*> choices;
interaction_return_value_type expected_return_value = interaction_return_value_type::give_choice;
int run();
~give_choice();
};

// Обманываем компилятор
class enemy;

// Здесь о возврашемом значении можно не париться
struct start_battle: public interaction_type {
std::vector<enemy*> enemies;
interaction_return_value_type expected_return_value = interaction_return_value_type::start_battle;
int run();
};

// Обманываем не только компилятор, но и себя
class trader;

// Используем указатель на торговца, чтобы в функции run вызыать у него что-то
struct start_trade: public interaction_type {
trader* trade_with;
interaction_return_value_type expected_return_value = interaction_return_value_type::start_trade;
int run();
};

// Несмотря на название, это не дерево, а веточка
struct interaction_tree {
std::vector<interaction_type*> interactions;
int returned_value;
unsigned int progress = 0;
int run_tree();
// В реализации run_tree() что-то типа 
// returned_value = this->interactions[progress]->run();
// ++progress;
~interaction_tree();
};

// Собственно класс, который будут наследовать сущности
class interactable {
public:
std::vector<interaction_tree> interaction_trees;
unsigned int selected_interaction_tree = 0;
void execute();
// В реализации execute():
// interaction_trees[selected_interaction_tree ].run_tree()
// *обработка в зависимости от зачений в структурах*
virtual ~interactable() = default;
}



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



class entity: public interactable {
protected:
    inventory _inventory;
    QString _name;
    QString _sprite_family;
    entity_stats _entity_stats;
    entity_level _entity_level;
    int _health;
    int _max_health;
    int _money;
public:
    virtual ~entity() = default;
    inventory get_inventory();
    QString get_name();
    QString get_sprite_family();
    entity_stats get_entity_stats();
    entity_level get_entity_level();
    int get_heatlh();
    int get_max_health();
    int get_money();
    void set_inventory(inventory& inventory_);
    void set_name(QString& name);
    void set_sprite_family(QString& sprite_family);
    void set_entity_stats(entity_stats& entity_stats_);
    void set_entity_level(entity_level& entity_level_);
    void set_health(int health);
    void set_max_health(int max_health);
    void set_money(int money);

    void add_experience(unsigned int amount);

};
