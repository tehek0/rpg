#include "../header/read_object.hpp"
#include <QDebug>
#include <QDirIterator>
#include <QRegularExpressionMatch>
#include "../../header/id_support.hpp"


QString dev::get_path_to_datatype_folder(dev::datatype type) {
    QString path = "../../../objects/";
    if (is_type_component(type)) {
        path += "components/";
    }
    switch(type) {
    case dev::datatype::item_requirement: path += "item_requirement"; break;
    case dev::datatype::requirement: path += "requirement"; break;
    case dev::datatype::armor_bonus: path += "armor_bonus"; break;
    case dev::datatype::item: path += "item"; break;
    case dev::datatype::on_use: path += "on_use"; break;
    default: break;
    }
    return path;
}

bool dev::is_directory_empty(QString path) {
    QDir dir(path);
    if (dir.isEmpty()) {
        return true;
    }
    return false;
}

QStringList dev::lines_present(const QString& path){
    QStringList lines;
    QDirIterator itFiles(path, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (itFiles.hasNext()) {
        QFile file(itFiles.next());
        file.open(QIODevice::ReadOnly);
        QString line = file.readAll();
        if (!line.isEmpty()) {
          lines.append(line.remove('\r').remove('\n').simplified());
        }
    }
    return lines;
}

const QRegularExpression cut_id_out_of_file_name_e("[1-9][0-9]{0,}");
QStringList dev::read_ids(const QString& path) {
    QStringList ids;
    QDirIterator itFiles(path, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (itFiles.hasNext()) {
        QFile file(itFiles.next());
        file.open(QIODevice::ReadOnly);
        QString id = cut_id_out_of_file_name_e.match(file.fileName()).captured();
        if (!id.isEmpty()) {
            ids.append(id);
        }
    }

    return ids;
}

void dev::delete_object(dev::datatype object_type, QStringList ids) {
    QDirIterator itFiles(get_path_to_datatype_folder(object_type), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (itFiles.hasNext()) {
        QFile file(itFiles.next());
        QString file_id = cut_id_out_of_file_name_e.match(file.fileName()).captured();
        for (QString id : ids) {
            if (id == file_id) {
                dev::remove_id(object_type, id.toULongLong());
                file.remove();
            }
        }

    }
}
