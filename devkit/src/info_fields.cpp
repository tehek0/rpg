#include "../header/info_fields.hpp"
#include <QDebug>
#include <QComboBox>
#include <QPushButton>
#include <QTreeWidget>
#include "../header/inc/json.hpp"
#include "../header/read_object.hpp"
#include "../header/data/general.hpp"
using js = nlohmann::ordered_json;


//INFO_FIELD
void dev::info_field::fill_combo_box_data(QComboBox* field, dev::datatype type) {
    if (type == dev::datatype::boolean){
        field->addItems({"false", "true"});
    }
    else if (dev::is_type_struct(type)){
        QString path = "objects/";
        switch(type) {
        case dev::datatype::requirement:
            path += "requirement";
            break;
        default: break;
        }
        field->addItems(dev::lines_present(path));
    }
    else if (dev::is_type_sybtype(type)) {
        switch(type) {
        case dev::datatype::item_subtypes:
            field->addItems({"none", "weapon", "ammo", "armor", "consumable"});
            break;
        case dev::datatype::requirement_subtypes:
            field->addItems({"none", "char_type", "skill_type", "item_requirements"});
            break;
        default: break;
        }
        connect(field, SIGNAL(currentIndexChanged(int)), field->parent(), SLOT(type_chosen()));
    }
    else {
        switch(type) {
        case dev::datatype::damage_type:
            field->addItems({"bullet", "energy", "melee", "unarmed", "explosive"});
            break;
        case dev::datatype::ammo_type:
            field->addItems({"none", "pistol", "shotgun", "rifle", "energy", "energy_shotgun", "rocket"});
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
};

dev::info_field::info_field(QString key, dev::datatype field_type, QPoint location, QWidget* parent) : field_type_(field_type) {
    this->setParent(parent);
    label_ = new QLabel(parent);
    label_->setText(key);
    label_->setGeometry(location.x(), location.y(), label_w, any_line_hight);

    if (dev::is_type_linear(field_type)) {
        field_ = new QLineEdit(parent);
    }
    else if (dev::is_type_struct(field_type)) {

    }
    else {
        QComboBox* field = new QComboBox(parent);
        fill_combo_box_data(field, field_type);
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
    // if (field_->parent() == nullptr) {
    //     delete optional_add_;
    // }
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
