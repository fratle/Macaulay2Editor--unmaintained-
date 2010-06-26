#ifndef FILE_BROWSER_WIDGET_H
#define FILE_BROWSER_WIDGET_H

#include <QFileSystemModel>
#include <QListView>
#include <QTreeView>
#include <QtGui>
#include <QModelIndex>

//! The FileBrowserWidget provides a simple dock widget for filesystem navigation.
class FileBrowserWidget: public QDockWidget {
    Q_OBJECT
public:
    FileBrowserWidget(QWidget *parent = 0);
public slots:

private slots:
    //! checks if a folder or a file was double clicked then calls the appropriate function.
    void m_itemDoubleClick(const QModelIndex&);
    //! go up a directory.
    void m_go_up();

private:
    //! create and initialise widgets.
    void setupWidgets();
    //! create and initialize the toolbar.
    void setuptoolbar();

    //! Interface to the filesystem.
    QFileSystemModel *fileSystemModel;

    //! widget displaying files and directories.
    QListView *listview;
    //! toolbar containing operations for hiding the widget, and go up one directory.
    QToolBar *toolbar;
    //! main layout.
    QHBoxLayout *hbox;

    //! setup signals.
    void setupPrivateSignals();

    //! emit signal when file is double clicked.
    /*!
      \sa m_folderDoubleClick();
      \param path path to the file that was double clicked.
    */
    void m_fileDoubleClick(const QString&);
    //! change root path when folder is double clicked.
    /*!
      \sa m_fileDoubleClick();
      \param path path to the folder that was double clicked.
    */
    void m_folderDoubleClick(const QString&);

signals:
    //! this signal is emited when a file is double clicked.
    void fileOpen(const QString&);
};

#endif
