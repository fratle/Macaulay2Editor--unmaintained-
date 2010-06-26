#include "meditor.h"
#include "MSettings/msettings.h"

MEditor::MEditor(QObject *parent): QObject(parent) {
}

MEditor::MEditor(KTextEditor::Document *document, QObject *parent) :
        QObject(parent)
{
    m_document = document;
    m_view = m_document->createView(0);
    m_storeDoc = new MStoredoc();

    m_autocomplete = new MAutocomplete(m_view, m_storeDoc);

    m_smart = qobject_cast<KTextEditor::SmartInterface*>( m_document );
    m_textHintRange = m_smart->newSmartRange();
    m_textHintRangeNotifier = new KTextEditor::SmartRangeNotifier();

    connect(m_document, SIGNAL(documentSavedOrUploaded(KTextEditor::Document*,bool)), this, SLOT(documentSaved(KTextEditor::Document*,bool)));

    MSettings settings;

    if (settings.getAutocompletion()) {
        enableAutocompletion();
        setAutomaticCompletion(settings.getAutoAutoCompletion());
    }

    KTextEditor::TextHintInterface *iface2 = dynamic_cast<KTextEditor::TextHintInterface*>(m_view);
    if ( iface2 )
        iface2->enableTextHints(1);

    if (qobject_cast<KTextEditor::ModificationInterface *>(m_document)) {
        qobject_cast<KTextEditor::ModificationInterface *>(m_document)->setModifiedOnDiskWarning (true);
    }
    m_textHintRange->addNotifier(m_textHintRangeNotifier);
}


void MEditor::documentSaved(KTextEditor::Document *document, bool saveAs) {
    QString text = m_document->text();
    QRegExp rx( QRegExp::escape("loadPackage") + "\\s+\"(\\w+)\"" );
    rx.setMinimal(true);
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1) {
        m_storeDoc->addPackage(rx.cap(1));
        pos += rx.matchedLength();
    }
}

KTextEditor::View* MEditor::view() {
    return m_view;
}

KTextEditor::Document* MEditor::document() {
    return m_document;
}

KTextEditor::SmartInterface* MEditor::smart() {
    return m_smart;
}

MAutocomplete* MEditor::autocomplete() {
    return m_autocomplete;
}

MStoredoc* MEditor::storeDoc() {
    return m_storeDoc;
}

KTextEditor::SmartRange* MEditor::textHintRange() {
    return m_textHintRange;
}

KTextEditor::SmartRangeNotifier* MEditor::textHintRangeNotifier() {
    return m_textHintRangeNotifier;
}

void MEditor::setAutomaticCompletion(bool value) {
    KTextEditor::CodeCompletionInterface *iface =
        qobject_cast<KTextEditor::CodeCompletionInterface*>( m_view );
    if ( iface ) {
        iface->setAutomaticInvocationEnabled(value);
    }
}

void MEditor::disableAutocompletion() {
    KTextEditor::CodeCompletionInterface *iface =
        qobject_cast<KTextEditor::CodeCompletionInterface*>( m_view );
    if ( iface ) {
        iface->unregisterCompletionModel(m_autocomplete);
    }
}

void MEditor::enableAutocompletion() {
    KTextEditor::CodeCompletionInterface *iface =
        qobject_cast<KTextEditor::CodeCompletionInterface*>( m_view );
    if ( iface ) {
        iface->registerCompletionModel(m_autocomplete);
    }
}


