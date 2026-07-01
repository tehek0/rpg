#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "game/header/scenes.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    game_scene* menu_scene;
    game_scene* hub_scene = nullptr;
    game_scene* dialog_scene = nullptr;
    game_scene* location_scene = nullptr;
    battle_scene* battle_scene = nullptr;
    game_scene* current_scene = nullptr;

    Ui::MainWindow *ui;
public slots:
    void menu_play();
    void new_game();
    void open_settings();
    void change_volume(double* source, float value);
    void menu_exit();
    void switch_to_scene(game_scene* scene_);
    void draw_destination_line(QGraphicsLineItem*& line, game_scene* map_);
private:

};
#endif // MAINWINDOW_H
