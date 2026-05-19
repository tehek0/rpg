#pragma once
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include "QPoint"
#include <QPushButton>
#include "../header/data/datatypes.hpp"


namespace dev {
//название + поле ввода. Конструктор с типом сам решает, lineedit или combobox выбрать в зависимости от типа данных в поле
class info_field : public QObject {
    Q_OBJECT

    QLabel* label_;
    QWidget* field_;
    dev::default_types field_type_;

public:
    info_field(QString key, dev::default_types field_type, QPoint location, QWidget* parent = nullptr);
    info_field(const info_field& other);
    ~info_field();

    QLabel* get_label() {return label_;}
    QWidget* get_field() {return field_;}
};

//Само окно создание предмета для заданного типа
class object_dialog_window : public QWidget {
    Q_OBJECT

    std::vector<info_field> info_fields_;
    std::vector<info_field> info_subfields_;
    dev::datatype object_type_;
    QPushButton* save_;

public:
    explicit object_dialog_window(dev::datatype object_type);
    void change_subfields(short object_sybtype);
    ~object_dialog_window() {delete save_;}

public slots:
    void type_chosen();
    void on_save_clicked();
};

}
