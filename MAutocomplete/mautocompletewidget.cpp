#include <QFontMetrics>
#include "mautocompletewidget.h"
#include "MainWindow/mainwindow.h"

MAutocompleteWidget::MAutocompleteWidget(QWidget *parent, Qt::WindowFlags f): QTextBrowser(parent)
{
    setOpenLinks(false);
    setReadOnly(true);
    //setMinimumSize(100, 5);
    setWindowFlags(f);
    setMaximumHeight(150);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setAcceptRichText(true);
    MainWindow *main = MainWindow::getInstance();
    connect(this, SIGNAL(anchorClicked(const QUrl&)), main, SLOT(openUrl(const QUrl&)));
}

void MAutocompleteWidget::addText(const QString &text) {
    insertHtml(text);
    QFontMetrics fontMetric(currentFont());
    int height = fontMetric.height()*(toPlainText().count("\n")+3);
    setMaximumHeight(height);
}

void MAutocompleteWidget::setVisible(bool visible) {
    QTextBrowser::setVisible(visible);
    verticalScrollBar()->setValue(0);
}

void MAutocompleteWidget::show() {
    QTextBrowser::show();
    verticalScrollBar()->setValue(0);
}
