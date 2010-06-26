#include "mprocess.h"
#include <QtDebug>
#include <QObject>
#include <QDir>
#include <QTime>
#include <QSettings>
#include <signal.h>
#include <sys/types.h>

MProcess::MProcess(QObject *parent) {
    QStringList arguments;
    M2process = new KPtyProcess(this);
    M2process->setUseUtmp(true);
    M2Device = M2process->pty();
    M2process->setPtyChannels(KPtyProcess::AllChannels);
    connect(M2Device, SIGNAL(readyRead()), this, SLOT(m_readyReadStdout()));
    connect(M2process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
    connect(M2process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
}

void MProcess::sendInput(const QString &code) {
    M2Device->write(code.toUtf8());
    M2Device->waitForBytesWritten(-1);
    M2Device->waitForReadyRead(-1);
}

void MProcess::signal(int sig) {
    kill(M2process->pid(), sig);
}

void MProcess::restart() {
    M2process->close();
    start();
}

void MProcess::m_readyReadStdout() {
    QByteArray bytestdout = M2Device->readAll();
    QString stdout = bytestdout;
    emit receivedStdout(stdout);
}

void MProcess::processError(KProcess::ProcessError error) {
    qDebug() << "KProcess::ProcessError" << error;
}

void MProcess::processFinished(int exitCode, KProcess::ExitStatus exitstatus) {
    qDebug() << "KProcess::ExitStatus" << exitstatus;
}

void MProcess::start() {
    QSettings settings;
    QString program =  settings.value("MainWindow/Executable").toString();
    if (!program.isEmpty()) {
        M2process->setWorkingDirectory(QDir::homePath());
        M2process->setShellCommand(program + " --no-readline");
        M2process->start();
        M2Device->waitForBytesWritten(-1);
        M2Device->waitForReadyRead(-1);
        M2process->waitForFinished(2000);
    }
}

