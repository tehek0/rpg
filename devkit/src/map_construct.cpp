#include "../header/map_construct.hpp"
#include "../header/data/general.hpp"
#include <QPoint>
#include "../header/inc/json.hpp"
#include <fstream>
#include "../header/id_support.hpp"
using js = nlohmann::ordered_json;

void dev::map_grid_tile::change_tile_tint() {
    if (is_locked_ == true) {
        this->setStyleSheet(QString("background: rgba(17, 9, 52, 100);"));
    } else {
        this->setStyleSheet(QString("background: rgba(17, 9, 52, 0);"));
    }
}

dev::map_poi::map_poi(QString sprite_family, unsigned long long location_id,QString location_name)
    : location_id_(location_id), location_name_(location_name), sprite_family_(sprite_family) {
    disp_ = new QPushButton();
}

dev::map_grid_tile::map_grid_tile(unsigned short size) {
    setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
    this->setGeometry(0, 0, size, size);
    this->setText("-");
    poi_ = new map_poi();

    connect(this, &QPushButton::clicked, this, &map_grid_tile::process_click);
    connect(poi_->disp_, &QPushButton::clicked, this, &map_grid_tile::process_click_poi);

    change_tile_tint();

}
dev::map_grid::map_grid(unsigned short width, unsigned short height, unsigned short tile_size, QWidget* parent): QWidget(parent) {
    width_ = width;
    height_ = height;
    tile_size_ = tile_size;
    this->setGeometry(0, 0, width_ * tile_size_, height_ * tile_size_);
    layout_ = new QGridLayout(this);
    layout_->setContentsMargins(0,0,0,0);
    layout_->setSpacing(0);


    tiles_.reserve(width_ * height_);
    for (unsigned int i = 0; i < width_ * height_; ++i) {
        auto tile = new map_grid_tile(tile_size_);
        tiles_.emplace_back(tile);
        layout_->addWidget(tile, i / width_, i % width_);
        connect(tile, &map_grid_tile::clicked_tile, this, &map_grid::process_clicked_tile);
        connect(tile, &map_grid_tile::clicked_poi, this, &map_grid::process_clicked_poi);
    }
    show();
}

dev::map_grid::~map_grid() {
    for (auto elem : tiles_) {
        elem->deleteLater();
    }
    delete layout_;
}

// dev::map_grid_tile* dev::map_grid::get_tile_at(QPoint coord) {
//     unsigned int row = coord.y() / tile_size_;
//     unsigned int col = coord.x() / tile_size_;
//     unsigned int index = (row * width_) + col;
//     if (row >= height_ || row < 0 || col >= width_ || col < 0)
//         return nullptr;

//     return tiles_[index];
// }


void dev::map_construct_window::add_save_button() {
    QPushButton* save = new QPushButton("Save", this);
    save->setGeometry(1000, gap, button_side, button_side);
    connect(save, SIGNAL(clicked()), this, SLOT(on_save_clicked()));
    save->show();
    buttons_.push_back(save);
}

// void dev::map_construct_window::add_load_button() {
//     load = new QPushButton("Load", this);
//     load->setGeometry(1000+button_side+gap , gap, button_side, button_side);
//     connect(load, SIGNAL(clicked()), this, SLOT(on_load_clicked()));
//     load->show();
//buttons_.push_back(load);
// }

void dev::map_construct_window::add_brushes() {
    QLabel* player_object = new QLabel("Игрок", this);
    player_object->setGeometry(10,10,button_side/1.5, button_side/2);
    player_object->setStyleSheet("background: rgb(12,100,100);");
    player_object ->show();

    QLabel* brushes = new QLabel("Кисти", this);
    brushes->setGeometry(1000, button_side + 3*gap, label_w, any_line_hight);
    brushes->show();

    QPushButton* locked_brush = new QPushButton("Открывашка", this);
    locked_brush->setGeometry(1000, any_line_hight + button_side + 3*gap, button_side*2, button_side);
    locked_brush->show();
    connect(locked_brush, SIGNAL(clicked()), this, SLOT(on_locked_brush_clicked()));

    QPushButton * player_brush = new QPushButton("Сдвинуть игрока", this);
    player_brush->setGeometry(1000, button_side*2 + 5*gap + any_line_hight, button_side*2, button_side);
    player_brush->show();
    connect(player_brush, SIGNAL(clicked()), this, SLOT(on_player_brush_clicked()));

    QPushButton * poi_brush = new QPushButton("Поставить poi", this);
    poi_brush->setGeometry(1000, button_side*3 + 7*gap + any_line_hight, button_side*2, button_side);
    poi_brush->show();
    connect(poi_brush, SIGNAL(clicked()), this, SLOT(on_poi_brush_clicked()));

    QLineEdit* poi_id = new QLineEdit("poi id", this);
    QLineEdit* poi_name = new QLineEdit("poi name",this);
    QLineEdit* poi_sprite_family = new QLineEdit("poi sprite family", this);
    poi_id->setGeometry(1000, button_side*4 + 9*gap + any_line_hight, field_w, any_line_hight);
    poi_name->setGeometry(1000, button_side*4 + 11*gap + any_line_hight*2, field_w, any_line_hight);
    poi_sprite_family->setGeometry(1000, button_side*4 + 13*gap + any_line_hight*3, field_w, any_line_hight);
    poi_id->show();
    poi_name->show();
    poi_sprite_family->show();

    edits_.emplace_back(poi_id);
    edits_.emplace_back(poi_name);
    edits_.emplace_back(poi_sprite_family);
    labels_.emplace_back(player_object);
    labels_.emplace_back(brushes);
    buttons_.emplace_back(locked_brush);
    buttons_.emplace_back(player_brush);
}
dev::map_construct_window::map_construct_window() {
    grid_ = new map_grid(19, 12, 50, this);
    this->setGeometry(50,50, 1200,600);
    connect(grid_, &map_grid::clicked_child_tile, this, &map_construct_window::clicked_tile);
    connect(grid_, &map_grid::clicked_child_poi, this, &map_construct_window::clicked_poi);

    add_save_button();
    //add_load_button();
    add_brushes();
}

void dev::map_construct_window::clicked_poi(map_grid_tile* tile) {
    tile->poi_->location_id_ = edits_[0]->text().toULongLong();
    tile->poi_->location_name_ = edits_[1]->text();
    tile->poi_->sprite_family_ = edits_[2]->text();
    tile->poi_->disp_->setStyleSheet("background: rgb(30,30,0);");
}
void dev::map_construct_window::clicked_tile(map_grid_tile* tile) {
    switch (brush_) {
    case dev::map_brush::none : break;
    case dev::map_brush::locker :
        tile->is_locked_ = !tile->is_locked_;
        break;
    case dev::map_brush::player :
        labels_[0]->move(tile->pos().x() + 25, tile->pos().y() + 25);
        break;
    case dev::map_brush::poi :
        tile->poi_->disp_->setParent(this);
        tile->poi_->disp_->setGeometry(tile->pos().x() + 25, tile->pos().y() + 25, button_side/2, button_side/2);
        tile->poi_->disp_->show();
        tile->is_poi_real = true;
        break;
    }
    tile->change_tile_tint();
}

void dev::map_construct_window::on_player_brush_clicked() {
    if (brush_ == map_brush::player) {
        brush_ = map_brush::none;
    }
    else {
        brush_ = map_brush::player;
    }
}

void dev::map_construct_window::on_locked_brush_clicked() {
    if (brush_ == map_brush::locker) {
        brush_ = map_brush::none;
    }
    else {
        brush_ = map_brush::locker;
    }
};

void dev::map_construct_window::on_poi_brush_clicked() {
    if (brush_ == map_brush::poi) {
        brush_ = map_brush::none;
    }
    else {
        brush_ = map_brush::poi;
    }
};

dev::map_construct_window::~map_construct_window() {
    delete grid_;
}


void dev::map_construct_window::on_save_clicked() {
    QString path = dev::path_to_rpg_exe + "/objects/";
    //unsigned long long id = dev::throw_id(dev::datatype::map);
    // path += "/%1";
    // QString folder_path = QString(path).arg(datatypes_to_string[dev::datatype::map]);
    // if (!std::filesystem::exists(folder_path.toStdString())) {
    //     std::filesystem::create_directories(folder_path.toStdString());
    // }
    // path = (folder_path + QString("/%1_%2.json").arg(datatypes_to_string[dev::datatype::map]).arg(id));
    path += "map.json";
    std::ofstream file(path.toStdString());
    file.clear();
    js j = js::object();
    j["player_x"] = labels_[0]->pos().x()/50*75;
    j["player_y"] = labels_[0]->pos().y()/50*75;
    j["map"];
    js map_j = js::array();
    for (int i = 0; i < grid_->get_tiles().size(); ++i) {
        js tile = js::object();
        map_grid_tile* tile_obj = grid_->get_tiles()[i];
        tile["locked"] = tile_obj->is_locked_;
        if (tile_obj->is_poi_real == true) {
            js poi = js::object();
            poi["name"] = tile_obj->poi_->location_name_.toStdString();
            poi["location_id"] = tile_obj->poi_->location_id_;
            poi["sprite"] = tile_obj->poi_->sprite_family_.toStdString();
            tile["poi"] = poi;
        }
        tile["difficulty"] = tile_obj->difficulty_;
        tile["biome"] = static_cast<int>(tile_obj->biome_);
        map_j.emplace_back(tile);
    }
    j["map"] = map_j;
    file << j.dump(js_indent);
}
// void dev::map_construct_window::on_save_clicked(){
//     unsigned long long id = dev::throw_id(dev::datatype::map);
//     std::string path = "objects/maps" + QString("/%1_%2").arg(datatypes_to_string[dev::datatype::map]).arg(id).toStdString();
//     std::ofstream file(path);
//     js j = js::object();
//     j["map"];
//     js map_j = js::array();
//     for (int i = 0; i < grid_->get_tiles().size(); ++i) {
//         js tile = js::object();
//         map_grid_tile* tile_obj = grid_->get_tiles()[i];
//         tile["locked"] = tile_obj->is_locked_;
//         if (!(tile_obj->poi_ == nullptr)) {
//             js poi = js::object();
//             poi["name"] = tile_obj->poi_->location_name_.toStdString();
//             poi["location_id"] = tile_obj->poi_->location_id_;
//             poi["sprite"] = tile_obj->poi_->sprite_family_.toStdString();
//             tile["poi"] = poi;
//         }
//         tile["difficulty"] = tile_obj->difficulty_;
//         tile["biome"] = static_cast<int>(tile_obj->biome_);
//         map_j.emplace_back(tile);
//     }
//     j["map"] = map_j;
//     file << j.dump(js_indent);
//}
// bool dev::map_construct_window::on_load_clicked(){
//     std::string path = QString("objects/maps/map_%1.json").arg(dev::get_la(dev::datatype::map)).toStdString();
//     std::ifstream file(path);
//     js j;
//     try {
//         j = js::parse(file);
//     } catch (...) {
//         return false;
//     }
//     if (!j.is_object()) {
//         return false;
//     }
//     js map_j;
//     if (j.contains("map")) {
//         map_j = j["map"];
//     } else {
//         return false;
//     }
//     if (!map_j.is_array()) {
//         return false;
//     }
//     if (map_j.size() != grid_->get_tiles().size()) {
//         return false;
//     }
//     for (int i = 0; i < grid_->get_tiles().size(); ++i) {
//         map_grid_tile* tile_obj = grid_->get_tiles()[i];
//         js tile = map_j[i];
//         if (!tile.contains("locked") || !tile.contains("difficulty") || !tile.contains("biome")) {
//             return false;
//         }
//         if (!tile["locked"].is_boolean() || !tile["difficulty"].is_number_float() || !tile["biome"].is_number_unsigned()) {
//             return false;
//         }
//         bool locked;
//         int biome_;
//         float difficulty;
//         tile["locked"].get_to(locked);
//         tile["biome"].get_to(biome_);
//         tile["difficulty"].get_to(difficulty);
//         if (!locked) {
//             tile_obj->is_locked_ = false;
//         } else {
//             tile_obj->is_locked_ = true;
//         }
//         tile_obj->biome_ = biome{biome_};
//         tile_obj->difficulty_ = difficulty;
//         if (tile.contains("poi")) {
//             js tile_poi = tile["poi"];

//             if (!tile_poi.is_object())
//                 return false;

//             if (!tile_poi.contains("sprite") || !tile_poi.contains("name") || !tile_poi.contains("location_id"))
//                 return false;

//             if (!tile_poi["sprite"].is_string() || !tile_poi["name"].is_string() || !tile_poi["location_id"].is_number_unsigned())
//                 return false;

//             std::string sprite;
//             std::string name;
//             unsigned long long location_id;

//             tile_poi["sprite"].get_to(sprite);
//             tile_poi["name"].get_to(name);
//             tile_poi["location_id"].get_to(location_id);

//             map_poi* poi = new map_poi(QString::fromStdString(sprite), location_id, QString::fromStdString(name));
//             tile_obj->poi_ = poi;
//         }
//     }
//     qInfo() << 'l';
//     return true;
// }