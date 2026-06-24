#include "../header/object_dialogs.hpp"
#include <QDebug>
#include <QComboBox>
#include "../header/data/object_dialog_templates.hpp"
#include "../header/data/general.hpp"

//OBJECT_DIALOG_WINDOW
void dev::object_dialog_window::delete_subfields() {
    for (info_field subf : info_subfields_) {
        subf.get_label()->deleteLater();
        subf.get_field()->deleteLater();
        subf.deleteLater();
    }
    info_subfields_.clear();
    this->ypos = this->last_y_pos;
}
void dev::object_dialog_window::add_subfields(short object_sybtype) {
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
void dev::object_dialog_window::change_subfields(short object_sybtype) {
    delete_subfields();
    add_subfields(object_sybtype);
}

dev::object_dialog_window::object_dialog_window(dev::datatype object_type) : QWidget(), object_type_(object_type) {
    this->resize(600, 450);
    this->setAttribute(Qt::WA_DeleteOnClose);

    save_ = new QPushButton(this);
    save_->setText("Save");
    save_->setGeometry(gap, gap, label_w/2, any_line_hight*2);
    connect(save_, SIGNAL(clicked()), this, SLOT(on_save_clicked()));
    reset_ = new QPushButton(this);
    reset_->setText("Reset");
    reset_->setGeometry(gap*2 + label_w/2, gap, label_w/2, any_line_hight*2);
    connect(reset_, SIGNAL(clicked()), this, SLOT(on_reset_clicked()));
    this->ypos = any_line_hight*2 + gap;

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

