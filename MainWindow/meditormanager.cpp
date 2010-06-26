#include "meditormanager.h"
#include <MAutocomplete/mautocomplete.h>
#include <MSettings/msettings.h>
#include "mainwindow.h"
MEditorManager::MEditorManager(QObject *parent) :
        QObject(parent)
{
    editor = KTextEditor::EditorChooser::editor();
    MainWindow *mwin = MainWindow::getInstance();
    connect(this, SIGNAL(newEditorCreated(KTextEditor::View*)), mwin, SLOT(editorCreated(KTextEditor::View*)));

    createEditor();
    m_index = 0;
    activeEditor = editors.at(m_index);
}

QList<MEditor*> &MEditorManager::getAllEditors() {
    return editors;
}

KTextEditor::View* MEditorManager::getCurrentView() {
    return activeEditor->view();
}

KTextEditor::Document* MEditorManager::getCurrentDocument() {
    return activeEditor->document();
}

KTextEditor::View* MEditorManager::createEditor(QString filename) {
    KTextEditor::Document *new_doc = editor->createDocument(this);
    MEditor *editor =  new MEditor(new_doc);
    editors.append(editor);

    if (!filename.isEmpty()) {
        new_doc->openUrl(filename);
    }

    enableTextHint(editor);
    m_index = editors.count() - 1;
    emit newEditorCreated(editor->view());
    return editor->view();
}

void MEditorManager::enableTextHint(MEditor *meditor) {
    connect(meditor->view(), SIGNAL(needTextHint(const KTextEditor::Cursor&, QString&)), this, SLOT(textHintRequested(const KTextEditor::Cursor&, QString&)));
    connect(meditor->textHintRangeNotifier(), SIGNAL(mouseEnteredRange (KTextEditor::SmartRange*, KTextEditor::View*)), this, SLOT(mouseEnteredRange(KTextEditor::SmartRange*,KTextEditor::View*)));
    connect(meditor->textHintRangeNotifier(), SIGNAL(mouseExitedRange (KTextEditor::SmartRange*, KTextEditor::View*)), this, SLOT(mouseExitedRange(KTextEditor::SmartRange*,KTextEditor::View*)));
}

void MEditorManager::closeEditor(int index) {
    editors.at(index)->document()->closeUrl();
    editors.at(index)->view()->close();
    editors.removeAt(index);
}

void MEditorManager::openFile() {
    if (activeEditor->document()->isModified()) {
        activeEditor->document()->queryClose();
    }
    activeEditor->document()->openUrl(KFileDialog::getOpenFileName());
}

void MEditorManager::changeIndex(int index) {
    m_index = index;
    activeEditor = editors.at(m_index);
}


void MEditorManager::textHintRequested(const KTextEditor::Cursor& cursor, QString& word) {
    if (KApplication::keyboardModifiers() == Qt::ControlModifier) {
        QString filtertext;
        int line, col;
        cursor.position(line, col);
        filtertext = activeEditor->document()->line(line);

        bool valid = true;
        int leftcol = col;
        while (true) {
            if (leftcol < 1) {
                break;
            }
            if (activeEditor->document()->character(KTextEditor::Cursor(line, leftcol-1)).isLetterOrNumber()) {
                --leftcol;
            }
            else {
                break;
            }
        }

        int rightcol = col;
        while (valid) {
            if (!activeEditor->document()->character(KTextEditor::Cursor(line, rightcol)).isLetterOrNumber()) {
                break;
            }
            rightcol++;
        }
        if (leftcol < rightcol) {
            KTextEditor::Range range(KTextEditor::Cursor(line, leftcol), KTextEditor::Cursor(line, rightcol));
            if (activeEditor->storeDoc()->exists(activeEditor->document()->text(range))) {
                if (*activeEditor->textHintRange() != range) {
                    activeEditor->smart()->addHighlightToDocument(activeEditor->textHintRange(), true);
                    activeEditor->textHintRange()->setRange(range);
                }
            }
        }
    }
}

void MEditorManager::mouseEnteredRange (KTextEditor::SmartRange *range, KTextEditor::View *view) {
    KTextEditor::Attribute::Ptr attr(new KTextEditor::Attribute());
    attr->setFontBold(true);
    attr->setFontUnderline ( true );
    attr->setAnchorHref(activeEditor->document()->text(*activeEditor->textHintRange()));
    attr->setAnchor(true);
    activeEditor->textHintRange()->setAttribute(attr);
    MDocvalues *docvalue = activeEditor->storeDoc()->getByName(activeEditor->document()->text(*activeEditor->textHintRange()));
    textHintToolTip = new MAutocompleteWidget(0, Qt::ToolTip);
    textHintToolTip->insertHtml(docvalue->getFunctionHTML());
    textHintToolTip->move(QCursor::pos());
    textHintToolTip->show();
}

void MEditorManager::mouseExitedRange (KTextEditor::SmartRange *range, KTextEditor::View *view) {
    activeEditor->smart()->removeHighlightFromDocument(range);
    activeEditor->textHintRange()->setRange(KTextEditor::Range());
    textHintToolTip->close();
}

int MEditorManager::editorCount() {
    return editors.count();
}
