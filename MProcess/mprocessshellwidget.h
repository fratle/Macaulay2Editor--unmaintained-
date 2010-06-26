#ifndef MPROCESS_SHELL_WIDGET_H
#define MPROCESS_SHELL_WIDGET_H
#include <QDockWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <KTextEditor/View>
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTextEditor/EditorChooser>
#include "mprocess.h"
#include "mterminal.h"
//! Dockwidget for MTerminal widget and connection with MProcess
class MProcessShellWidget: public QDockWidget {
    Q_OBJECT
public:
    MProcessShellWidget(QWidget *parent);
    //! send input to the process.
    void sendInput(const QString& input);
    //! send output from Macaulay2 process to a view.
    void sendOutputToDoc(KTextEditor::View *view);
    void disableOutputToDoc();
    void start();
private slots:
    //! received ouput from the Macaulay2 process is displayed the Mterminal window
    void receiveStdout(const QString &stdout);
    //! handle keypresses in MTerminal and send them to the process
    void keyPressSlot(const QKeyEvent* e);
    //! handle keypresses in MTerminal and send them to the process
    /*!
        No output from process.
    */
    void keyPressSlotNoOutput(const QKeyEvent* e);

    //! restart the Macaulay2 background process
    void m_restart();

    void m_break();
    void m_stop();
    void m_quit();
    void m_kill();
    void m_save_to_file();

private:
    //! optionally send ouput to this view.
    /*!
        is set with \ref sendOutputToDoc(KTextEditor::View *view);
    */
    KTextEditor::View *m_externalView;
    //! true if \ref sendOutputToDoc(KTextEditor::View *view); has been called
    bool sendOutputToView;
    void setupSignals();

    MTerminal *m_terminal;
    MProcess *process;

};
#endif
