#include "mparser.h"
#include <iostream>
#include <QtSql>
#include <gdbm.h>

using namespace boost::xpressive;

MParser::MParser(QObject *parent) :
        QObject(parent)
{
    docnodes = new QList<MDocumentNode*>();
    pkg_docnodes = new QMap<QString, QList<MDocumentNode*> >();
}

void MParser::htmlFinished() {
    emit finishedHtmlLinks();
}


void MParser::parse() {
    searchFiles();
    commitToDatabase();
}

QMap<QString, QString> MParser::getAllFiles(const QString& _path) {
    QSettings settings;
    QString mDirectory = settings.value("MainWindow/Directory").toString();
    qDebug() << mDirectory;
    QMap<QString, QString> path_package;
    QDirIterator it(mDirectory, QStringList() << "rawdocumentation*.db" , QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
        QFileInfo fileinfo = it.fileInfo();
        QDir dir = fileinfo.dir();
        dir.cdUp();
        qDebug() << dir.dirName();
        qDebug() << fileinfo.absoluteFilePath();
        packages.append(dir.dirName());
        path_package.insert(fileinfo.absoluteFilePath(), dir.dirName());
    }
    return path_package;
}

void MParser::searchFiles() {
    QMap<QString, QString> dbfiles = getAllFiles("");
    QMapIterator<QString, QString> i(dbfiles);
    while (i.hasNext()) {
        QList<MDocumentNode*> tmpnodes;
        i.next();
        QString filename = i.key();
        QString package = i.value();
        GDBM_FILE dbf;
        dbf = gdbm_open(filename.toLocal8Bit().data(), 0, GDBM_READER, 0644, 0);
        if (!dbf) {
            qDebug() << "error opening file: " << filename.toLocal8Bit().data();
            break;
        }

        datum key;
        key = gdbm_firstkey ( dbf );
        while ( key.dptr ) {
            MDocumentNode *a = new MDocumentNode(0);

            datum nextkey;
            if (key.dsize > 0) {
                QString qkey = QByteArray(key.dptr, key.dsize);
                a->setRawKey(qkey.toStdString());
            }

            datum data;
            data = gdbm_fetch(dbf, key);
            if (data.dptr) {
                QString content = QByteArray(data.dptr, data.dsize);
                a->setNodeContent(content.toStdString());
                free(data.dptr);
            }

            a->setPackageName(package);
            if (a->isValid()) {
                tmpnodes.append(a);
            }
            nextkey = gdbm_nextkey( dbf, key );
            free(key.dptr);
            key = nextkey;
        }
        docnodes->append(tmpnodes);
        pkg_docnodes->insert(package, tmpnodes);
        gdbm_close(dbf);
    }
    qDebug() << "SIZE: " << docnodes->size();
}

void MParser::commitToDatabase() {
    packages.removeDuplicates();
    QString databasePath =  QDir::homePath() + "/.M2E/Macaulay2EditorDB.db";
    QFile databasefile(databasePath);

    if (databasefile.exists()) {
        databasefile.remove();
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(databasePath);
    bool ok = db.open();
    if (ok) {
        qDebug() << "OPEN";
    }

    QSqlQuery query;
    query.exec("BEGIN");
    QString createPackageTab = "CREATE TABLE \"packages\" ("
                               "\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                               "\"name\" TEXT UNIQUE NOT NULL)";
    query.exec(createPackageTab);


    QString createFunctionTab = "CREATE TABLE \"functions\" ("
                                "\"id\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                                "\"name\" TEXT NOT NULL,"
                                "\"param\" TEXT,"
                                "\"opParam\" TEXT,"
                                "\"headline\" TEXT,"
                                "\"link\" TEXT,"
                                "\"paramHtml\" TEXT,"
                                "\"opParamHtml\" TEXT,"
                                "\"packageId\" INTEGER NOT NULL,"
                                "FOREIGN KEY (packageId) REFERENCES packages(id))";
    query.exec(createFunctionTab);

    QString Key;
    QString param;
    QString opParam;
    QString Headline;
    QString FileName;
    QString Link;
    QString paramHtml;
    QString opParamHtml;

    QSqlTableModel model;
    QSqlRecord record;

    QMap<QString, QString> values;

    QString package;

    int pkg_id;

    for (int i =0;i<packages.count();i++) {
        package = packages.at(i);
        query.prepare("INSERT INTO packages (name)"
                      "VALUES (:name)");
        query.bindValue(":name", package);
        query.exec();
    }

    for (int i = 0; i<docnodes->count();i++) {
        MDocumentNode *node = docnodes->at(i);

        node->setDatabaseIndex(i+1);

        Key = node->getKey();
        param = node->getParameters();
        opParam = node->getOptionalParameters();
        Headline = node->getHeadline();
        pkg_id = packages.indexOf(node->getPackage())+1;

        query.prepare("INSERT INTO functions (name, param, opParam, headline, link, paramHtml, opParamHtml, packageId)"
                      "VALUES(:name, :param, :opParam, :headline, :link, :paramHtml, :opParamHtml, :packageId)");

        query.bindValue(":name", Key);
        query.bindValue(":param", param);
        query.bindValue(":opParam", opParam);
        query.bindValue(":headline", Headline);
        query.bindValue(":link", Key);
        query.bindValue(":paramHtml", param);
        query.bindValue(":opParamHtml", opParam);
        query.bindValue(":packageId", pkg_id);

        query.exec();
    }
    query.exec("COMMIT");
    db.close();
    MUpdateDBThread *thread = new MUpdateDBThread(pkg_docnodes);
    connect(thread, SIGNAL(finished()), this, SLOT(htmlFinished()));
    thread->start();
}
