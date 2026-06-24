#pragma once
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include "QPoint"
#include <QPushButton>
#include "../header/data/datatypes.hpp"
#include "info_fields.hpp"

namespace dev {
//Окно создание предмета для заданного типа
class object_dialog_window : public QWidget {
    Q_OBJECT

    std::vector<info_field> info_fields_;
    std::vector<info_field> info_subfields_;
    dev::datatype object_type_;
    QPushButton* save_;
    QPushButton* reset_;
    int last_y_pos;
    int ypos = 10;

public:
    explicit object_dialog_window(dev::datatype object_type);
    ~object_dialog_window();

    std::vector<info_field> get_info_fields() {return info_fields_;}

    void delete_subfields();
    void add_subfields(short object_sybtype);
    void change_subfields(short object_sybtype);

public slots:
    void type_chosen();
    void on_save_clicked();
    void on_reset_clicked();
};
}
