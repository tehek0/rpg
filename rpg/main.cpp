
#include <QApplication>
#include "mainwindow.h"
#include <cstdlib>
#include <ctime>
#include "game/header/visuals.h"

#include <QGridLayout>
#include <QSizePolicy>
#include <QFile>
int main()
{
    global::player_ = new player;

    std::srand(std::time(0));

    //MainWindow w;
    // QPoint p(960 - 150,540 - 150);
    // QSize z(150,150);
    // QString s = "shrimp";
    // QString n = "<img src=\":/pictures/ui_armor_icon.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\"> Броня: 5";
    // entity_object* entt = new entity_object(new entity(n, s), anim_sequence(1, anim("test_anim", 3, 6), anim("test_anim2", 10, 6)), p, z);
    // global::w.on_map.emplace_back(entt->_disp);
    // entt->_disp->hide();
    // global::w.connect(entt->_disp, &QPushButton::clicked, &global::w, [=]() {(&global::w)->OnEntClicked(entt);});
    // give_choice x(new dialogue_choice("Выбор 1", 1), new char_check_choice("Выбор с проверкой удачи", 0, 1, char_type::luck, 8), new dialogue_choice("Выбор 2", 2), new dialogue_choice("Выбор 3", 3));
    // entity_stats st;
    // st.luck = 7;
    // global::player_->set_entity_stats(st);
    // qInfo() << x.choices[1]->check();
    // interactable inter;
    // inter.interaction_trees.emplace_back(new interaction_tree(new tell_line("Привет"), new tell_line("Мир"), new give_choice(new dialogue_choice("Выбор 1", 1), new char_check_choice("Выбор с проверкой удачи", 0, 1, char_type::luck, 8), new dialogue_choice("Выбор 2", 2), new dialogue_choice("Выбор 3", 3)), new tell_line("Конец")));
    // qInfo() << inter.interaction_trees[0]->interactions.size();
    // qInfo() << inter.interaction_trees[0]->interactions[0]->run();
    global::w.showFullScreen();
    // inter.selected_interaction_tree = 0;
    // inter.execute();
    // inter.execute();
    // inter.execute();
    // inter.execute();

    // item_object* item_obj = new item_object(new item(QString("Тапки"),QString("Это тапки."), QString("icon_inv_armor_sandals"), 3, 56, 0.0f, 10, true), QPoint(100, 100));
    // qInfo() << item_obj->stack_label->text();
    // item_obj->_disp->hide();
    // QWidget* lyt_w = new QWidget(&global::w);

    // QGridLayout* lyt = new QGridLayout(lyt_w);
    // int rows = 4;
    // int columns = 4;
    // lyt_w->setGeometry(QRect(QPoint(1200,200),QSize(columns*z.width(), rows*z.height())));
    // for (int x = 0; x < rows; ++x) {
    //     for (int y = 0; y < columns; ++y) {
    //         QString namee = QString("Креветка на клетке %1, %2").arg(x).arg(y);
    //         entity_object* enttt = new entity_object(new entity(namee, s), anim_sequence(1, anim("test_anim", 3, 6), anim("test_anim2", 10, 6)));
    //         global::w.connect(enttt->_disp, &QPushButton::clicked, &global::w, [=]() {(&global::w)->OnEntClicked(enttt);});
    //         enttt->_disp->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
    //         lyt->addWidget(enttt->_disp, x, y);
    //     }
    // }

    // // global::w.on_map.emplace_back(lyt_w);
    // //delete lyt;
    // lyt_w->deleteLater();
    // inventory* inv = new inventory();
    // int generate_items = 70;
    // for (int i = 0; i < generate_items; ++i) {
    //     inv->add_item(new item(QString("Предмет %1").arg(i), QString("Тест инвентаря"), QString("icon_inv_combat_knife"), 2, 3, 1.0f, 100, true));
    // }
    // inv->add_item(item_obj->linked_item);
    // item_requirements* x_r = new item_requirements();
    // x_r->item_requirements_ptrs.emplace_back(new skill_requirement(40, skill_type::guns));
    // x_r->min_level = 5;
    // item_requirements* y_r = new item_requirements();
    // y_r->item_requirements_ptrs.emplace_back(new skill_requirement(5, skill_type::speech));
    // y_r->item_requirements_ptrs.emplace_back(new char_requirement(3, char_type::endurance));
    // armor_bonus bonus;
    // bonus.bonus = equipment_bonus::change_char_intelligence;
    // bonus.value = -2;
    // inv->add_item(new weapon("Пистолет массового поражения", "В комментариях не нуждается", "icon_inv_weapon_revolver", 1, 1, 3.5f, 199, true, x_r, 50, damage_type::bullet, ammo_type::pistol, 1));
    // inv->add_item(new armor("Броня бомжа", "Носят бомжи. Воняет.", "icon_inv_armor_scrapJacket", 1, 1, 7.5f, 30, true, y_r, armor_slot::body, 3, bonus));
    // on_use z_u;
    // z_u.effect = use_effect::change_health;
    // z_u.value = 15;
    // inv->add_item(new consumable("Бинт", "Лечит", "icon_inv_consumable_bleedBandage", 3, 5, 0.1, 30, true, z_u, 1, 1));
    // delete item_obj;
    // QPushButton* btttn = new QPushButton(&global::w);
    // btttn->setGeometry(QRect(QPoint(0,0),QSize(200,200)));
    // btttn->show();
    // QLabel* lbl = new QLabel(btttn);
    // lbl->setGeometry(btttn->geometry());
    // lbl->setText("Кол-во");
    // lbl->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    // delete btttn;
    // inventory_object* inv_o = new inventory_object(inv, inventory_context::container_self, 3, 3, 100, QPoint(250, 250));
    // qInfo() << inv_o->layout->columnCount() << inv_o->layout->rowCount();
    // global::w.on_inventory.emplace_back(inv_o->base);
    // text_holder* hold = new text_holder("black_text_bg");
    // text_object* t_o = new text_object("Это, может быть, лучший шрифт", "typing", false, 10);
    // t_o->setMargin(15);
    // t_o->setFont(QFont("MS Gothic", 15));
    // t_o->setStyleSheet("color: rgb(255,255,255);");
    // hold->set_text_object(t_o);
    // t_o->show();
    // hold->show();
    // for (item_object* elem : inv_o->item_objects) {
    //     qInfo() << elem->linked_item->get_name();
    // }
    // QSoundEffect* ef = new QSoundEffect(&global::w);
    // ef->setSource(QUrl(QString("qrc:/sounds/typing.wav")));
    // ef->setVolume(global::master_volume * global::sfx_volume);

    // ef->play();
    displayable* disppp = new displayable("icon_inv_collect_shrimp", QPoint(1880, 1010));
    disppp->_disp->setParent(global::w.menu_screen);
    global::w.menu_screen->scene()->addWidget(disppp->_disp);
    disppp->_disp->show();
    global::w.menu_screen->setStyleSheet("background-image: url(:/pictures/testbkg_menu.jpg);");
    global::w.on_menu_b_clicked();

    return global::a.exec();
}
