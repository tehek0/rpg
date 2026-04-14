#include "../header/character.h"
#include "../header/visuals.h"
#include "../header/global.h"

/*
entity::entity(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name){ //через делегирующий конструктор 2 он запускать не может, поэтому так

    //создание технического объекта
    entity();
    // создание визуальной компоненты
    clickable(w, coord_and_size, sprite_family, name);

    qInfo() << "[entity] constr";
}
*/
short living_entity::get_entity_stat(char_type type) {
    switch(type) {
    case char_type::agility: {
        return _entity_stats.agility;
    }
    case char_type::endurance: {
        return _entity_stats.endurance;
    }
    case char_type::intelligence: {
        return _entity_stats.intelligence;
    }
    case char_type::luck: {
        return _entity_stats.luck;
    }
    case char_type::strength: {
        return _entity_stats.strength;
    }
    default: {
        qInfo() << "[get_entity_stat] необрабатываемая характеристика";
        exit(-1);
    }
    }
}

short living_entity::get_entity_stat(skill_type type) {
    switch(type) {
    case skill_type::barter: {
        return _entity_stats.barter;
    }
    case skill_type::big_guns: {
        return _entity_stats.big_guns;
    }
    case skill_type::guns: {
        return _entity_stats.intelligence;
    }
    case skill_type::science: {
        return _entity_stats.science;
    }
    case skill_type::spech: {
        return _entity_stats.speech;
    }
    case skill_type::survival: {
        return _entity_stats.survival;
    }
    case skill_type::unarmed: {
        return _entity_stats.unarmed;
    }
    default: {
        qInfo() << "[get_entity_stat] необрабатываемый скилл";
        exit(-1);
    }
    }
}

void living_entity::set_entity_stats(entity_stats &entity_stats_) {
    _entity_stats = entity_stats_;
}

// Эта функция возвращает успех выбора данного варианта диалога. Для скилл чеков в этой функции будут проверяться характеристики игрока.
bool dialogue_choice::check() {
    return true;
}

bool char_check_choice::check() {
    if (global::player_->get_entity_stat(type) >= required)
        return true;

    return false;
}

bool skill_check_choice::check() {
    if (global::player_->get_entity_stat(type) >= required)
        return true;

    return false;
}


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


// Почему-то деструктор вызывается два раза. Без понятия, почему это может происходить
interaction_tree::~interaction_tree() {
    for (size_t i = 0; i < interactions.size(); ++i) {
        if (interactions[i] != nullptr) {
            delete interactions[i];
        }
    }
}

QString get_line_from_tell_line(tell_line* l) {
    return l->line;
}
QString get_line_from_tell_line(interaction_type* unstbl) {
    return "";
}

void interactable::execute() {
    size_t int_trees_size = interaction_trees.size();
    if (int_trees_size == 0)
        return;
    if (int_trees_size <= selected_interaction_tree)
        return;
    int catcher = interaction_trees[selected_interaction_tree].run_tree();
    if (catcher == -1)
        return;
    qInfo() << get_line_from_tell_line(interaction_trees[selected_interaction_tree].interactions[interaction_trees[selected_interaction_tree].progress]);
    ++interaction_trees[selected_interaction_tree].progress;
}

