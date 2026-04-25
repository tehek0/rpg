#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game/header/character.h"
#include "game/header/global.h"
#include <QPushButton>

/*struct sprite_size {
    QString label;
    QSize parameters;
};
const unsigned int amount_of_sprite_sizes = 10;
const sprite_size sprite_size[amount_of_sprite_sizes] {
    {"ui_button", QSize(50,50)},
    {"map_city", QSize(100,100)}
};*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    global::timer->start(global::tick_timeout);
    on_inventory.emplace_back(ui->pushButton);
    ui->pushButton->hide();

    on_menu.emplace_back(ui->menu_exit);
    on_menu.emplace_back(ui->menu_toGame);


    //test
    QPoint p(1000,100);
    QSize z(100,100);
    QString s = "shrimp";
    QString n = "test";
    animated_displayable* ent = new animated_displayable(this,true,p,z,s,n, anim_sequence(0, anim("test_anim", 3, 6, false, true)));
    connect(ent->_disp, &QPushButton::clicked, this, [=]() {this->OnEntClicked(ent);});
    on_map.emplace_back(ent->_disp);
    ent->_disp->hide();
    //
}

MainWindow::~MainWindow()
{
    delete ui;
}

//test
int ii = 30;
void MainWindow::on_pushButton_clicked()
{
    QPoint p(100+ii,100);
    QSize z(100,100);
    QString s = "shrimp";
    QString n = "test";
    ii += 50;
    animated_displayable* ent = new animated_displayable(this,true,p,z,s,n, anim_sequence(0, anim("test_anim", (std::rand() % (20 - 10 + 1)) + 10, 6)));
    connect(ent->_disp, &QPushButton::clicked, this, [=]() {this->OnEntClicked(ent);});
    on_stats.emplace_back(ent->_disp);
}

void MainWindow::OnEntClicked() {
    qInfo() << "Кнопка нажимается";
}

void MainWindow::OnEntClicked(animated_displayable* ent_) {
    QPoint new_destination = ent_->_disp->pos();
    QPoint center_point = ent_->_disp->rect().center();
    QPoint cursor_pos = ent_->_disp->mapFromGlobal(QCursor::pos());
    float vector_coef = 3.5f;
    int dx = vector_coef * (center_point.x() - cursor_pos.x());
    int dy = vector_coef * (center_point.y() - cursor_pos.y());
    new_destination.setX(new_destination.x() + dx);
    new_destination.setY(new_destination.y() + dy);
    ent_->begin_step(new_destination, 80, transpos_algs::bounce_out);
    ent_->set_swap_destinations(1);

    ent_->switch_paused();
    if (ent_->get_anim_sequence().paused) {
        qInfo() << QString("Остановлена анимация объекта %1 на кадре %2").arg(ent_->get_name()).arg(ent_->get_anim_sequence().current_frame);
        return;
    }
    qInfo() << QString("Анимация объекта %1 возобновлена").arg(ent_->get_name());

}

void debug_screen(const std::vector<QWidget*> &screen) {
    for (int i = 0; i< screen.size(); ++i){
        qInfo() << screen[i];
    }
    qInfo() << '\n';
}
void show_ui(MainWindow* w) {
    w->ui->menu_b->show();
    w->ui->map_b->show();
    w->ui->stats_b->show();
    w->ui->inventory_b->show();
}
void hide_ui(MainWindow* w) {
    w->ui->menu_b->hide();
    w->ui->map_b->hide();
    w->ui->stats_b->hide();
    w->ui->inventory_b->hide();
}

void hide_widgets(const std::vector<QWidget*> &screen) {
    for(size_t i = 0; i < screen.size(); ++i) {
        screen[i]->hide();
    }
}

void show_widgets(const std::vector<QWidget*> &screen) {
    for(size_t i = 0; i < screen.size(); ++i) {
        screen[i]->show();
    }
}

void MainWindow::on_map_b_clicked()
{
    if (!(current_screen == screens::map)) {
        if (current_screen == screens::menu) {
            show_ui(this);
        }
        debug_screen(on_screen);

        centralWidget()->setStyleSheet("background: url(:/map.jpg); background-position: center;");

        hide_widgets(on_screen);
        on_screen = on_map;
        show_widgets(on_screen);

        debug_screen(on_screen);

        current_screen = screens::map;
    }

}

void MainWindow::on_inventory_b_clicked()
{
    if (!(current_screen == screens::inventory)) {
        if (current_screen == screens::menu) {
            show_ui(this);
        }

        debug_screen(on_screen);

        centralWidget()->setStyleSheet("background: url(:/testbkg.jpg); background-position: center;");

        hide_widgets(on_screen);
        on_screen = on_inventory;
        show_widgets(on_screen);

        debug_screen(on_screen);

        current_screen = screens::inventory;
    }

}


void MainWindow::on_stats_b_clicked()
{
    if (!(current_screen == screens::stats)) {
        if (current_screen == screens::menu) {
            show_ui(this);
        }

        debug_screen(on_screen);

        centralWidget()->setStyleSheet("background: url(:/testbkg_stats.jpg); background-position: center;");

        hide_widgets(on_screen);
        on_screen = on_stats;
        show_widgets(on_screen);

        debug_screen(on_screen);

        current_screen = screens::stats;
    }
}


void MainWindow::on_menu_b_clicked()
{
    if (!(current_screen == screens::menu)) {

        debug_screen(on_screen);

        centralWidget()->setStyleSheet("background: url(:/testbkg_menu.jpg); background-position: center;");
        hide_ui(this);

        hide_widgets(on_screen);
        on_screen = on_menu;
        show_widgets(on_screen);

        debug_screen(on_screen);

        current_screen = screens::menu;
    }

}


void MainWindow::on_menu_toGame_clicked()
{
    on_map_b_clicked();
}


void MainWindow::on_menu_exit_clicked()
{
    this->close();
}

