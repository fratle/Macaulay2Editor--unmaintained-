#ifndef MAUTOCOMPLETEWIDGET_H
#define MAUTOCOMPLETEWIDGET_H

#include <QTextBrowser>
//! Simple QTextBrowser Widget
/*!
    Used for displaying information about Macaulay2 autocompletions and texthints in the editor.
*/
class MAutocompleteWidget : public QTextBrowser
{
    Q_OBJECT
public:
    MAutocompleteWidget(QWidget *parent=0, Qt::WindowFlags f = 0);
    //! add text to the widget
    void addText(const QString&);
    /*!
        show() is overridden to update the scrollbar position before showing the widget.
    */
    void show();
    /*!
      setVisible(bool visible) is overridden to update the scrollbar position before showing the widget.
    */
    void setVisible(bool visible);

};

#endif // MAUTOCOMPLETEWIDGET_H
