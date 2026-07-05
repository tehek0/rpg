#include "../header/character.h"
#include "../header/global.h"
#include "../header/message.h"
inventory* entity::get_inventory() {
    return _inventory;
}
QString entity::get_name() {
    return _name;
}
QString entity::get_asset() {
    return _asset;
}

void entity::set_inventory(inventory* inventory_) {
    delete _inventory;
    _inventory = inventory_;
}
void entity::set_name(const QString& name) {
    _name = name;
}
void entity::set_asset(const QString& asset) {
    _asset = asset;
}

short entity_stats::get_stat(char_type type) {
    switch(type) {
    case char_type::agility: {
        return agility;
    }
    case char_type::endurance: {
        return endurance;
    }
    case char_type::intelligence: {
        return intelligence;
    }
    case char_type::luck: {
        return luck;
    }
    case char_type::strength: {
        return strength;
    }
    default: {
        qInfo() << "[get_stat] необрабатываемая характеристика";
        exit(-1);
    }
    }
}

short entity_stats::get_stat(skill_type type) {
    switch(type) {
    case skill_type::barter: {
        return barter;
    }
    case skill_type::big_guns: {
        return big_guns;
    }
    case skill_type::guns: {
        return guns;
    }
    case skill_type::science: {
        return science;
    }
    case skill_type::speech: {
        return speech;
    }
    case skill_type::survival: {
        return survival;
    }
    case skill_type::unarmed: {
        return unarmed;
    }
    default: {
        qInfo() << "[get_stat] необрабатываемый скилл";
        exit(-1);
    }
    }
}

void entity_stats::add_stat(skill_type type, short value) {
    switch(type) {
    case skill_type::barter: {
        barter += value;
        break;
    }
    case skill_type::big_guns: {
        big_guns += value;
        break;
    }
    case skill_type::guns: {
        guns += value;
        break;
    }
    case skill_type::science: {
        science += value;
        break;
    }
    case skill_type::speech: {
        speech += value;
        break;
    }
    case skill_type::survival: {
        survival += value;
        break;
    }
    case skill_type::unarmed: {
        unarmed += value;
        break;
    }
    default: {
        qInfo() << "[get_stat] необрабатываемый скилл";
        exit(-1);
    }
    }
}

entity_stats entity_stats::operator+(entity_stats other) {
    entity_stats total;
    total.strength = this->strength + other.strength;
    total.agility = this->agility + other.agility;
    total.endurance = this->endurance + other.endurance;
    total.intelligence = this->intelligence + other.intelligence;
    total.luck = this->luck + other.luck;

    total.guns = this->guns + other.guns;
    total.big_guns = this->big_guns + other.big_guns;
    total.unarmed = this->unarmed + other.unarmed;
    total.science = this->science + other.science;
    total.speech = this->speech + other.speech;
    total.barter = this->barter + other.barter;
    total.survival = this->survival + other.survival;

    return total;
}

entity_stats living_entity::get_entity_stats() {
    return this->_entity_stats;
}

void living_entity::set_entity_stats(entity_stats &entity_stats_) {
    _entity_stats = entity_stats_;
}

entity_level living_entity::get_entity_level() {
    return _entity_level;
}

int living_entity::get_max_health() {
    return _max_health;
}


int living_entity::get_health() {
    return _health;
}

int living_entity::get_base_armor() {
    return _base_armor;
}

int player::get_bonus_armor() {
    return _bonus_armor;
}

int player::get_bonus_health() {
    return _bonus_health;
}

int player::get_bonus_energy() {
    return _bonus_energy;
}

int living_entity::get_money() {
    return _money;
}
void living_entity::add_money(int amount) {
    _money += amount;
}

void living_entity::set_entity_level(entity_level &entity_level_) {
    _entity_level = entity_level_;
}

void living_entity::set_max_health(int max_health) {
    _max_health = max_health;
}
void living_entity::set_health(int health) {
    if (health > _max_health) {
        _health = _max_health;
    }
    else if (health < 0 && abs(health) >_health) {
        _health = 0;
    }
    else {
        _health = health;
    }

}

void living_entity::set_base_armor(int base_armor) {
    _base_armor = base_armor;
}

void living_entity::set_money(int money) {
    _money = money;
}


entity_stats player::get_bonus_stats() {
    return this->_bonus_stats;
}

entity_stats player::get_total_stats() {
    return this->_entity_stats + this->_bonus_stats;
}

float player::get_max_weight() {
    return _max_weight;
}

int player::get_max_energy() {
    return _max_energy;
}

void player::set_max_weight(float max_weight) {
    _max_weight = max_weight;
}

void player::set_max_energy(int max_energy) {
    _max_energy = max_energy;
}

void player::set_bonus_armor(int value) {
    _bonus_armor = value;
}

void player::set_bonus_health(int value) {
    _bonus_health = value;
}

void player::set_bonus_energy(int value) {
    _bonus_energy = value;
}

void player::set_level_up_points(int value) {
    _level_up_points = value;
}

unsigned int player::get_level_up_points() {
    return _level_up_points;
}

void player::set_inventory(inventory* inventory_) {
    delete _inventory;
    _inventory = inventory_;
    connect(_inventory, &inventory::change_weight, this, &player::change_weight);
}

void player::set_base_stats() {
    this->_max_health = 45 + (5 * this->_entity_stats.endurance) + (this->_entity_stats.endurance >= 10 ? 15 : 0);
    this->_health = _max_health;
    this->_max_weight = 45.0 + (5.0 * this->_entity_stats.strength) + (this->_entity_stats.strength >= 10 ? 15 : 0);
    this->_max_energy = 2 + (this->_entity_stats.agility / 3) + (this->_entity_stats.agility >= 10 ? 1 : 0);
}

float player::get_weight() {
    return _weight;
}

void player::apply_equipment_bonuses() {

}

void player::change_weight(float weight) {
    _weight += weight;
}

bool player::able_to_level_up() {
    if (_entity_level.experiecne >= _entity_level.current_needed)
        return true;

    return false;
}

void player::level_up() {
    ++_entity_level.level;
    new screen_message(QString("> Новый уровень: %1\nДобавлены очки навыков").arg(_entity_level.level), 170, 300, 24, 40, 255, 40);
    _entity_level.current_needed = _entity_level.current_needed + (_entity_level.scaling * _entity_level.level);
    _max_weight += 2;
    _max_health += 5;
    _health += 5;
    _level_up_points += 5;
    if (able_to_level_up())
        level_up();
}

void player::set_entity_level(entity_level& entity_level_) {
    _entity_level = entity_level_;
    if (able_to_level_up())
        level_up();
}

void player::add_exp(int amount) {
    _entity_level.experiecne += amount;
    new screen_message(QString("+%1 опыта").arg(amount), 70, 10, 20, 255, 255, 255);
    if (_entity_level.experiecne < 0)
        _entity_level.experiecne = 0;
    if (able_to_level_up()) {
        level_up();
    }
}

// Эта функция возвращает успех выбора данного варианта диалога. Для скилл чеков в этой функции будут проверяться характеристики игрока.
bool dialogue_choice::check() {
    return true;
}

bool char_check_choice::check() {
    if (global::player_->get_total_stats().get_stat(type) >= required)
        return true;

    return false;
}

bool skill_check_choice::check() {
    if (global::player_->get_total_stats().get_stat(type) >= required)
        return true;

    return false;
}

void living_entity::attacked() {
    //Сложную логику рассчёта урона сюда
    int damage = 10;
    this->set_health(_health - damage);
};
//TODO: реализация ранов

// Здесь будем пихать в диалоговое окно фразу и может быть спрайт
int tell_line::run() {
    return 1;
}
// В этой функции будем передавать в диалоговое окно варианты ответа, пока пустуем
int give_choice::run() {
    return 1;
}
// Чистим чистим чистим
give_choice::~give_choice() {
    for (size_t i = 0; i < choices.size(); ++i) {
        delete choices[i];
    }
}
// Тут запускаем битву
int start_battle::run() {
    return 1;
}
// Тут запускаем торговлю
int start_trade::run() {
    return 1;
}

int interaction_tree::run_tree() {
    if (interactions.size() <= progress) {
        returned_value = -1;
        return -1;
    }
    returned_value = interactions[progress]->run();
    return 0;
}


interaction_tree::~interaction_tree() {
    for (size_t i = 0; i < interactions.size(); ++i) {
            delete interactions[i];
    }
}

interactable::~interactable() {
    for (size_t i = 0; i < interaction_trees.size(); ++i) {
            delete interaction_trees[i];
    }
}


void interactable::execute() {
    size_t int_trees_size = interaction_trees.size();
    if (int_trees_size == 0)
        return;
    if (int_trees_size <= selected_interaction_tree)
        return;
    int catcher = interaction_trees[selected_interaction_tree]->run_tree();
    if (catcher == -1)
        return;
    ++interaction_trees[selected_interaction_tree]->progress;
}

