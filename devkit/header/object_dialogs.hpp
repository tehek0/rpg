#pragma once
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include "QPoint"
#include <QPushButton>
#include "../header/data/datatypes.hpp"
#include "create_object.hpp"


namespace dev {
//Содержит название + поле ввода. Конструктор с типом сам решает, lineedit или combobox выбрать в зависимости от типа данных в поле
class info_field : public QObject {
    Q_OBJECT

    QLabel* label_;
    QWidget* field_;
    dev::default_types field_type_;

public:
    info_field(QString key, dev::default_types field_type, QPoint location, QWidget* parent = nullptr);
    info_field(const info_field& other);
    ~info_field();

    QLabel* get_label() const {return label_;}
    QWidget* get_field() const {return field_;}
    dev::default_types get_field_type() const {return field_type_;}

    void clear_info_field();
};

//Считывает данные с шnfo_field и записывает в шаблонном виде object_data(из create_object) (для экспорта в json)
void read_from_infoField_to_objectData(const dev::info_field& data, dev::object_data& object);

//Само окно создание предмета для заданного типа
class object_dialog_window : public QWidget {
    Q_OBJECT

    std::vector<info_field> info_fields_;
    std::vector<info_field> info_subfields_;
    dev::datatype object_type_;
    QPushButton* save_;
    QPushButton* reset_;

public:
    explicit object_dialog_window(dev::datatype object_type);
    void change_subfields(short object_sybtype);
    ~object_dialog_window();

public slots:
    void type_chosen();
    void on_save_clicked();
    void on_reset_clicked();
};

}
