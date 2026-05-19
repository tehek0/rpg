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
}


