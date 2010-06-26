#ifndef MDOCUMENTNODE_H
#define MDOCUMENTNODE_H
#include <QObject>
#include <QStringList>
#include <string>
#include <boost/xpressive/xpressive.hpp>
#include <boost/xpressive/regex_actions.hpp>

//! Class that represents Macaulay2 document node
class MDocumentNode : public QObject
{
    Q_OBJECT
public:
    explicit MDocumentNode(QObject *parent = 0);

    //! get documents node index in the database.
    const int& getDatabaseIndex() const;

    //! set documents node index in the database.
    void setDatabaseIndex(const int& index);

    //! \return document node key
    const QString& getKey() const;
    //! \return document node package.
    const QString& getPackage() const;
    //! \return document node headline
    const QString& getHeadline() const;

    //! get parameters as QString. Parameters are seperated by ",".
    QString getParameters();
    //! get optional parameters as QString. Parameters are seperated by ","
    QString getOptionalParameters();

    //! return optional parameters as QStringList.
    const QStringList& getOptionalParameterList() const;
    //! return parameters as QStringList
    const QStringList& getParameterList() const;

    //! set document node content
    /*!
        Function extracts headline for string.
    */
    void setNodeContent(const std::string&);

    //! set rawkey
    /*!
      rawkey is for example a function with parameters: func(param1, param2).
      function name and parameters are extracted and made available by the getters.
    */
    void setRawKey(const std::string&);

    //! set package name document node belongs to.
    void setPackageName(const QString& package);
    bool isValid();
signals:

public slots:

private:
    int m_databaseIndex;

    //! get headline from node content
    void m_parseNode();
    //! get key, headline and parameters from raw key
    void m_parseKey();

    std::string m_node;
    std::string m_key;
    std::string m_raw_key;
    std::string m_headline;

    QString mq_key;
    QString mq_headline;
    QString mq_package;

    QStringList mq_parameters;
    QStringList m_parameter_list;
    QStringList m_opParameter_list;

};

#endif // MDOCUMENTNODE_H
