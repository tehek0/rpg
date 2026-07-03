#pragma once
#include "custom_window.h"
#include "qlineedit.h"
#include <QGridLayout>
#include <QPushButton>

namespace dev {

enum biome {
    none,
    desert,
    city,
    swamp,
    plateau,
    forest,
    mutant_forest,
    crater,
};
struct map_poi : public QObject{
    Q_OBJECT
public:
    map_poi() {disp_ = new QPushButton();};
    map_poi(QString sprite_family, unsigned long long location_id,QString location_name);
    ~map_poi() {if (disp_->parent() == nullptr) {delete disp_;}};

    unsigned long long location_id_;
    QString location_name_;
    QString sprite_family_;

    QPushButton* disp_;
};

class map_grid_tile : public QPushButton {
    Q_OBJECT
public:
    bool is_locked_ = true;
    biome biome_ = biome::none;
    map_poi* poi_ = nullptr;
    bool is_poi_real = false;
    float difficulty_ = 0;

    map_grid_tile(unsigned short size);
    ~map_grid_tile() {delete poi_;}

    void change_tile_tint();
    void change_tile_text();
signals:
    void clicked_tile(map_grid_tile* tile);
    void clicked_poi(map_grid_tile* tile);
public slots:
    void process_click() {
        emit clicked_tile(this);
    }
    void process_click_poi() {
        emit clicked_poi(this);
    }
};

class map_grid : public QWidget {
    Q_OBJECT
private:
    QGridLayout* layout_;
    std::vector<map_grid_tile*> tiles_;
    unsigned short width_ = 20;
    unsigned short height_ = 12;
    unsigned short tile_size_ = 50;
public:
    map_grid(unsigned short width, unsigned short height, unsigned short tile_size = 50, QWidget* parent = nullptr);
    ~map_grid();

    std::vector<map_grid_tile*> get_tiles() {return tiles_;}
    //map_grid_tile* get_tile_at(QPoint coord);
signals:
    void clicked_child_tile(map_grid_tile* tile);
    void clicked_child_poi(map_grid_tile* tile);
public slots:
    void process_clicked_tile(map_grid_tile* tile) {
        emit clicked_child_tile(tile);
    }
    void process_clicked_poi(map_grid_tile* tile) {
        emit clicked_child_poi(tile);
    }

};

enum class map_brush {
    none,
    locker,
    player,
    poi,
    biome,
};

class map_construct_window : public custom_window {
    Q_OBJECT
private:
    map_brush brush_ = map_brush::none;
    map_grid* grid_;
    std::vector<QPushButton*> buttons_;
    std::vector<QLabel*> labels_;
    std::vector<QLineEdit*> edits_;
    std::vector<QComboBox*> boxes_;

    void add_save_button();
    void add_load_button();
    void add_brushes();
public:
    map_construct_window();
    ~map_construct_window();

    map_grid* get_grid() {return grid_;}
public slots:
    void on_save_clicked();
    //bool on_load_clicked();
    void on_locked_brush_clicked();
    void on_player_brush_clicked();
    void on_poi_brush_clicked();
    void on_biome_brush_clicked();

    void clicked_tile(map_grid_tile* tile);
    void clicked_poi(map_grid_tile* tile);
};

}





