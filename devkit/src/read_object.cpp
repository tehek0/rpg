#include "../header/read_object.hpp"
#include <QDebug>
#include <QDirIterator>
#include <QRegularExpressionMatch>
#include "../../header/id_support.hpp"
#include "header/data/general.hpp"
#include "../header/inc/json.hpp"
#include "fstream"
using js = nlohmann::ordered_json;

QString dev::get_path_to_datatype_folder(dev::datatype type) {
    QString path = dev::path_to_rpg_exe;
    std::filesystem::path root = std::filesystem::path(path.toStdString());
    root /= "objects";
    if (is_type_component(type)) {
        root /= "components";
    }
    switch(type) {
    case dev::datatype::item_requirement: root /= "item_requirement"; break;
    case dev::datatype::requirement: root /= "requirement"; break;
    case dev::datatype::armor_bonus: root /= "armor_bonus"; break;
    case dev::datatype::item: root /= "item"; break;
    case dev::datatype::on_use: root /= "on_use"; break;
    default: break;
    }
    return QString::fromStdString(root.string());
}

bool dev::is_directory_empty(const QString& path) {
    QDir dir(path);
    if (dir.isEmpty()) {
        return true;
    }
    return false;
}

bool dev::is_that_true(const std::string& path, std::string json_parameter_key) {
    std::ifstream file(path);
    js file_j;
    try {
        file_j = js::parse(file);
    }
    catch(...) {
        qInfo() << "[FATAL][is_that_true] cannot parse";
    }

    bool check;

    file_j[json_parameter_key].get_to(check);
    qInfo() << check;
    return check;
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
    ui::inform->setText("Удалено");
}
