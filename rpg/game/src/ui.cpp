#include "../header/ui.h"
#include "../header/save.h"
#include "QSlider"
#include "QVBoxLayout"

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
    return scene_;
}
