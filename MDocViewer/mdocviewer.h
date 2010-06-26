#ifndef MDOCVIEWER_H
#define MDOCVIEWER_H
#include <QDockWidget>
#include <QTextBrowser>
#include <KAction>
#include <QToolBar>

//! DockWidget which holds a QTextBrowser for displaying html documents.
/*!
*/
class MDocViewer : public QDockWidget
{
    Q_OBJECT
public:
    MDocViewer(QWidget *parent = 0);
    //! append text to the text browser.
    void append(const QString &text);
private:
    //! text browser for html.
    QTextBrowser *browser;
    //! navigation action, goto previous source.
    KAction *act_goback;
    //! navigation action, goto forward source.
    KAction *act_goforward;

    //! Action to maximize this widget.
    KAction *act_maximized;
    //! Action to normalize this widget.
    KAction *act_normal;
    //! Action to pin this widget.
    KAction *act_pin;
    //! widgets main toolbar.
    /*!
        contains icons to trigger the navigation actions.
    */
    QToolBar *toolbar;
public slots:
    //! set the source document of the QTextBrowser, browser.
    void setSource(const QUrl &url);
    //! goto the previous source.
    void go_back();
    //! goto the forward source.
    void go_forward();
    //! enable/disable the forward action depending on if a forward source is available.
    void forward_available(bool value);
    //! enable/disable the back action depending on if a previous source is available.
    void back_available(bool value);
    //! maximize widget.
    void go_maximized();
    //! normalize widget.
    void go_normal();
    //! pin widget.
    void go_pin();
};

#endif // MDOCVIEWER_H
