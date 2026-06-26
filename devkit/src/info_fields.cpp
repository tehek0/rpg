#include "../header/info_fields.hpp"
#include <QDebug>
#include <QComboBox>
#include <QPushButton>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QRegularExpressionValidator>
#include "../header/inc/json.hpp"
#include "../header/read_object.hpp"
#include "../header/data/general.hpp"
using js = nlohmann::ordered_json;


//INFO_FIELD
void dev::info_field::fill_combo_box_data(QComboBox* field, dev::datatype type) {
    if (type == dev::datatype::erased) {
        for (int i = dev::datatype::location; i < dev::datatype::item; ++i) {
            field->addItem(QString::fromStdString(datatypes_to_string[i]));
        }
        connect(field, SIGNAL(currentIndexChanged(int)), field->parent(), SLOT(delete_type_chosen()));
    }
    if (type == dev::datatype::boolean){
        field->addItems({"false", "true"});
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
    else if (!dev::is_type_struct(type)){
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
        case dev::datatype::armor_slot:
            field->addItems({"head", "body", "legs"});
            break;
        case dev::datatype::use_effect:
            field->addItems({"none", "change_health", "change_energy"});
            break;
        case dev::datatype::equipment_bonus:
            field->addItems({"none", "change_armor", "change_health", "change_energy", "change_strength", "change_agility",
                             "change_endurance", "change_intellegence", "change_luck", "change_guns", "change_big_guns",
                             "change_unarmed", "change_science", "change_spech", "change_barter", "change_survival"});
            break;
        default: break;
        }

    }
};

void dev::info_field::fill_qtable_data(QTreeWidget* field, dev::datatype type) {
    QString path = get_path_to_datatype_folder(type);

    field->setColumnCount(2);
    field->setColumnWidth(0, button_side/2);
    field->setColumnWidth(1, field_w);
    field->setHeaderHidden(true);

    auto lines = dev::lines_present(path);
    auto ids = dev::read_ids(path);
    for (size_t i = 0; i < ids.length(); ++i) {
        QStringList field_line = {ids[i],lines[i]};
        QTreeWidgetItem* item = new QTreeWidgetItem(field_line);
        field->addTopLevelItem(item);
    }
    field->resize(field_w + button_side, (field->topLevelItemCount()%5)*any_line_hight);
    connect(field, SIGNAL(itemClicked(QTreeWidgetItem*,int)), field->parent(), SLOT(table_cell_clicked(QTreeWidgetItem*,int)));
}

const QRegularExpression num_e("-{0,1}\\d{0,}");
const QRegularExpression unum_e("[1-9]\\d{0,}");
const QRegularExpression double_e("[1-9]\\d{0,}.\\d{1,}");
const QValidator* num_val = new QRegularExpressionValidator(num_e);
const QValidator* unum_val = new QRegularExpressionValidator(unum_e);
const QValidator* double_val = new QRegularExpressionValidator(double_e);
dev::info_field::info_field(QString key, dev::datatype field_type, QPoint location, QWidget* parent) : field_type_(field_type) {
    this->setParent(parent);
    label_ = new QLabel(parent);
    label_->setText(key);
    label_->setGeometry(location.x(), location.y(), label_w, any_line_hight);

    if (dev::is_type_linear(field_type)) {
        QLineEdit* temp = new QLineEdit(parent);

        if (field_type == dev::datatype::double_t) {
            temp->setValidator(double_val);
        }
        else if (dev::is_type_unsigned(field_type)) {
            temp->setValidator(unum_val);
        }
        else if (!(field_type == dev::datatype::string || field_type == dev::datatype::qstring)) {
            temp->setValidator(num_val);
        }

        field_ = temp;
        field_->setGeometry(location.x()+ label_w + gap, location.y(), field_w , any_line_hight);

    }
    else if (dev::is_type_struct(field_type)) {
        QTreeWidget* field = new QTreeWidget(parent);
        fill_qtable_data(field,field_type);
        field_ = field;
        field_->move(location.x()+ label_w + gap, location.y());
    }
    else {
        QComboBox* field = new QComboBox(parent);
        fill_combo_box_data(field, field_type);
        field_ = field;
        field_->setGeometry(location.x()+ label_w + gap, location.y(), field_w , any_line_hight);
    }

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
    else if (is_type_struct(field_type_)) {
        QTreeWidget* table = dynamic_cast<QTreeWidget*>(field_);
        for (int i = 0; i < table->topLevelItemCount(); ++i) {
            table->topLevelItem(i)->setBackground(1, QBrush());
        }
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
    else if (is_type_struct(current_type)) {
        QTreeWidget* field = dynamic_cast<QTreeWidget*>(data.get_field());
        for (size_t i = 0; i < field->topLevelItemCount(); ++i) {
            QTreeWidgetItem* item = field->topLevelItem(i);
            if (item->background(1) == check_color) {
                current_value += field->topLevelItem(i)->text(0);
                current_value += txt_separator;
            }
        }
    }
    else {
        current_value = QString::number(dynamic_cast<QComboBox*>(data.get_field())->currentIndex());
    }

    object.keys_.append(current_key);
    object.values_.append(current_value);
    object.types_.emplace_back(current_type);
}
