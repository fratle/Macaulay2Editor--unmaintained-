#include "mterminal.h"
#include <QDebug>
#include <QMenu>
#include <KGlobalSettings>

MTerminal::MTerminal(QWidget *parent) : KTextEdit(parent) {
    pos = 0;
    setAcceptRichText(true);
    setUndoRedoEnabled (false);
    setupContextMenu();

    setPalette(QPalette(Qt::black));
    setTextColor(QColor(Qt::lightGray));

    scrollbar = verticalScrollBar();
    setLineWrapMode(QTextEdit::NoWrap);

    QFont f = KGlobalSettings::fixedFont();
    setFont(f);
}

void MTerminal::setupContextMenu() {
    contextMenu = createStandardContextMenu();
    QList<QAction*> actions = contextMenu->actions();
    contextMenu->clear();
    contextMenu->addAction("Cut", this, SLOT(cut()));
    contextMenu->addAction("Copy", this, SLOT(copy()));
    contextMenu->addAction("Paste", this, SLOT(paste()));
    contextMenu->addAction("Select All", this, SLOT(selectAll()));
    contextMenu->addAction("Restart", this, SLOT(restart()));
}

void MTerminal::contextMenuEvent ( QContextMenuEvent * event ) {
    contextMenu->exec(event->globalPos());
}

void MTerminal::restart() {
    QKeyEvent e(QEvent::KeyPress, 0, Qt::NoModifier, "restart\r\n");
    emit keyPressSignal(&e);
}

void MTerminal::paste() {
    QClipboard *clipboard = QApplication::clipboard();
    QString text = clipboard->text();
    if (!text.isEmpty()) {
        text.replace('\n', '\r');
        QKeyEvent e(QEvent::KeyPress, 0, Qt::NoModifier, text);
        emit keyPressSignal(&e); // expose as a big fat keypress event
    }
}

void MTerminal::insertText(const QString &text) {
    QString _text = text;
    insertPlainText(_text);
    //pos = toPlainText().length();
    scrollbar->triggerAction(QAbstractSlider::SliderToMaximum);
}

void MTerminal::appendText(const QString &text) {
    append(text);
    //pos = toPlainText().length();
    scrollbar->triggerAction(QAbstractSlider::SliderToMaximum);
}

void MTerminal::cut() {
    if (textCursor().hasSelection()) {
        //if( ( textCursor().selectionStart() >= pos ) && ( textCursor().selectionEnd() >= pos)){
        QTextEdit::cut();
        //}
    }
}

void MTerminal::inputMethodEvent(QInputMethodEvent *e) {
    QKeyEvent *a = new QKeyEvent(QEvent::None, 0, Qt::NoModifier, e->commitString());
    qDebug() << "inputMethodEvent";
    emit keyPressSignal(a);
}

void MTerminal::keyPressEvent(QKeyEvent *e) {
    int key = e->key();
    switch (key) {
    case Qt::Key_Backspace:
        QTextEdit::keyPressEvent(e);
        emit keyPressSignalNoOutput(e);
        break;

    default:
        emit keyPressSignal(e);
        break;
    }
}



