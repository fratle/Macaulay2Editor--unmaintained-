#ifndef MSTOREDOC_H
#define MSTOREDOC_H
#include <QString>
#include <QObject>
#include <QStringList>
#include <QtSql>
#include <QSqlDatabase>
#include "mdocvalues.h"
#include <QTest>

//! This class is used to read the database values.
class MStoredoc : public QObject
{
    Q_OBJECT

public:
    MStoredoc();

    //! Adds a new package to the completion list containing MDocvalues objects.
    /*!
      \param QString & is the name of the package.

      This function reads the rows and columns in the local database and puts the information in a new MDocvalues
      object and adds this to a QList. If we have rows where name occur more than once we get a new QList with
      MDocvalues objects inside the QList.
      */
    void addPackage(const QString &);

    //! \return MDocvalues object for a invoked completion text.
    /*!
      \param QString & is a string which will be compared to the keynames inside a QList of MDocvalues objects.

      If the string matches any keyname, completerNameHelp will return this object.
      */
    MDocvalues* completerNameHelp(const QString&);
    //! \return MDocvalues object for a matched key name within the completion list.
    /*!
      \param QString & is the string which will be compared with the keynames inside the completion list.
      */
    MDocvalues* getByName(const QString &);
    //! \retun MDocvalues object for a matched id within the completion list.
    /*!
      \param QModelIndex & is the row's index in the completion list.
      */
    MDocvalues* getById(const QModelIndex &);

    //! \return true if the given keyname exists in the completion list.
    bool exists(const QString &name);

    //! QList containing MDocvalues objects which is the list used by auto completion.
    QList<MDocvalues*> *currentPackages;

private:
    //! QList containing MDocvalues objects, but does not take duplicate key names.
    QList<MDocvalues*> m_DocValues;
    //! MDocvalues object used when function name occur more than once in the database.
    MDocvalues *currentdocObject;
    //! A StringList containing which packages is loaded in the active view.
    QStringList loadedPackages;

};
#endif // MSTOREDOC_H
