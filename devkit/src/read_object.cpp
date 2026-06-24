#include "../header/read_object.hpp"
#include <QDebug>
#include <QDirIterator>

QStringList dev::lines_present(QString path){
    QStringList a;
    QDirIterator itFiles(path, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (itFiles.hasNext()) {
        QFile file(itFiles.next());
        file.open(QIODevice::ReadOnly);
        QString line = file.fileName() + file.readAll();
        if (!line.isEmpty()) {
          a.append(line.remove('\r').remove('\n').simplified());
        }
    }
    return a;
}
