#include "appdata.h"

#include <QCoreApplication>
#include <QDir>

namespace AppData {
const QList<QString> genres = {
    "Action",
    "Adventure",
    "Animation",
    "Comedy",
    "Crime",
    "Documentary",
    "Drama",
    "Fantasy",
    "Historical",
    "Horror",
    "Musical",
    "Mystery"
    "Noir",
    "Sci-Fi",
    "Romance",
    "Superhero",
    "Thriller",
    "War",
    "Western"
};

const QList<QString> fsk = {
    "FSK 0 (freigegeben ohne Altersbeschränkung)",
    "FSK 6 (freigegeben ab 6 Jahren)",
    "FSK 12 (freigegeben ab 12 Jahren)",
    "FSK 16 (freigegeben ab 16 Jahren)",
    "FSK 18 (keine Jugendfreigabe)"
};

QString pathToDb() {
    QDir binDir(QCoreApplication::applicationDirPath());
    QString dbPath = binDir.absoluteFilePath("db/kino.db");
    int count = 0;
    while ( !QFileInfo::exists(dbPath) ) {
        binDir.cdUp();
        dbPath = binDir.absoluteFilePath("db/kino.db");
        count++;
        if (count > 5) break;
    }

    return dbPath;
}

}
