
#include <QApplication>
#include "mainwindow.h"
//#include <cstdlib>
//#include <ctime>
//#include "game/header/global.h"
//#include <QPushButton>

int main(int argc, char *argv[])
{

    //std::srand(std::time(0));
    QApplication a(argc, argv);
    MainWindow w;
    // give_choice x(new dialogue_choice("Выбор 1", 1), new char_check_choice("Выбор с проверкой удачи", 0, 1, char_type::luck, 8), new dialogue_choice("Выбор 2", 2), new dialogue_choice("Выбор 3", 3));
    // entity_stats st;
    // st.luck = 7;
    // global::player_->set_entity_stats(st);
    // qInfo() << x.choices[1]->check();
    // interactable inter;
    // interaction_tree tree(new tell_line("Привет"), new tell_line("Мир"), new tell_line("Конец"));
    // inter.interaction_trees.push_back(tree);
    // qInfo() << inter.interaction_trees[0].interactions.size();
    // qInfo() << inter.interaction_trees[0].interactions[0]->run();
    // //w.showFullScreen();
    // inter.selected_interaction_tree = 0;
    // inter.execute();
    // inter.execute();
    // inter.execute();
    // inter.execute();


    w.show();
    w.on_menu_b_clicked();

    return a.exec();
}
