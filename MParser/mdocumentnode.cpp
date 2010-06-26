#include "mdocumentnode.h"
#include <iostream>
#include <QDebug>

using namespace boost::xpressive;

MDocumentNode::MDocumentNode(QObject *parent) :
        QObject(parent)
{
}

const QString& MDocumentNode::getKey() const {
    return mq_key;
}

const QString& MDocumentNode::getHeadline() const {
    return mq_headline;
}

const QString& MDocumentNode::getPackage() const {
    return mq_package;
}

QString MDocumentNode::getParameters() {
    return m_parameter_list.join(",");
}

QString MDocumentNode::getOptionalParameters() {
    return m_opParameter_list.join(",");
}

const QStringList& MDocumentNode::getOptionalParameterList() const {
    return m_opParameter_list;
}

const QStringList& MDocumentNode::getParameterList() const {
    return m_parameter_list;
}

bool MDocumentNode::isValid() {
    if (mq_key.isEmpty()) {
        return false;
    }
    if (mq_key.size() < 2) {
        return false;
    }
    return true;
}

void MDocumentNode::setNodeContent(const std::string& node) {
    m_node = node;
    m_parseNode();
}

void MDocumentNode::setRawKey(const std::string& rawkey) {
    m_raw_key = rawkey;
    m_parseKey();
}



void MDocumentNode::setPackageName(const QString &package) {
    mq_package = package;
}

void MDocumentNode::setDatabaseIndex(const int &index) {
    m_databaseIndex = index;
}

const int& MDocumentNode::getDatabaseIndex() const {
    return m_databaseIndex;
}

void MDocumentNode::m_parseNode() {
    sregex key_exp = "=>" >> -*_s;
    mark_tag headline(1), headline_value(2);
    sregex re_headline = -*_s >> (headline=("Headline")) >> -*_s >> key_exp >> '"' >> (headline_value=-*_) >> '"';

    smatch what;
    if (regex_search(m_node, what, re_headline)) {
        m_headline = what[headline_value];
        mq_headline = m_headline.c_str();
    }
}

void MDocumentNode::m_parseKey() {
    sregex word = (+_w >> !as_xpr("'"));
    sregex funcname = bos >> (s1=(word)) >> -*_s >> eos;
    smatch what, funcwhat;


    if (regex_search(m_raw_key, what, funcname)) {
        m_key = what[1];
        mq_key = m_key.c_str();
    }

    std::string funcdefinition;
    sregex funcdef = (s1=word) >> -*_s >> '(' >> (s2=(+_ >> ')'));
    if (regex_search(m_raw_key, funcwhat, funcdef)) {
        m_key = funcwhat[1];
        mq_key = m_key.c_str();
        funcdefinition = funcwhat[2];
    }

    if (funcdefinition.empty()) {
        return;
    }
    if (mq_key.isEmpty()) {
        return;
    }


    sregex opParam = (s1=word) >> -*_s >> "=>" >> -*_s;
    sregex lineend = (as_xpr(')')|',');
    sregex params = (s1=word) >> -*_s >> ~before("=>") >> -*_s >> lineend;

    sregex_iterator cur( funcdefinition.begin(), funcdefinition.end(), params);
    sregex_iterator end;

    for ( ; cur != end; ++cur )
    {
        smatch const &sowhat = *cur;
        std::string param_str = sowhat[1];
        QString q_param = param_str.c_str();
        m_parameter_list.append(q_param);
    }

    sregex_iterator opcur( funcdefinition.begin(), funcdefinition.end(), opParam);
    sregex_iterator opend;
    for ( ; opcur != opend; ++opcur )
    {
        smatch const &sowhat = *opcur;
        std::string param_str = sowhat[1];
        QString q_param = param_str.c_str();
        m_opParameter_list.append(q_param);
    }
}



