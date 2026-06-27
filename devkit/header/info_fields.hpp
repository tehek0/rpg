#pragma once
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include "QPoint"
#include <QPushButton>
#include <QTreeWidget>
#include "../header/data/datatypes.hpp"
#include "create_object.hpp"
class object_dialog_window;

namespace dev {
const inline QColor check_color = QColor(96, 230, 156);

//Содержит название + поле ввода. Конструктор с типом сам решает, lineedit или combobox выбрать в зависимости от типа данных в поле
//additional add - плюсик для поддержки массивов
class info_field : public QObject {
    Q_OBJECT

    QLabel* label_;
    QWidget* field_;
    dev::datatype field_type_;

public:
    info_field(QString key, dev::datatype field_type, QPoint location, QWidget* parent = nullptr);
    info_field(const info_field& other);
    ~info_field();

    QLabel* get_label() const {return label_;}
    QWidget* get_field() const {return field_;}
    dev::datatype get_field_type() const {return field_type_;}

    //Функуция для бережного использования. (только для структурных типов, где field - QTreeWidget)
    int calculate_table_hight();

    void clear_info_field();
    //Проверяет типы данных и заполняет combo box соответствующими ему значениями
    void fill_combo_box_data(QComboBox* field, dev::datatype type);
    //Аналогично для treewidget
    void fill_qtable_data(QTreeWidget* field, dev::datatype type);
};

//Считывает данные с info_field и записывает в шаблонном виде object_data(из create_object) (для экспорта в json). В процессе конвертаций поэтапно обрабатываются массивы id
void read_from_infoField_to_objectData(const dev::info_field& data, dev::object_data& object);

}
