#include "../header/object_dialogs.hpp"
#include <QDebug>
#include <QComboBox>

#include "../header/data/object_dialog_templates.hpp"

const short label_w = 120;
const short field_w = 300;
const short any_line_hight = 25;
const short gap = 5;
int ypos = 10;
int last_main_field_ypos;



dev::info_field::info_field(QString key, dev::default_types field_type, QPoint location, QWidget* parent) : field_type_(field_type) {
    label_ = new QLabel(parent);
    label_->setText(key);
    label_->setGeometry(location.x(), location.y(), label_w, any_line_hight);

    if (dev::is_type_linear(field_type)) {
        field_ = new QLineEdit(parent);
    }
    else {
        QComboBox* field = new QComboBox(parent);
        if (field_type == dev::default_types::boolean){
            field->addItems({"false", "true"});
        }
        else {
            if (field_type == dev::default_types::item_subtypes){
                field->addItems({"none", "weapon", "ammo", "armor", "consumable"});
                connect(field, SIGNAL(currentIndexChanged(int)), parent, SLOT(type_chosen()));
            }
            if (field_type == dev::default_types::damage_type){
                field->addItems({"bullet", "energy", "melee", "unarmed", "explosive"});
            }
            if (field_type == dev::default_types::ammo_type){
                field->addItems({"none", "pistol", "shotgun", "rifle", "energy", "energy_shotgun", "rocket"});
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
void dev::object_dialog_window::change_subfields(short object_sybtype) {
    for (info_field subf : info_subfields_) {
        subf.get_label()->deleteLater();
        subf.get_field()->deleteLater();
        subf.deleteLater();
    }
    info_subfields_.clear();
    ypos = last_main_field_ypos;

    this->resize(500, 600);

    short amount_of_fields;
    try {
        amount_of_fields = subtypes_template[object_type_][object_sybtype].keys_.length();
    } catch (...) {
        qInfo() << "[WARN][dev::add_subfields] tried to add a fields for non existing sybtype of object";
        amount_of_fields = 0;
    }

    for (short i = 0; i < amount_of_fields; ++i) {
        QString current_key = subtypes_template[object_type_][object_sybtype].keys_[i];
        dev::default_types current_type = subtypes_template[object_type_][object_sybtype].types_[i];

        dev::info_field whole_field = {current_key, current_type, QPoint(gap, ypos), this};
        info_subfields_.emplace_back(whole_field);

        ypos += any_line_hight + gap;
    }
}
dev::object_dialog_window::object_dialog_window(dev::datatype object_type) : QWidget(), object_type_(object_type) {
    this->setBaseSize(500, 800);

    short amount_of_fields;
    try {
        amount_of_fields = templates[object_type].keys_.length();
    } catch (...) {
        qInfo() << "[WARN][dev::object_dialog_window] tried to build a window for non existing type of object";
        amount_of_fields = 0;
    }

    for (short i = 0; i < amount_of_fields; ++i) {
        QString current_key = templates[object_type].keys_[i];
        dev::default_types current_type = templates[object_type].types_[i];

        dev::info_field whole_field = {current_key,current_type, QPoint(gap, ypos), this};
        info_fields_.emplace_back(whole_field);

        ypos += any_line_hight + gap;
    }
    last_main_field_ypos = ypos;

    save_ = new QPushButton(this);
    save_->setText("Save");
    save_->setGeometry(gap*3 + label_w + field_w, gap, label_w/2, any_line_hight*2);
    connect(save_, SIGNAL(clicked()), this, SLOT(on_save_clicked()));

    this->show();
}


void dev::object_dialog_window::type_chosen() {
    short object_sybtype = dynamic_cast<QComboBox*>(this->info_fields_[0].get_field())->currentIndex();
    change_subfields(object_sybtype);
}
void dev::object_dialog_window::on_save_clicked() {
    qInfo() << "save";
}
