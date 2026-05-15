#include "../header/object_dialogs.hpp"
#include <QDebug>

short label_w = 120;
short field_w = 300;
short any_line_hight = 25;
short gap = 5;

dev::info_field::info_field(QLabel* label, QWidget* field, QPoint location, QWidget* w) : w_(w), label_(label), field_(field) {
    label_->setParent(w);
    field_->setParent(w);
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
QWidget* dev::build_dialog(dev::datatype object_type) {
    QWidget* dialog = new QWidget();
    dialog->setBaseSize(400,800);
    int ypos = 10;
    for (short i = 0; i < 11; ++i) {
        dev::info_field whole_field = {new QLabel(), new QLineEdit(), QPoint(10,ypos), dialog};
        ypos += any_line_hight + 5;
    }
    qInfo() << dialog->children();
    dialog->show();
    return dialog;
}
