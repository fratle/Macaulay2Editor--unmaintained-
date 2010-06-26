#include "mupdatedbthread.h"
#include <QDebug>
#include <KPtyProcess>
MUpdateDBThread::MUpdateDBThread(QMap<QString, QList<MDocumentNode*> > *pkg_docnodes)
{
    m_pkg_docnodes = pkg_docnodes;
    QString databasePath =  QDir::homePath() + "/.M2E/Macaulay2EditorDB.db";
    db = QSqlDatabase::addDatabase("QSQLITE", "updateHtmlLinks");
    db.setDatabaseName(databasePath);
    bool ok = db.open();
    if (ok) {
        qDebug() << "OPEN";
    }
}

MUpdateDBThread::~MUpdateDBThread() {
    db.close();
}

void MUpdateDBThread::updateDbWithHTML(const int& keyId, const QString &keyLink, const QString &paramLink, const QString &opParamLink) {
    QSqlQuery updateQ(db);
    updateQ.prepare("UPDATE functions SET link=:link, paramHtml=:paramHtml, opParamHtml=:opParamHtml WHERE id=:keyId");
    updateQ.bindValue(":link", keyLink);
    updateQ.bindValue(":paramHtml", paramLink);
    updateQ.bindValue(":opParamHtml", opParamLink);
    updateQ.bindValue(":keyId", keyId);
    updateQ.exec();
}

void MUpdateDBThread::run() {
    QSqlQuery updateQ(db);
    updateQ.exec("BEGIN");
    QTime a;
    a.start();
    generateHTML();
    qDebug() << "generateHTML finished";
    updateQ.exec("COMMIT");
    qDebug() << "generateHTML(): " << a.elapsed();
}

QStringList MUpdateDBThread::uniqueTypesFunctions(QList<MDocumentNode*> docnodes) {
    QStringList types_functions;
    for (int i=0;i<docnodes.count();i++) {
        MDocumentNode* node =  docnodes.at(i);
        types_functions.append(node->getKey());
        foreach(QString list, node->getParameterList()) {
            types_functions.append(list);
        }
        foreach(QString list, node->getOptionalParameterList()) {
            types_functions.append(list);
        }
    }
    types_functions.removeDuplicates();
    return types_functions;
}

void MUpdateDBThread::generateHTML() {

    QSettings settings;
    QString program =  settings.value("MainWindow/Executable").toString();
    if (program.isEmpty()) {
        return;
    }

    QRegExp re;
    re.setPattern(QRegExp::escape("<a href=") + ".*" +  QRegExp::escape("</a>"));
    re.setMinimal(true);

    QString regexpString = "<a href=.*>" + QString("(.*)") +  QRegExp::escape("</a>");
    QRegExp rx(regexpString);
    rx.setMinimal(true);

    QMap<QString, QList<MDocumentNode*> >::const_iterator map = m_pkg_docnodes->constBegin();
    while (map != m_pkg_docnodes->constEnd()) {
        QStringList func_vars = uniqueTypesFunctions(map.value());

        QString runCommand;
        runCommand = "loadPackage \"" + map.key() +  "\"\n\r";
        runCommand += "loadPackage \"Text\"\n\r";
        runCommand += "html TO RR\n\r";

        for (int i=0;i<func_vars.size();i++) {
            QString name = func_vars.at(i);
            runCommand += "html TO \"" + name + "\"\n\r";
        }
        runCommand += "exit\n\r";

        KProcess *process = new KPtyProcess();
        process->setWorkingDirectory(QDir::homePath());
        process->setProgram(program);
        process->setOutputChannelMode(KProcess::MergedChannels);
        process->start();
        process->waitForFinished(1000);

        process->write(runCommand.toUtf8());
        process->waitForBytesWritten(-1);
        process->waitForReadyRead(-1);
        process->waitForFinished(-1);
        QString html = process->readAll();
        process->close();

        int pos = 0;
        while ((pos = rx.indexIn(html, pos)) != -1) {
            if (m_func_html.value(rx.cap(1)).isEmpty()) {
                m_func_html.insert(rx.cap(1), rx.cap(0));
            }
            pos += rx.matchedLength();
        }
        map++;
    }
    updateDatabase();
}

void MUpdateDBThread::updateDatabase() {
    QMap<QString, QList<MDocumentNode*> >::const_iterator map = m_pkg_docnodes->constBegin();
    while (map != m_pkg_docnodes->constEnd()) {
        QList<MDocumentNode*> docnodes = map.value();
        for (int i = 0;i<docnodes.count();i++) {
            MDocumentNode *node = docnodes.at(i);
            QString funcHtml = "";
            int id = node->getDatabaseIndex();
            QString funcName = node->getKey();
            QStringList opParamList =  node->getOptionalParameterList();
            QStringList paramList = node->getParameterList();

            funcHtml = m_func_html.value(funcName, funcName);

            QStringList paramHtmlList;
            QStringList opparamHtmlList;

            foreach(QString param, paramList) {
                if (!m_func_html.value(param).isEmpty()) {
                    paramHtmlList.append(m_func_html.value(param));
                }
                else {
                    paramHtmlList.append(param);
                }
            }

            foreach(QString param, opParamList) {
                if (!m_func_html.value(param).isEmpty()) {
                    opparamHtmlList.append(m_func_html.value(param));
                }
                else {
                    opparamHtmlList.append(param);
                }
            }
            updateDbWithHTML(id, funcHtml, paramHtmlList.join(","), opparamHtmlList.join(","));
        }
        ++map;
    }
}
