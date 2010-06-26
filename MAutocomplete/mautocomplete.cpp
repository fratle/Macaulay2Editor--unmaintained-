#include "mautocomplete.h"

MAutocomplete::MAutocomplete(QObject *parent, MStoredoc *newmstoredoc) : KTextEditor::CodeCompletionModel2(parent) {
    setHasGroups(false);
    m_mode = none;

    mstoredoc = newmstoredoc;

    connect(this, SIGNAL(newFunctionName(QString&)), this, SLOT(getFunctionName(QString&)));
    connect(this, SIGNAL(newRow(int)), this, SLOT(getRow(int)));
}


void MAutocomplete::completionInvoked(KTextEditor::View *view,
                                      const KTextEditor::Range &range,
                                      InvocationType invocationType) {
    Q_UNUSED (range)
    Q_UNUSED (invocationType)

    KTextEditor::Document *doc = view->document();

    setRowCount(mstoredoc->currentPackages->size());

    int line, column;
    view->cursorPosition().position(line, column);

    QString lineStr = view->document()->line(line);
    QString leftCh = lineStr.mid( column-1, 1);

    QString completionCheck;
    QRegExp check("(\\w+\\($)");
    int pos = check.indexIn(lineStr);
    if (pos > -1) {
        completionCheck = check.cap(0);
    }
    parameterValue = mstoredoc->completerNameHelp(completionCheck);

    if (!parameterValue->isEmpty()) {
        m_mode = parameters;
        setRowCount(1);

        if (leftCh == "(") {
            KTextEditor::Cursor curPos = view->cursorPosition();
            curPos.setColumn( curPos.column() );
            doc->insertText(curPos, ")");
            view->setCursorPosition(curPos);
        }

    }
    else if (leftCh == "(") {
        m_mode = parameters;
        setRowCount(1);
    }
    else {
        m_mode = none;
    }
}
void MAutocomplete::getFunctionName(QString &funcName) {
    functionName = funcName;
}
void MAutocomplete::getRow(int row) {
    completedRow = row;
}
void MAutocomplete::executeCompletionItem2(KTextEditor::Document *document,
        const KTextEditor::Range &word,
        const QModelIndex &index) const {

    KTextEditor::Range toReplace = word;

    QString text = data(index.sibling( index.row(), Name), Qt::DisplayRole).toString();
    int rowNumber = index.row();

    emit newRow(rowNumber);
    emit newFunctionName(text);
    KTextEditor::View *kv = document->activeView();

    document->replaceText( toReplace, text.trimmed());

    KTextEditor::Cursor curPos = kv->cursorPosition();
    curPos.setColumn( curPos.column());
    kv->setCursorPosition( curPos );
}
QVariant MAutocomplete::data(const QModelIndex &index, int role) const {
    if ( role == InheritanceDepth )
        return 10000; //Very high value, so the word-completion group and items are shown behind any other groups/items if there is multiple

    if ( !index.parent().isValid() ) {
        //It is the group header
        switch ( role )
        {
        case GroupRole:
            return Qt::DisplayRole;
            break;
        case ArgumentHintDepth:
            return 0;
            break;
        case ItemSelected:
            if (m_mode == parameters) {
                return mstoredoc->currentPackages->at(completedRow)->getHeadline();
            }
            return mstoredoc->currentPackages->at(index.row())->getHeadline();
            break;
        case IsExpandable:
            return true;
            break;
        case ExpandingWidget:
            MAutocompleteWidget *edit = new MAutocompleteWidget(0);
            if (m_mode == parameters) {
                edit->addText(mstoredoc->currentPackages->at(completedRow)->getFunctionHTML());
            }
            else {
                edit->addText(mstoredoc->currentPackages->at(index.row())->getFunctionHTML());
            }
            QVariant v;
            v.setValue<QWidget*>(edit);
            return v;
            break;
        }
    }
    if ( index.column() == KTextEditor::CodeCompletionModel::Name && role == Qt::DisplayRole ) {
        QString completion = mstoredoc->currentPackages->at(index.row())->getKeyName();
        if (completion.size() < 40) {
            for (int i=completion.size();i<40;i++) {
                completion.append(" ");
            }
        }
        if (m_mode == parameters) {
            return functionName;
        }
        else {
            return completion;
        }

    }
    return QVariant();
}
