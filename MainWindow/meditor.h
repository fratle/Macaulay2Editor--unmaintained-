#ifndef MEDITOR_H
#define MEDITOR_H

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
#include <MAutocomplete/mautocomplete.h>
#include <MStoreDoc/mstoredoc.h>

class MEditor : public QObject {
    Q_OBJECT
public:
    MEditor(QObject *parent = 0);
    MEditor(KTextEditor::Document *document, QObject *parent = 0);
    KTextEditor::View *view();
    KTextEditor::Document* document();
    KTextEditor::SmartInterface* smart();
    MAutocomplete *autocomplete();
    MStoredoc *storeDoc();
    KTextEditor::SmartRange* textHintRange();
    KTextEditor::SmartRangeNotifier* textHintRangeNotifier();

    void setAutomaticCompletion(bool value);
    void disableAutocompletion();
    void enableAutocompletion();
private:
    KTextEditor::View *m_view;
    KTextEditor::Document* m_document;
    KTextEditor::SmartInterface* m_smart;
    MAutocomplete *m_autocomplete;
    MStoredoc *m_storeDoc;
    KTextEditor::SmartRange* m_textHintRange;
    KTextEditor::SmartRangeNotifier* m_textHintRangeNotifier;


signals:

private slots:


    //! runs when a document is saved.
    /*!
      \param document the document that has been saved.
      \param saveAs true if the document has been saved as.
    */
    void documentSaved(KTextEditor::Document *document, bool saveAs);

public slots:

};

#endif // MEDITOR_H
