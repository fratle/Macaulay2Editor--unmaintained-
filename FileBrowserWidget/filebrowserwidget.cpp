#include "filebrowserwidget.h"
#include <kicon.h>


FileBrowserWidget::FileBrowserWidget(QWidget *parent) : QDockWidget(parent) {
    setupWidgets();
    setuptoolbar();
    setWidget(listview);
    setupPrivateSignals();
}


void FileBrowserWidget::setuptoolbar() {
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);

    toolbar = new QToolBar(this);
    toolbar->addAction(KIcon("go-up"),"go-up", this, SLOT(m_go_up()));
    toolbar->addWidget(spacerWidget);
    toolbar->addAction(KIcon("window-close"), "close", this, SLOT(setVisible(bool)));
    toolbar->setIconSize(QSize(16,16));
    setTitleBarWidget(toolbar);
}


void FileBrowserWidget::setupWidgets() {
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath(QDir::rootPath());

    listview = new QListView(this);
    listview->setModel(fileSystemModel);
    listview->setRootIndex(fileSystemModel->index(QDir::currentPath()));
    listview->setCurrentIndex(fileSystemModel->index(QDir::currentPath()));

}


void FileBrowserWidget::setupPrivateSignals() {
    connect(listview, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(m_itemDoubleClick(QModelIndex)));
}


void FileBrowserWidget::m_itemDoubleClick(const QModelIndex &index) {
    if (fileSystemModel->isDir(index)) {
        m_folderDoubleClick(fileSystemModel->filePath(index));
    }
    else {
        m_fileDoubleClick(fileSystemModel->filePath(index));
    }
}


void FileBrowserWidget::m_fileDoubleClick(const QString& path) {
    emit(fileOpen(path));
}


void FileBrowserWidget::m_go_up() {
    QModelIndex index =  listview->currentIndex();
    if (index.isValid()) {
        listview->setRootIndex(fileSystemModel->parent(index));
        listview->setCurrentIndex(fileSystemModel->parent(index));
    }
}


void FileBrowserWidget::m_folderDoubleClick(const QString& path) {
    listview->setRootIndex(fileSystemModel->index(path));
}

