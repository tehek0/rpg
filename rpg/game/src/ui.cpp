#include "../header/ui.h"
#include "../header/save.h"
#include "QSlider"
#include "QVBoxLayout"

#include "../header/character.h"


game_scene* throw_menu_scene() {
    game_scene* scene_ = new game_scene(new QGraphicsScene(), &global::w);
    scene_->setGeometry(0, 0, global::window_width, global::window_height);
    scene_->setSceneRect(scene_->rect());
    scene_->set_background(":/pictures/testbkg_menu.jpg");

    QPushButton* play_button = new QPushButton("Играть");
    play_button->setGeometry(QRect(20, 900, 400, 50));
    scene_->add(play_button);

    QPushButton* settings_button = new QPushButton("Настройки");
    settings_button->setGeometry(QRect(20, 960, 400, 50));
    scene_->add(settings_button);

    QPushButton* exit_button = new QPushButton("Выйти");
    exit_button->setGeometry(QRect(20, 1020, 400, 50));
    scene_->add(exit_button);

    QLabel* logo = new QLabel();
    logo->setGeometry(QRect(10, 450, 768, 450));
    logo->setPixmap(QPixmap(":/pictures/logo.png"));
    scene_->add(logo);

    auto btn = new QPushButton();
    scene_->add(btn);

    auto ccw = new character_creation_widget();
    scene_->add(ccw);
    ccw->move(600, 0);
    global::w.connect(btn, &QPushButton::clicked, &global::w, [=]() {delete global::w.hub_scene; global::w.hub_scene = nullptr;});

    global::w.connect(play_button, &QPushButton::clicked, &global::w, &MainWindow::menu_play);
    global::w.connect(settings_button, &QPushButton::clicked, &global::w, &MainWindow::open_settings);
    global::w.connect(exit_button, &QPushButton::clicked, &global::w, &MainWindow::menu_exit);
    return scene_;
}

disposable_scene* throw_settings_scene() {
    disposable_scene* scene_ = new disposable_scene(new QGraphicsScene(), &global::w);
    scene_->setGeometry(0, 0, global::window_width, global::window_height);
    scene_->setSceneRect(scene_->rect());
    scene_->set_background(":/pictures/testbkg_settings.jpg");

    QSlider* master_vol = new QSlider(Qt::Horizontal);
    master_vol->setMinimum(0);
    master_vol->setMaximum(100);
    master_vol->setValue(global::master_volume * 100);
    master_vol->connect(master_vol, &QSlider::valueChanged, &global::w, [=](int value) {global::w.change_volume(&global::master_volume, value);});

    QSlider* sfx_vol = new QSlider(Qt::Horizontal);
    sfx_vol->setMinimum(0);
    sfx_vol->setMaximum(100);
    sfx_vol->setValue(global::sfx_volume * 100);
    sfx_vol->connect(sfx_vol, &QSlider::valueChanged, &global::w, [=](int value) {global::w.change_volume(&global::sfx_volume, value);});

    QSlider* music_vol = new QSlider(Qt::Horizontal);
    music_vol->setMinimum(0);
    music_vol->setMaximum(100);
    music_vol->setValue(global::music_volume * 100);
    music_vol->connect(music_vol, &QSlider::valueChanged, &global::w, [=](int value) {global::w.change_volume(&global::music_volume, value);});

    QWidget* layout_widget = new QWidget();
    layout_widget->setGeometry(QRect(710,390,500,300));
    QVBoxLayout* layout = new QVBoxLayout(layout_widget);
    layout->addWidget(new QLabel("Общая громкость:"));
    layout->addWidget(master_vol);
    layout->addWidget(new QLabel("Громкость эффектов:"));
    layout->addWidget(sfx_vol);
    layout->addWidget(new QLabel("Громкость музыки:"));
    layout->addWidget(music_vol);

    QPushButton* back = new QPushButton("Назад");
    game_scene* switch_back_to = global::w.current_scene;
    back->connect(back, &QPushButton::clicked, &global::w, [=]() {global::w.switch_to_scene(switch_back_to);});
    layout->addWidget(back);
    scene_->add(layout_widget);
    return scene_;
}

constexpr int instruments_width = 1700;
constexpr int instruments_panel_hight_start = 200;
constexpr int instrument_side = 100;

game_scene* throw_hub_scene() {
    game_scene* scene_ = new game_scene(new QGraphicsScene(), &global::w);
    scene_->setGeometry(0, 0, global::window_width, global::window_height);
    scene_->setSceneRect(scene_->rect());
    scene_->set_background(":/pictures/testbkg_hub.jpg");

    auto button = new QPushButton;
    global::w.connect(button, &QPushButton::clicked, &global::w, [=]() {global::w.switch_to_scene(global::w.menu_scene);});
    scene_->add(button);
    auto map = new map_widget(QPoint(242, 111), scene_);
    scene_->add(map);
    global::w.connect(map->player_object, &map_player_object::link_line, &global::w, [=](QGraphicsLineItem*& line) {global::w.draw_destination_line(line, map);});
    auto save_btn = new QPushButton("Сохранить");
    save_btn->move(400, 0);
    scene_->add(save_btn);
    auto load_btn = new QPushButton("Загрузить");
    load_btn->move(500, 0);
    scene_->add(load_btn);
    global::w.connect(save_btn, &QPushButton::clicked, &global::w, [=]() {save_map(map, 1);});
    global::w.connect(load_btn, &QPushButton::clicked, &global::w, [=]() {bool x = load_map(map, 1);});

    auto interact_btn = new QPushButton("Зайти");
    interact_btn->resize(instrument_side,instrument_side);
    interact_btn->move(instruments_width, instruments_panel_hight_start);
    global::w.connect(interact_btn, &QPushButton::clicked, &global::w, [=]() {global::w.switch_to_scene(global::w.battle_scene);});
    scene_->add(interact_btn);


    return scene_;
}

constexpr int fight_options_width_start = 300;
constexpr int fight_options_panel_hight = 900;
constexpr int bar_width = 50;
constexpr int gap = 30;
constexpr int battle_ground_hight = 800;
constexpr int player_x = 300;
constexpr int enemy_x[3][3] = {
    {
      200
    },
    {
        150,
        250,
    },
    {
        100,
        150,
        200
    },
    };
constexpr QSize entity_size[7] = {
    {100,100},
    {400,250},
    {700,500},
    {800,700},
    {1000,800},
    {200,400},
    {300,600},
};
battle_scene* throw_battle_scene(player* player, std::vector<enemy*> enemies) {
    battle_scene* scene_ = new battle_scene(new QGraphicsScene(), &global::w);
    scene_->setGeometry(0, 0, global::window_width, global::window_height);
    scene_->setSceneRect(scene_->rect());
    scene_->set_background(":/pictures/testbkg_fight.jpg");

    auto button_b = new QPushButton;
    button_b->resize(50,50);
    global::w.connect(button_b, &QPushButton::clicked, &global::w, [=]() {global::w.switch_to_scene(global::w.menu_scene);});
    scene_->add(button_b);

    int pile = 0;
    for (short i = 0; i < enemies.size(); ++i) {
        QSize size = entity_size[enemies[i]->get_size_class()];
        pile += size.width();
        QPoint place = {global::window_width - enemy_x[enemies.size() - 1][i] - pile, battle_ground_hight - size.height()};
        QProgressBar* healthbar = new QProgressBar();
        int current_bar_width = bar_width + size.width()/2;
        healthbar->resize(current_bar_width, bar_width/2);
        healthbar->move({place.x() + size.width()/2 - current_bar_width/2, battle_ground_hight});
        healthbar->setMaximum(enemies[i]->get_max_health());
        healthbar->setValue(enemies[i]->get_health());
        scene_->add(healthbar);

        enemy_object* enemy_o = new enemy_object(enemies[i],anim_sequence(), place, size, healthbar);
        QObject::connect(enemy_o->_disp, &QPushButton::clicked, enemy_o, &enemy_object::enemy_clicked);
        scene_->add(enemy_o);


    }

    QSize size = entity_size[player->get_size_class()];
    QPoint place = {player_x - size.width(), battle_ground_hight - size.height()};
    entity_object* player_d = new entity_object(player, anim_sequence(), place, size,false);
    scene_->add(player_d);

    auto energy = new QLabel;
    energy->setText(QString("%1/%2").arg(player->get_max_energy()).arg(player->get_max_energy()));
    energy->resize(instrument_side, instrument_side);
    energy->move(fight_options_width_start, fight_options_panel_hight);
    scene_->add(energy);

    auto choose_target = new QPushButton;
    QObject::connect(choose_target, &QPushButton::clicked, scene_, &battle_scene::target_selection_clicked);
    choose_target->setText("Выбрать цель");
    choose_target->resize(instrument_side, instrument_side);
    choose_target->move(fight_options_width_start + instrument_side + gap, fight_options_panel_hight);
    scene_->add(choose_target);

    return scene_;
}