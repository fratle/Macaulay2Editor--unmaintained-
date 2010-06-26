#ifndef MTERMINAL_H
#define MTERMINAL_H
#include <QTextEdit>
#include <QKeyEvent>
#include <qapplication.h>
#include <qclipboard.h>
#include <QScrollBar>
#include <KTextEditor/View>
#include <KTextEdit>

//! QTextEdit that acts as a terminal window
class MTerminal : public KTextEdit {
    Q_OBJECT
public:
    MTerminal(QWidget *parent=0);
    //! insert text
    void insertText(const QString &text);
    //! append text
    void appendText(const QString &text);
public slots:
    //! get clipboard content and send it to the macaulay2 process
    void paste();
    void cut();
    //! send restart command to macaulay2 process.
    void restart();
protected:
    void keyPressEvent(QKeyEvent *e);
    void contextMenuEvent ( QContextMenuEvent * event );
    void inputMethodEvent(QInputMethodEvent *);
signals:
    //! emit signal when key is pressed
    /*!
      keypress is send to MProcessShellWidget then to MProcess for evaluation in the process.
    */
    void keyPressSignal(const QKeyEvent*);
    //! emit signal when key is pressed
    /*!
      keypress is send to MProcessShellWidget then to MProcess for evaluation in the process.
      will not return any potential output
      \sa keyPressSignal
    */
    void keyPressSignalNoOutput(const QKeyEvent*);
private:
    void setupContextMenu();
    int pos;
    QScrollBar *scrollbar;
    QMenu *contextMenu;
};

#endif
