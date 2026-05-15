#include "../header/object_dialogs.hpp"
#include <QDebug>

short label_w = 120;
short field_w = 300;
short any_line_hight = 25;
short gap = 5;

dev::info_field::info_field(QLabel* label, QWidget* field, QPoint location) : label_(label), field_(field) {
    label_->setGeometry(location.x(), location.y(), label_w, any_line_hight);
    field_->setGeometry(location.x()+ label_w + gap, location.y(), field_w , any_line_hight);
    label_->show();
    field_->show();
};
dev::info_field::~info_field() {
    if (w_ == nullptr) {
        delete w_;
        delete label_;
        delete field_;
    }
}

QStringList item_keys = {"name","desc","asset","max_stack_size","base_weight","base_cost","sellablle"};
QStringList weapon_keys = {"base_dmg", "damage_type", "ammo_type", "energy_cost", "requirements"};
QWidget* dev::build_dialog(dev::datatype object_type) {
    QWidget* dialog = new QWidget();
    std::vector<QWidget*> fields;
    std::vector<QLabel*> labels;
    short amount_of_line_edits;
    dialog->setBaseSize(400,800);

    if (object_type == dev::datatype::item) {
        amount_of_line_edits = item_keys.length();
    }
    else {
        qInfo() << "[WARN][buid_dialog] unhangled type of object (datatype)";
        amount_of_line_edits = 0;
    }

    int ypos = 10;
    for (short i = 0; i < amount_of_line_edits; ++i) {
        QLabel* l = new QLabel(dialog);
        l->setText(item_keys[i]);
        QLineEdit* le = new QLineEdit(dialog);
        dev::info_field whole_field = {l, le, QPoint(10, ypos)};
        labels.emplace_back(l);
        fields.emplace_back(le);
        ypos += any_line_hight + 5;
    }

    dialog->show();
    return dialog;
}
