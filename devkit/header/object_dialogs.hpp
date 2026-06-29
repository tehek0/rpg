#pragma once
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include "QPoint"
#include <QPushButton>
#include "../header/data/datatypes.hpp"
#include "info_fields.hpp"

namespace dev {
//Окно создание предмета для заданного типа. В общем то заложена универсальная сборочная программа для полей ввода разного типа
class object_dialog_window : public QWidget {
    Q_OBJECT

    std::vector<info_field> info_fields_;
    std::vector<info_field> info_subfields_;
    dev::datatype object_type_;
    QPushButton* save_;
    QPushButton* reset_;
    QPushButton* delete_;
protected:
    int last_y_pos;
    int ypos = 10;

public:
    object_dialog_window(); //не задуман к использованию
    explicit object_dialog_window(dev::datatype object_type);
    ~object_dialog_window();

    std::vector<info_field> get_info_fields() {return info_fields_;}

    void resize_dependent_on_fields();

    void delete_subfields();
    void add_subfields(short object_sybtype);
    void change_subfields(short object_sybtype);

    void add_save_button();
    void add_delete_button();
public slots:
    void on_save_clicked();
    void on_reset_clicked();
    void on_delete_clicked();

    void type_chosen();
    void table_cell_clicked(QTreeWidgetItem* item, int column);
};

class help_window : public object_dialog_window {
private:
    std::vector<QLabel*> labels_;
public:
    help_window();
};
}

