#ifndef MUPDATEDBTHREAD_H
#define MUPDATEDBTHREAD_H
#include <QThread>
#include <QtSql>
#include "mdocumentnode.h"

//! Generate html links for document nodes, then update database.
/*!
    Starts a Macaulay2 process which generates html for all keys in pkg_docnodes.
    These links are then inserted into the existing database.
*/
class MUpdateDBThread: public QThread
{
public:
    MUpdateDBThread(QMap<QString, QList<MDocumentNode*> > *pkg_docnodes);
    ~MUpdateDBThread();
    void run();
private:
    //! map containing package name as key and documentation nodes for that package.
    QMap<QString, QList<MDocumentNode*> > *m_pkg_docnodes;
    //! map of functions,types with their html link.
    QMap<QString, QString> m_func_html;
    //! generate a QStringList with unique keys
    QStringList uniqueTypesFunctions(QList<MDocumentNode*> docnodes);
    //! generate html links.
    void generateHTML();
    //! update database with links.
    void updateDatabase();
    //! update database with links.
    /*!
        \param keyId index in database to update.
        \param keyLink html for key.
        \param paramLink html for parameters.
        \param opParamLink html for optional parameters.
    */
    void updateDbWithHTML(const int& keyId, const QString &keyLink, const QString &paramLink, const QString &opParamLink);
    //! database connection.
    QSqlDatabase db;
};

#endif // MUPDATEDBTHREAD_H
