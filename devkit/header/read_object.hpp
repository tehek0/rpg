#pragma once
#include <QStringList>

namespace dev {
//читает файлы в данной папке,составляет из данных строки-представления. Даёт фрагментарное понимание содержимого
//Нужно для отображния кастомных типов данных в девките. Не собирает объекты
QStringList lines_present(const QString& path);
//Читает айди всех объектов в данной директории
QStringList read_ids(const QString& path);

}
