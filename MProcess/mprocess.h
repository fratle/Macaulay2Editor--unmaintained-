#ifndef MPROCESS_H
#define MPROCESS_H
#include <kprocess.h>
#include <iostream>
#include <cstdio>
#include <QThread>
#include <kptyprocess.h>
#include <kptydevice.h>

//! This class is used to setup and run the Macaulay2 process
class MProcess: public QObject {
    Q_OBJECT
public:
    MProcess(QObject *parent = 0);
    //! setup executable and start process.
    void start();
    //! send input to process.
    void sendInput(const QString&);

    //! restart Macaulay2 process
    void restart();

    void signal(int sig);
private:
    //! process
    KPtyProcess *M2process;
    //! pty device
    KPtyDevice *M2Device;
private slots:
    //! read ouput from process
    void m_readyReadStdout();
    //! debug error message when an error occurs in the process
    void processError(QProcess::ProcessError error);
    //! debug exitstatus message when an error occurs in the process
    void processFinished(int exitCode, QProcess::ExitStatus exitstatus);
signals:
    //! emitted when receving output from process
    void receivedStdout(const QString&);
};

#endif


