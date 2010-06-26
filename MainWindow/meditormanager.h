#ifndef MEDITORMANAGER_H
#define MEDITORMANAGER_H

#include <QObject>
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <KTextEditor/Editor>
#include <KTextEditor/EditorChooser>
#include <KTextEditor/SmartInterface>
#include <KTextEditor/SmartRangeNotifier>
#include <KTextEditor/Attribute>
#include <KTextEditor/ModificationInterface>
#include <KTextEditor/TextHintInterface>
#include <KTextEditor/CodeCompletionInterface>
#include <MStoreDoc/mstoredoc.h>
#include <MAutocomplete/mautocomplete.h>
#include <KLocale>
#include <KApplication>
#include <KFileDialog>
#include <KMessageBox>
#include "meditor.h"

class MEditorManager : public QObject
{
    Q_OBJECT
public:
    explicit MEditorManager(QObject *parent = 0);
    KTextEditor::View* getCurrentView();
    KTextEditor::Document* getCurrentDocument();
    MEditor *getCurrentEditor();
    KTextEditor::View* createEditor(QString filename="");
    void openFile();
    int editorCount();
    QList<MEditor*> &getAllEditors();


private:
    int m_index;

    KTextEditor::Editor *editor;

    QList<MEditor*> editors;
    MEditor *activeEditor;

    MAutocompleteWidget *textHintToolTip;


    void enableTextHint(MEditor* meditor);
    void disableAutoCompletion();
    void updateAutoCompletion();


private slots:
    //! run when mouse cursor enters range.
    /*!
      \param range that has been entered.
      \param view view containing range.
      used with texthints, to keep track of mouseover.
    */
    void mouseEnteredRange (KTextEditor::SmartRange *range, KTextEditor::View *view);

    //! run when mouse cursor exits range.
    /*!
        \param range that has been exited.
        \param view view containing range.
        used with texthints, to keep track of mouseover.
    */
    void mouseExitedRange (KTextEditor::SmartRange *range, KTextEditor::View *view);


    //! runs when texthint is requested.
    /*!
        \param cursor current mouse cursor position.
    */
    void textHintRequested(const KTextEditor::Cursor& cursor, QString& word);
signals:
    void newEditorCreated(KTextEditor::View*);
public slots:
    void changeIndex(int index);
    void closeEditor(int index);
};

#endif
