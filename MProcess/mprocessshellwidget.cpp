#include "mprocessshellwidget.h"
#include  <QDebug>
#include <QToolBar>
#include <signal.h>
#include <sys/types.h>
#include <KFileDialog>
MProcessShellWidget::MProcessShellWidget(QWidget *parent): QDockWidget(parent) {
    sendOutputToView = false;
    m_terminal = new MTerminal();
    setWidget(m_terminal);


    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);

    QToolBar *toolbar = new QToolBar(this);
    toolbar->addAction(KIcon("system-reboot"),"restart the Macaulay2 process", this, SLOT(m_restart()));

    toolbar->addAction("Break", this, SLOT(m_break()));

    toolbar->addAction("Kill", this, SLOT(m_kill()));

    toolbar->addAction("Quit", this, SLOT(m_quit()));

    toolbar->addAction("Stop", this, SLOT(m_stop()));

    toolbar->addAction(KIcon("document-save-as"), "Save to file", this, SLOT(m_save_to_file()));

    toolbar->addWidget(spacerWidget);
    toolbar->addAction(KIcon("window-close"), "close", this, SLOT(setVisible(bool)));
    toolbar->setIconSize(QSize(16,16));
    setTitleBarWidget(toolbar);


    process = new MProcess(this);
    setupSignals();
    process->start();
}

void MProcessShellWidget::m_save_to_file() {
    QString filename  = KFileDialog::getSaveFileName(QDir::homePath());
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);
    out << m_terminal->toPlainText();
    file.close();
}

void MProcessShellWidget::m_restart() {
    process->restart();
}


void MProcessShellWidget::m_break() {
    process->signal(SIGINT);
}

void MProcessShellWidget::m_stop() {
    process->signal(SIGSTOP);
}

void MProcessShellWidget::m_quit() {
    process->signal(SIGQUIT);
}

void MProcessShellWidget::m_kill() {
    process->signal(SIGKILL);
}

void MProcessShellWidget::start() {
    process->start();
}

void MProcessShellWidget::setupSignals() {
    connect(m_terminal, SIGNAL(keyPressSignalNoOutput(const QKeyEvent*)),this, SLOT(keyPressSlotNoOutput(const QKeyEvent*)));
    connect(m_terminal, SIGNAL(keyPressSignal(const QKeyEvent*)), this, SLOT(keyPressSlot(const QKeyEvent*)));
    connect(process, SIGNAL(receivedStdout(const QString&)), this, SLOT(receiveStdout(const QString&)));
}

void MProcessShellWidget::keyPressSlot(const QKeyEvent *e) {
    if (!e->text().isEmpty()) {
        process->sendInput(e->text());
    }
}

void MProcessShellWidget::keyPressSlotNoOutput(const QKeyEvent *e) {
    disconnect(process, 0,0,0);
    if (!e->text().isEmpty()) {
        process->sendInput(e->text());
    }
    connect(process, SIGNAL(receivedStdout(const QString&)), this, SLOT(receiveStdout(const QString&)));
}

void MProcessShellWidget::receiveStdout(const QString &stdout) {
    QString toView = stdout;
    toView.remove("\r");
    m_terminal->insertText(toView);
    if (sendOutputToView) {
        m_externalView->insertText(toView);
    }
}


void MProcessShellWidget::sendInput(const QString &input) {
    process->sendInput(input);
}

void MProcessShellWidget::sendOutputToDoc(KTextEditor::View *view) {
    sendOutputToView = true;
    m_externalView = view;
}

void MProcessShellWidget::disableOutputToDoc() {
    sendOutputToView = false;
}
