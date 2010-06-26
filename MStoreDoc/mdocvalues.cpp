#include "mdocvalues.h"

MDocvalues::MDocvalues() : QObject(0) {
}

bool MDocvalues::isEmpty() {
    if (m_key.isEmpty()) {
        return true;
    }
    return false;
}
/*
 * Set id
 */
void MDocvalues::setId(const int &id) {
    this->m_id = id;
}
/*
 * Get id
 */
const int& MDocvalues::getId() const {
    return this->m_id;
}
/*
 * Set Keyname
 */
void MDocvalues::setKeyName(const QString &key) {
    this->m_key = key;
}
/*
 * Get Keyname
 */
const QString& MDocvalues::getKeyName() const {
    return this->m_key;
}
/*
 * Set Headline
 */
void MDocvalues::setHeadline(const QString &headline) {
    this->m_headline = headline;
}
/*
 * Get Headline
 */
const QString& MDocvalues::getHeadline() const {
    return this->m_headline;
}
/*
 * Set keyname link Html
 */
void MDocvalues::setKeyLinkHtml(const QString &linkHtml) {
    this->m_linkHtml = linkHtml;
}
/*
 * Get keyname link Html
 */
const QString& MDocvalues::getKeyLinkHtml() const {
    return this->m_linkHtml;
}
/*
 * Set Packagename
 */
void MDocvalues::setPackageName(const QString &packageName) {
    this->m_packageName = packageName;
}
/*
 * Get Packagename
 */
const QString& MDocvalues::getPackageName() const {
    return this->m_packageName;
}
/*
 * Set Parameters
 */
void MDocvalues::setParam(const QString &param) {
    //QStringList tempParam = param.split(";");
    this->m_param = param;
}
/*
 * Get Parameters
 */
const QString& MDocvalues::getParam() const {
    return this->m_param;
}
/*
 * Set Optional Parameters
 */
void MDocvalues::setOpParam(const QString &opParam) {
    //QStringList tempOpParam = opParam.split(";");
    this->m_opParam = opParam;
}
/*
 * Get Optional Parameters
 */
const QString& MDocvalues::getOpParam() const {
    return this->m_opParam;
}
/*
 * Set Html for Parameters
 */
void MDocvalues::setParamHtml(const QString &paramHtml) {
    //QStringList tempParamHtml = paramHtml.split(";");
    this->m_paramHtml = paramHtml;
}
/*
 * Get Html for Parameters
 */
const QString& MDocvalues::getParamHtml() const {
    return this->m_paramHtml;
}
/*
 * Set Html for Optional Parameters
 */
void MDocvalues::setOpParamHtml(const QString &opParamHtml) {
    //QStringList tempOpParamHtml = opParamHtml.split(";");
    this->m_opParamHtml = opParamHtml;
}
/*
 * Get Html for Optional Parameters
 */
const QString& MDocvalues::getOpParamHtml() const {
    return this->m_opParamHtml;
}
/*
 * Adds a MDocvalues object which contains additional headline, parameters and optional parameters.
 */
void MDocvalues::addDocValue(MDocvalues *value) {
    func_variants.append(value);

    if (!value->getHeadline().isEmpty()) {
        m_additionalFuncHtml += "<b>" + value->getHeadline() +"."+ "</b><br />";
    }
    if (!value->getHeadline().isEmpty()) {
        if (!value->getParamHtml().isEmpty()) {
            m_additionalFuncHtml += "Parameters:<br />";
            m_additionalFuncHtml += "(" + value->getParamHtml() + ")";
            m_additionalFuncHtml += "<br />";
        }
    }
    else {
        if (!value->getParamHtml().isEmpty()) {
            m_paramNoHead += "(" + value->getParamHtml() + ")";
            m_paramNoHead += "<br />";
        }
    }
    if (!value->getHeadline().isEmpty()) {
        if (!value->getOpParamHtml().isEmpty()) {
            m_additionalFuncHtml += "Optional parameters:<br />";
            m_additionalFuncHtml += value->getOpParamHtml();
            m_additionalFuncHtml += "<br />";
        }
    }
    else {
        if (!value->getOpParamHtml().isEmpty()) {
            m_opParamNoHead += "(" + value->getOpParamHtml() + ")";
            m_opParamNoHead += "<br />";
        }
    }
    if (!value->getHeadline().isEmpty() && (!value->getParamHtml().isEmpty() || !value->getOpParamHtml().isEmpty())) {
        m_additionalFuncHtml += "<hr />";
    }
}
/*
 * Returns a string which has a html structure and contains headline, parameters, optional parameters and links
 * to the documentation. getFunctionHTML() is used both in MAutocomplete.cpp for the ExpandingWidget and in
 * mautocompletewidget.cpp.
 */
const QString MDocvalues::getFunctionHTML() const {
    QString returnValue = "";

    if (!getKeyLinkHtml().isEmpty()) {
        returnValue += getKeyLinkHtml() +"<br />";
        returnValue += "<br />";
    }

    if (!getHeadline().isEmpty()) {
        returnValue += "<b>" + getHeadline() + ".</b><br />";
    }

    if (!getParamHtml().isEmpty()) {
        returnValue += "Parameters:<br />";

        returnValue += "("+getParamHtml() + ")<br />";
    }

    if (!getOpParamHtml().isEmpty()) {
        returnValue += "Optional Parameters: <br />";
        returnValue += "("+getOpParamHtml() +")<br />";
    }

    if (!getHeadline().isEmpty() || (!getParamHtml().isEmpty() || !getOpParamHtml().isEmpty())) {
        returnValue += "<hr />";
    }
    returnValue += m_additionalFuncHtml;

    if (!m_paramNoHead.isEmpty() || !m_opParamNoHead.isEmpty()) {
        returnValue += "<b>Additional usage of " + getKeyName() + ":</b><br />";
    }

    if (!m_paramNoHead.isEmpty()) {
        returnValue += "Parameters:<br />";
        returnValue += m_paramNoHead;
    }

    if (!m_opParamNoHead.isEmpty()) {
        returnValue += "Optional Parameters: <br />";
        returnValue += m_opParamNoHead;
    }
    return returnValue;
}
