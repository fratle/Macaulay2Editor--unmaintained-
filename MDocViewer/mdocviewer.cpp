#include "mdocviewer.h"
#include <QToolBar>
#include <QDebug>
MDocViewer::MDocViewer(QWidget *parent): QDockWidget(parent)
{
    browser = new QTextBrowser(this);

    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);

    act_goback = new KAction(KIcon("go-previous"), "back", this);
    act_goforward = new KAction(KIcon("go-next"), "forward", this);
    act_maximized = new KAction(KIcon("zoom-in"), "maximize", this);
    act_normal = new KAction(KIcon("zoom-out"), "normal", this);
    act_pin = new KAction(KIcon("go-jump"), "pin", this);

    act_goback->setEnabled(false);
    act_goforward->setEnabled(false);

    toolbar = new QToolBar(this);
    toolbar->addAction(act_goback);
    toolbar->addAction(act_goforward);
    toolbar->addWidget(spacerWidget);
    toolbar->addAction(act_pin);
    toolbar->addAction(act_normal);
    toolbar->addAction(act_maximized);
    toolbar->addAction(KIcon("window-close"), "close", this, SLOT(setVisible(bool)));
    toolbar->setIconSize(QSize(16,16));
    setTitleBarWidget(toolbar);
    setWidget(browser);

    connect(act_goback, SIGNAL(triggered()), this, SLOT(go_back()));
    connect(act_goforward, SIGNAL(triggered()), this, SLOT(go_forward()));
    connect(act_maximized, SIGNAL(triggered()), this, SLOT(go_maximized()));
    connect(act_normal, SIGNAL(triggered()), this, SLOT(go_normal()));
    connect(act_pin, SIGNAL(triggered()), this, SLOT(go_pin()));

    connect(browser, SIGNAL(backwardAvailable(bool)), this, SLOT(back_available(bool)));
    connect(browser, SIGNAL(forwardAvailable(bool)), this, SLOT(forward_available(bool)));
}

void MDocViewer::back_available(bool value) {
    act_goback->setEnabled(value);
}

void MDocViewer::forward_available(bool value) {
    act_goforward->setEnabled(value);
}

void MDocViewer::append(const QString &text) {
    browser->append(text);
}

void MDocViewer::setSource(const QUrl &url) {
    browser->setSource(url);
}

void MDocViewer::go_back() {
    if (browser->isBackwardAvailable()) {
        browser->backward();
    }
}

void MDocViewer::go_forward() {
    if (browser->isForwardAvailable()) {
        browser->forward();
    }
}

void MDocViewer::go_maximized() {
    if (!this->isFloating()) {
        this->setFloating(true);
    }
    this->showMaximized();
}

void MDocViewer::go_normal() {

    if (!this->isFloating()) {
        this->setFloating(true);
    }
    this->showNormal();
}

void MDocViewer::go_pin() {
    if (!this->isFloating()) {
        this->setFloating(true);
    }
    else {
        this->setFloating(false);
    }


}
