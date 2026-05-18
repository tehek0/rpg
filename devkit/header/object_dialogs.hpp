#pragma once
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include "QPoint"
#include "../header/create_object.hpp"
#include "../header/datatypes.hpp"

namespace dev {
class info_field {
    QWidget* w_;
public:
    QLabel* label_;
    QWidget* field_;

    info_field(QLabel* label, QWidget* field, QPoint location);
    virtual ~info_field();
};

//Выбор qlineedit или выпадающего списка как field для info_field
QWidget* create_appropriate_field(dev::default_types object_type, QWidget* parent);

QWidget* build_dialog_window(dev::datatype object_type);
}
