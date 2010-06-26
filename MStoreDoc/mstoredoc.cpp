#include "mstoredoc.h"

MStoredoc::MStoredoc(): QObject(0) {
    currentPackages = new QList<MDocvalues*>();
    currentdocObject = new MDocvalues();
    addPackage("Macaulay2Doc");

}
MDocvalues* MStoredoc::completerNameHelp(const QString &name) {
    QString tempName = name;
    tempName = tempName.mid(0, name.size()-1);
    foreach(MDocvalues *key, m_DocValues) {
        if (key->getKeyName() == tempName) {
            return key;
        }
    }
    return (new MDocvalues());
}
void MStoredoc::addPackage(const QString &packageName) {

    if (loadedPackages.contains(packageName)) {
        return;
    }

    loadedPackages.append(packageName);

    QString databasePath =  QDir::homePath() + "/.M2E/Macaulay2EditorDB.db";
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);
    db.open();


    QString currentFunction ="";
    //MDocvalues *currentdocObject;
    QSqlQuery selectDefaultPackages("SELECT * FROM functions, packages WHERE packages.name = '"+packageName+"'"
                                    "AND packageId = packages.id ORDER BY name");
    int count = 0;
    QString functionName = "";
    QString paramfromDB;
    QString opParamfromDB;
    QString headLinefromDB;
    QString filenamefromDB;
    QString functionLink;
    int idFromDB;

    QString params;
    QString opparams;

    while (selectDefaultPackages.next()) {

        count++;
        idFromDB = selectDefaultPackages.value(0).toInt();
        functionName = selectDefaultPackages.value(1).toString();
        params = selectDefaultPackages.value(2).toString();
        opparams = selectDefaultPackages.value(3).toString();
        headLinefromDB = selectDefaultPackages.value(4).toString();
        functionLink = selectDefaultPackages.value(5).toString();
        paramfromDB = selectDefaultPackages.value(6).toString();
        opParamfromDB = selectDefaultPackages.value(7).toString();

        if (currentFunction == functionName) {
            MDocvalues *docObject = new MDocvalues();
            docObject->setId(idFromDB);
            docObject->setKeyName(functionName);
            docObject->setParam(params);
            docObject->setParamHtml(paramfromDB);
            docObject->setOpParam(opparams);
            docObject->setOpParamHtml(opParamfromDB);
            docObject->setHeadline(headLinefromDB);
            docObject->setKeyLinkHtml(functionLink);
            currentdocObject->addDocValue(docObject);
        }
        else {
            MDocvalues *docObject = new MDocvalues();
            docObject->setId(idFromDB);
            docObject->setKeyName(functionName);
            docObject->setParam(params);
            docObject->setParamHtml(paramfromDB);
            docObject->setOpParam(opparams);
            docObject->setOpParamHtml(opParamfromDB);
            docObject->setHeadline(headLinefromDB);
            docObject->setKeyLinkHtml(functionLink);
            currentdocObject = docObject;
            currentPackages->append(docObject);
            m_DocValues.append(docObject);
            currentFunction =  functionName;
        }
    }
    db.close();
}
MDocvalues* MStoredoc::getByName(const QString &name) {
    MDocvalues* docObject = new MDocvalues();
    int compareBuffer;

    for (int i = 0; i<currentPackages->size(); i++) {
        compareBuffer = QString::compare(currentPackages->at(i)->getKeyName(), name, Qt::CaseSensitive);
        if (compareBuffer == 0) {
            docObject = currentPackages->at(i);
            return docObject;
        }
    }
    return docObject;
}
MDocvalues* MStoredoc::getById(const QModelIndex &indexAt) {
    return currentPackages->at(indexAt.row());
}
bool MStoredoc::exists(const QString &name) {
    for (int i = 0; i<currentPackages->size();i++) {
        if (currentPackages->at(i)->getKeyName() == name) {
            return true;
        }
    }
    return false;
}
