#include "../header/object_dialogs.hpp"
#include <QDebug>
#include <QComboBox>
#include "../header/data/object_dialog_templates.hpp"
#include "../header/data/general.hpp"
#include "../header/read_object.hpp"

void dev::object_dialog_window::resize_dependent_on_fields() {
    short additional = 0;
    for (auto& field : info_fields_) {
        if (dev::is_type_struct(field.get_field_type())) {
            additional += field.calculate_table_hight();
        }
    }
    for (auto& field : info_subfields_) {
        if (dev::is_type_struct(field.get_field_type())) {
            additional += field.calculate_table_hight();
        }
    }
    resize(label_w + field_w + button_side + gap*3, button_side + (info_fields_.size() + info_subfields_.size()) * (any_line_hight + gap) + additional + gap);
};

void dev::object_dialog_window::delete_subfields() {
    for (info_field subf : info_subfields_) {
        subf.get_label()->deleteLater();
        subf.get_field()->deleteLater();
        subf.deleteLater();
    }
    info_subfields_.clear();
    ypos = last_y_pos;
}

void dev::object_dialog_window::add_subfields(short object_sybtype) {
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

        if (is_type_struct(current_type)) {
            ypos += whole_field.calculate_table_hight();
        }
        else {
            ypos += any_line_hight + gap;
        }
        resize_dependent_on_fields();
    }

}
void dev::object_dialog_window::change_subfields(short object_sybtype) {
    delete_subfields();
    add_subfields(object_sybtype);
}

void dev::object_dialog_window::add_save_button() {
    save_ = new QPushButton(this);
    save_->setText("Save");
    save_->setGeometry(gap, gap, button_side, button_side);
    connect(save_, SIGNAL(clicked()), this, SLOT(on_save_clicked()));
    save_->show();
}

void dev::object_dialog_window::add_delete_button() {
    delete_ = new QPushButton(this);
    delete_->setText("Delete");
    delete_->setGeometry(gap, gap, button_side, button_side);
    connect(delete_, SIGNAL(clicked()), this, SLOT(on_delete_clicked()));
    delete_->show();
}

dev::object_dialog_window::object_dialog_window() {
    reset_ = nullptr;
    save_ = nullptr;
    delete_ = nullptr;
}

dev::object_dialog_window::object_dialog_window(dev::datatype object_type) : custom_window(), object_type_(object_type) {
    this->setWindowTitle(QString::fromStdString(datatypes_to_string[object_type]));

    if (object_type == dev::erased) {
        add_delete_button();
    }
    else {
        add_save_button();
    }
    reset_ = new QPushButton(this);
    reset_->setText("Reset");
    reset_->setGeometry(gap*2 + label_w/2, gap, button_side, button_side);
    connect(reset_, SIGNAL(clicked()), this, SLOT(on_reset_clicked()));
    ypos = button_side + gap*2;
    reset_->show();

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

        if (is_type_struct(current_type)) {
            this->ypos += whole_field.calculate_table_hight();
        }
        else {
            this->ypos += any_line_hight + gap;
        }

    }
    last_y_pos = ypos;

    resize_dependent_on_fields();
    show();
}

dev::object_dialog_window::~object_dialog_window() {
    ypos = 10;
    delete reset_;
    if (object_type_ == dev::datatype::erased) {
        delete delete_;
    }
    else {
        delete save_;
    }
}

//SLOTS
void dev::object_dialog_window::on_save_clicked() {
    object_data object;

    for (const info_field& data : info_fields_) {
        read_from_infoField_to_objectData(data, object);
    }
    for (const info_field& data : info_subfields_) {
        read_from_infoField_to_objectData(data, object);
    }

    create_object(object_type_, object);
    ui::inform->setText("Сохранено.");
}
void dev::object_dialog_window::on_reset_clicked() {
    for (int i = 0; i < info_fields_.size(); ++i) {
        info_fields_[i].clear_info_field();
    }
    for (int i = 0; i < info_subfields_.size(); ++i) {
        info_subfields_[i].clear_info_field();
    }
}

void dev::object_dialog_window::on_delete_clicked() {
    for (info_field& field : info_fields_) {
        QTreeWidget* table = reinterpret_cast<QTreeWidget*>(field.get_field());
        for (int i = 0; i < table->topLevelItemCount(); ++i) {
            if (table->topLevelItem(i)->background(1) == check_color) {
                dev::delete_object(field.get_field_type(), dev::read_ids(dev::get_path_to_datatype_folder(field.get_field_type())));
                table->takeTopLevelItem(i);
            }

        }
    }
}

void dev::object_dialog_window::type_chosen() {
    short object_sybtype = reinterpret_cast<QComboBox*>(this->info_fields_[0].get_field())->currentIndex();
    change_subfields(object_sybtype);
}

void dev::object_dialog_window::table_cell_clicked(QTreeWidgetItem* item, int column) {
    if (item->background(1).color() == check_color) {
        item->setBackground(1, QBrush());
    }
    else {
       item->setBackground(1,QBrush(check_color));
    }


};
