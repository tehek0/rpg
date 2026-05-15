#pragma once
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include "QPoint"
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
QWidget* build_dialog(dev::datatype object_type);
}
