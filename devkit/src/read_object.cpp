#include "../header/read_object.hpp"
#include <QDebug>
#include <QDirIterator>
#include <QRegularExpressionMatch>
#include "../header/data/general.hpp"

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

const QRegularExpression cut_id_out_of_file_name("[1-9][0-9]{0,}");
QStringList dev::read_ids(const QString& path) {
    QStringList ids;
    QDirIterator itFiles(path, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (itFiles.hasNext()) {
        QFile file(itFiles.next());
        file.open(QIODevice::ReadOnly);
        QString id = cut_id_out_of_file_name.match(file.fileName()).captured();
        if (!id.isEmpty()) {
            ids.append(id);
        }
    }

    return ids;
}
