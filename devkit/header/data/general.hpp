#pragma once
#include <QString>
#include <QLabel>
namespace dev {
//Величина отступов в json
const inline short js_indent = 2;
//Разделитель, используемый в файлах
const inline char txt_separator = '/';
//Шаблоны для записи id
const inline QString q_dangling_ptrn = "dangling_%1_ids";
const inline QString q_last_ptrn = "last_%1_id";

//Параметры размера и положения объектов на экране, общие для всех окон
constexpr short label_w = 120;
constexpr short field_w = 350;
constexpr short any_line_hight = 25;
constexpr short gap = 5;
constexpr short button_side = any_line_hight*2;
constexpr short show_table_k = 10;

namespace  ui {
    inline QLabel* inform;
}

    inline QString path_to_rpg_exe = "";
}



