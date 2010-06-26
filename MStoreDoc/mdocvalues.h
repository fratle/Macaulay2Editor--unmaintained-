#ifndef MSTOREVALUES_H
#define MSTOREVALUES_H
#include <QString>
#include <QRegExp>
#include <QObject>
#include <QStringList>
#include <QDebug>

//! Holds information for a Macaulay2 document node.
/*!
    This documentation node consists of a key, headline, packagename, parameters and optional parameters.
    The key, parameters and optional parameters can have html links.
*/
class MDocvalues: public QObject
{
    Q_OBJECT
public:
    //! Make a new MDocvalues
    /*!
      Constructs a MDocvalues object
     */
    MDocvalues();

    //! Set the database id.
    void setId(const int &);
    //! Set key name.
    void setKeyName(const QString &);
    //! Set Headline.
    void setHeadline(const QString &);
    //! Set parameters.
    void setParam(const QString &);
    //! Set optional parameters.
    void setOpParam(const QString &);

    //! Set html link for the key.
    /*!
      The link should point to local Macaulay html documentation..
    */
    void setKeyLinkHtml(const QString &);
    //! Set html links for the parameters
    /*!
      The link should point to local Macaulay html documentation..
    */
    void setParamHtml(const QString &);
    //! Set html links for the optional parameters.
    /*!
      The link should point to local Macaulay html documentation..
    */
    void setOpParamHtml(const QString &);
    //! Set the packagename.
    void setPackageName(const QString &);

    //! /return the database id of the node.
    const int& getId() const;
    //! \return key name
    const QString& getKeyName() const;
    //! \return key headline
    const QString& getHeadline() const;
    //! \return the parameters
    const QString& getParam() const;
    //! \return the optional parameters
    const QString& getOpParam() const;
    //! \return link to local Macaulay2 html documentation for the key.
    const QString& getKeyLinkHtml() const;
    //! \return links to local Macaulay2 html documentation for the parameters.
    const QString& getParamHtml() const;
    //! \return links to local Macaulay2 html documentation for the optional parameters.
    const QString& getOpParamHtml() const;
    //! \return package name
    const QString& getPackageName() const;
    //! \return link to local Macaulay2 html documentation for the key.
    const QString getFunctionHTML() const;

    //! add node with identical key.
    void addDocValue(MDocvalues* value);
    //! List of other nodes with identical key.
    QList<MDocvalues*> func_variants;

    //! check if node is empty.
    bool isEmpty();

private:
    int m_id;
    QString m_key;
    QString m_headline;
    QString m_param;
    QString m_opParam;
    QString m_linkHtml;
    QString m_paramHtml;
    QString m_opParamHtml;
    QString m_packageName;
    QString m_additionalFuncHtml;
    QString m_paramNoHead;
    QString m_opParamNoHead;

};
#endif // MSTOREVALUES_H
