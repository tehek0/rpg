#include "../header/object_dialogs.hpp"
#include <QDebug>
#include <QComboBox>
#include "../header/data/object_dialog_templates.hpp"
#include "../rpg/game/inc/json.hpp"
using js = nlohmann::ordered_json;

//Параметры размера и положения объектов на экране, общие для всех окон
constexpr short label_w = 120;
constexpr short field_w = 300;
constexpr short any_line_hight = 25;
constexpr short gap = 5;


//INFO_FIELD
dev::info_field::info_field(QString key, dev::datatype field_type, QPoint location, QWidget* parent) : field_type_(field_type) {
    label_ = new QLabel(parent);
    label_->setText(key);
    label_->setGeometry(location.x(), location.y(), label_w, any_line_hight);

    if (dev::is_type_linear(field_type)) {
        field_ = new QLineEdit(parent);
    }
    else {
        QComboBox* field = new QComboBox(parent);      
        if (field_type == dev::datatype::boolean){
            field->addItems({"false", "true"});
        }
        else {
            switch(field_type) {
            case dev::datatype::item_subtypes:
                field->addItems({"none", "weapon", "ammo", "armor", "consumable"});
                connect(field, SIGNAL(currentIndexChanged(int)), parent, SLOT(type_chosen()));
                break;
            case dev::datatype::damage_type:
                field->addItems({"bullet", "energy", "melee", "unarmed", "explosive"});
                break;
            case dev::datatype::ammo_type:
                field->addItems({"none", "pistol", "shotgun", "rifle", "energy", "energy_shotgun", "rocket"});
                break;
            case dev::datatype::requirements_subtypes:
                field->addItems({"none", "char_type", "skill_type"});
                connect(field, SIGNAL(currentIndexChanged(int)), parent, SLOT(type_chosen()));
                break;
            case dev::datatype::char_type:
                field->addItems({"strength", "agility", "endurance", "intelligence", "luck"});
                break;
            case dev::datatype::skill_type:
                field->addItems({"guns", "big_guns", "unarmed", "science", "spech", "barter", "survival"});
                break;
            default: break;
            }

        }
        field_ = field;
    }
    field_->setGeometry(location.x()+ label_w + gap, location.y(), field_w , any_line_hight);
    label_->show();
    field_->show();
};

dev::info_field::info_field(const info_field& other) {
    label_ = other.label_;
    field_ = other.field_;
    field_type_ = other.field_type_;
};

dev::info_field::~info_field() {
    //Когда поля привязаны к pqrent окну, окно само запускает для них delete
    if (label_->parent() == nullptr) {
        delete label_;
    }
    if (field_->parent() == nullptr) {
        delete field_;
    }
}

void dev::info_field::clear_info_field() {
    if (is_type_linear(field_type_)) {
        dynamic_cast<QLineEdit*>(field_)->clear();
    }
    else {
        dynamic_cast<QComboBox*>(field_)->setCurrentIndex(0);
    }
}

void dev::read_from_infoField_to_objectData(const dev::info_field& data, dev::object_data& object) {
    QString current_key = data.get_label()->text();
    dev::datatype current_type = data.get_field_type();
    QString current_value;
    if (is_type_linear(current_type)) {
        current_value = dynamic_cast<QLineEdit*>(data.get_field())->text();
    }
    else {
        current_value = QString::number(dynamic_cast<QComboBox*>(data.get_field())->currentIndex());
    }

    object.keys_.append(current_key);
    object.values_.append(current_value);
    object.types_.emplace_back(current_type);

}

//OBJECT_DIALOG_WINDOW
void dev::object_dialog_window::change_subfields(short object_sybtype) {
    for (info_field subf : info_subfields_) {
        subf.get_label()->deleteLater();
        subf.get_field()->deleteLater();
        subf.deleteLater();
    }
    info_subfields_.clear();
    this->ypos = this->last_y_pos;

    //this->resize(500, 600);

    short amount_of_fields;
    try {
        amount_of_fields = subtypes_template[object_type_][object_sybtype].keys_.length();
    } catch (...) {
        qInfo() << "[WARN][dev::add_subfields] tried to add a fields for non existing sybtype of object";
        amount_of_fields = 0;
    }

    for (short i = 0; i < amount_of_fields; ++i) {
        QString current_key = subtypes_template[object_type_][object_sybtype].keys_[i];
        dev::datatype current_type = subtypes_template[object_type_][object_sybtype].types_[i];

        dev::info_field whole_field = {current_key, current_type, QPoint(gap, this->ypos), this};
        info_subfields_.emplace_back(whole_field);

        this->ypos += any_line_hight + gap;
    }
}

dev::object_dialog_window::object_dialog_window(dev::datatype object_type) : QWidget(), object_type_(object_type) {
    this->resize(500, 400);
    this->setAttribute(Qt::WA_DeleteOnClose);

    short amount_of_fields;
    try {
        amount_of_fields = templates[object_type].keys_.length();
    } catch (...) {
        qInfo() << "[WARN][dev::object_dialog_window] tried to build a window for non existing type of object";
        amount_of_fields = 0;
    }

    for (short i = 0; i < amount_of_fields; ++i) {
        QString current_key = templates[object_type].keys_[i];
        dev::datatype current_type = templates[object_type].types_[i];

        dev::info_field whole_field = {current_key,current_type, QPoint(gap, this->ypos), this};
        info_fields_.emplace_back(whole_field);

        this->ypos += any_line_hight + gap;
    }
    this->last_y_pos = this->ypos;

    save_ = new QPushButton(this);
    save_->setText("Save");
    save_->setGeometry(gap*3 + label_w + field_w, gap, label_w/2, any_line_hight*2);
    connect(save_, SIGNAL(clicked()), this, SLOT(on_save_clicked()));
    reset_ = new QPushButton(this);
    reset_->setText("Reset");
    reset_->setGeometry(gap*3 + label_w + field_w, gap*2 + any_line_hight*2, label_w/2, any_line_hight*2);
    connect(reset_, SIGNAL(clicked()), this, SLOT(on_reset_clicked()));

    this->show();
}

dev::object_dialog_window::~object_dialog_window() {
    this->ypos = 10;
    delete save_;
}

//SLOTS
void dev::object_dialog_window::type_chosen() {
    short object_sybtype = dynamic_cast<QComboBox*>(this->info_fields_[0].get_field())->currentIndex();
    change_subfields(object_sybtype);
}
void dev::object_dialog_window::on_save_clicked() {
    object_data object;

    for (const info_field& data : info_fields_) {
        read_from_infoField_to_objectData(data, object);
    }
    for (const info_field& data : info_subfields_) {
        read_from_infoField_to_objectData(data, object);
    }

    create_object(object_type_, object);
    qInfo() << "save";
}

void dev::object_dialog_window::on_reset_clicked() {
    for (int i = 0; i < info_fields_.size(); ++i) {
        info_fields_[i].clear_info_field();
    }
    for (int i = 0; i < info_subfields_.size(); ++i) {
        info_subfields_[i].clear_info_field();
    }
}
