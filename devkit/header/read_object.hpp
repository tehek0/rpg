#pragma once
#include <QStringList>
#include "data/datatypes.hpp"

namespace dev {
QString get_path_to_datatype_folder(dev::datatype type);
bool is_directory_empty(const QString& path);
bool is_that_true(const std::string& path, std::string json_parameter_key);
//читает файлы в данной папке,составляет из данных строки-представления. Даёт фрагментарное понимание содержимого
//Нужно для отображния кастомных типов данных в девките. Не собирает объекты
QStringList lines_present(const QString& path);
//Читает айди всех объектов в данной директории
QStringList read_ids(const QString& path);

//Удаляет файл и чистит id
void delete_object(dev::datatype object_type, QStringList ids);
}
