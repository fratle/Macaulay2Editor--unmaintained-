#ifndef MPARSER_H
#define MPARSER_H

#include <QObject>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <QSet>
#include <QMap>
#include "mdocumentnode.h"
#include "mupdatedbthread.h"
#include <boost/xpressive/xpressive_static.hpp>

//! Parser for Macaulay2 document nodes.
/*!
    The parser locates Macaulay2's rawdocumentation, usually in /usr/lib/Macaulay2.
    The parser then finds all keys and node content and creates a MDocumentNode for each key, then stores each documentnode in a database (~/.M2E/Macaulay2EditorDB.db)
    Finally it starts the MUpdateDBThread thread which generates html for all keys.
*/
class MParser : public QObject
{
    Q_OBJECT
public:
    explicit MParser(QObject *parent = 0);
    //! begin parsing.
    void parse();
signals:
    //! emitted when MUpdateDBThread thread is finished.
    void finishedHtmlLinks();
private slots:
    void htmlFinished();
public slots:

private:
    //! a list containing all found Macaulay2 documentation nodes
    QList<MDocumentNode*> *docnodes;
    //! map containing package name as key and documentation nodes for that package
    QMap<QString, QList<MDocumentNode*> > *pkg_docnodes;
    //! all packages found
    QStringList packages;
    //! \return QMap of all packages as key and Macaulay2 raw documentation files as value.
    QMap<QString, QString> getAllFiles(const QString& path);
    //! open Macaulay2 raw documentation files and get all documentation nodes
    /*!
        populates pkg_docnodes and docnodes.
    */
    void searchFiles();
    //! save documentation nodes to a database.
    /*!
        database is sqlite3 and the database is stored in user home folder under .M2E/Macaulay2EditorDB.db
    */
    void commitToDatabase();
};

#endif // MPARSER_H
