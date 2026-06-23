#include "../header/ui.h"

game_scene* throw_menu_scene() {
    game_scene* scene_ = new game_scene(new QGraphicsScene(), &global::w);
    scene_->setGeometry(0, 0, global::window_width, global::window_height);
    scene_->setBackgroundBrush(QBrush(QPixmap(":/pictures/testbkg_menu.jpg")));
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

    global::w.connect(play_button, &QPushButton::clicked, &global::w, &MainWindow::menu_play);
    global::w.connect(exit_button, &QPushButton::clicked, &global::w, &MainWindow::menu_exit);
    return scene_;
}
