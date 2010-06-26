#ifndef MAUTOCOMPLETE_H
#define MAUTOCOMPLETE_H
#include <QModelIndex>
#include <QVariant>
#include <QAbstractItemModel>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QTextBrowser>
#include <QSizePolicy>
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <KTextEditor/Editor>
#include <KTextEditor/EditorChooser>
#include <KTextEditor/CodeCompletionModel>
#include <KTextEditor/Range>
#include <KTextEditor/CodeCompletionModelControllerInterface>
#include "mautocompletewidget.h"
#include "MStoreDoc/mstoredoc.h"

//! Implementation of CodeCompletionModel interface
/*!
    Used for giving main window auto completion by overloading CodeCompletionModel functions.
 */
class MAutocomplete: public KTextEditor::CodeCompletionModel2,
            public KTextEditor::CodeCompletionModelControllerInterface2
{
    Q_OBJECT
    Q_INTERFACES(KTextEditor::CodeCompletionModelControllerInterface2)
public:
    MAutocomplete(QObject *parent, MStoredoc *newmstoredoc);
    //! Gives the completion list wanted functionality
    /*!
      \param QModelIndex &index which is the index of the item in the completion list.
      \param role which switch between different properties for the completion list
      \return QVariant to give the completion list a new functionality or preface.
      */
    QVariant data(const QModelIndex &index, int role) const;
    //! Called whenever something is auto completed, and can set rules for how to handle the auto completion
    /*!
      \param KTextEditor::View *view is the current active view in the editor.
      \param KTextEditor::Range &range is the range of the completed text.
      \param InvocationType invocationType sets the standard for completion.
      */
    void completionInvoked(KTextEditor::View *view,
                           const KTextEditor::Range &range,
                           InvocationType invocationType);
    //! Called when an item from the completion list is beeing executed.
    /*!
     \param KTextEdtior::Document *document is the current active document.
     \param KTextEdtior::Range &word is the range of the executed completed text.
     \param QModelIndex &index is the index at the selected item in the completion list.

     When an item from the completion list is beeing pressed, this function is called. And we can give the executed
     text rules for execution if wanted.
     */
    void executeCompletionItem2(KTextEditor::Document *document,
                                const KTextEditor::Range &word,
                                const QModelIndex &index) const;
protected:
    //! Enmueration for parameter property.
    enum Mode {none, parameters};
private:
    //! Sets a new Mode enum.
    Mode m_mode;
    //! Containing the new updated index in the completion list.
    int completedRow;
    //! Containing the executed completion text.
    QString functionName;

    //! MStoredoc object containing the QList of MDocvalues objects.
    MStoredoc *mstoredoc;
    //! MDocvalues object which contains parameters if the completion item has parameters.
    MDocvalues *parameterValue;
private slots:
    //! Read function name from the item in the completion list which has been sent to the document.
    void getFunctionName(QString &functionName);
    //! Read row for the new completion list.
    void getRow(int row);
signals:
    //! Emit when a completion item is being executed.
    void newFunctionName(QString &functionName) const;
    //! Emit when a completion item is being executed.
    void newRow(int row) const;
};
#endif // MAUTOCOMPLETE_H
