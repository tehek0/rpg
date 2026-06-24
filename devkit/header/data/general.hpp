#pragma once
#include <QString>

namespace dev {
//Величина отступов в json
inline short js_indent = 2;
//Разделитель, используемый в txt файлах
inline char txt_separator = '/';
//Шаблоны для записи id
inline QString q_dangling_ptrn = "dangling_%1_ids";
inline QString q_last_ptrn = "last_%1_id";

//Параметры размера и положения объектов на экране, общие для всех окон
constexpr short label_w = 120;
constexpr short field_w = 350;
constexpr short any_line_hight = 25;
constexpr short gap = 5;
}



