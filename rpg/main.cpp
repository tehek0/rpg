
#include <QApplication>
#include "mainwindow.h"
#include <cstdlib>
#include <ctime>
#include "game/header/character.h"
#include "game/header/global.h"

int main()
{
    global::player_ = new player;

    std::srand(std::time(0));

    //MainWindow w;
    QPoint p(960 - 150,540 - 150);
    QSize z(150,150);
    QString s = "shrimp";
    QString n = "test_entity";
    entity* entt = new entity(&global::w, p, z, s, n, anim_sequence(1, anim("test_anim", 3, 6), anim("test_anim2", 10, 6)));
    global::w.on_map.emplace_back(entt->_disp);
    entt->_disp->hide();
    global::w.connect(entt->_disp, &QPushButton::clicked, &global::w, [=]() {(&global::w)->OnEntClicked(entt);});
    give_choice x(new dialogue_choice("Выбор 1", 1), new char_check_choice("Выбор с проверкой удачи", 0, 1, char_type::luck, 8), new dialogue_choice("Выбор 2", 2), new dialogue_choice("Выбор 3", 3));
    entity_stats st;
    st.luck = 7;
    global::player_->set_entity_stats(st);
    qInfo() << x.choices[1]->check();
    interactable inter;
    inter.interaction_trees.emplace_back(new interaction_tree(new tell_line("Привет"), new tell_line("Мир"), new give_choice(new dialogue_choice("Выбор 1", 1), new char_check_choice("Выбор с проверкой удачи", 0, 1, char_type::luck, 8), new dialogue_choice("Выбор 2", 2), new dialogue_choice("Выбор 3", 3)), new tell_line("Конец")));
    qInfo() << inter.interaction_trees[0]->interactions.size();
    qInfo() << inter.interaction_trees[0]->interactions[0]->run();
    //w.showFullScreen();
    inter.selected_interaction_tree = 0;
    inter.execute();
    inter.execute();
    inter.execute();
    inter.execute();


    global::w.show();
    global::w.on_menu_b_clicked();

    return global::a.exec();
}
