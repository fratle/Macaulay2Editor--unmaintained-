#include "mainwindow.h"
#include <KMimeTypeTrader>
#include "MProcess/mprocess.h"
#include <KShortcutsDialog>
#include <KAboutData>
#include <ktextedit.h>
#include "MSettings/msettings.h"
#include <MParser/mparser.h>
#include <QDebug>

MainWindow *MainWindow::instance = 0;
MainWindow* MainWindow::getInstance() {
    if (!MainWindow::instance) {
        MainWindow::instance = new MainWindow();
    }
    return MainWindow::instance;
}

MainWindow::MainWindow(QWidget * parent)
{
    MainWindow::instance = this;
    QCoreApplication::setOrganizationName("Macaulay2");
    QCoreApplication::setApplicationName("Macaulay2Editor");
    firstRun();
    this->resize(800,600);

    m_statusBar = statusBar();

    m_tab = new KTabWidget(this);
    m_tab->setTabsClosable(true);
    m_tab->setDocumentMode(true);
    m_tab->setMovable(true);
    m_tab->resize(800,600);

    setCentralWidget(m_tab);
    setupDockWidgets();
    setupEditorDocsViews();
    setupActions();
    setupContextMenu(editorManager->getCurrentView());

    terminalDock = new MProcessShellWidget(this);
    terminalDock->setObjectName("TerminalDock");
    this->addDockWidget(Qt::BottomDockWidgetArea, terminalDock);

    setXMLFile(QDir::homePath() + "/.M2E/editorui.rc");
    createShellGUI(true);

    setStandardToolBarMenuEnabled(true);
    guiFactory()->addClient(editorManager->getCurrentView());
    setupSignals();

    QSettings settings;
    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();
}

void MainWindow::setupEditorDocsViews() {
    editorManager = new MEditorManager(this);
}

void MainWindow::openUrl(const QUrl &url) {
    docViewer->setSource(url);
}

void MainWindow::setupDockWidgets() {
    docViewer = new MDocViewer(this);
    docViewer->setObjectName("DocumentationViewer");
    filebrowser = new FileBrowserWidget(this);
    filebrowser->setObjectName("Filebrowser");
    this->addDockWidget(Qt::LeftDockWidgetArea, filebrowser);
    this->addDockWidget(Qt::RightDockWidgetArea, docViewer);
}

//connect signals here.
void MainWindow::setupSignals() {
    /////////M_TAB//////////
    connect(m_tab, SIGNAL(currentChanged ( int )), this, SLOT(changeTab(int)));
    connect(m_tab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    //connect(m_tab, SIGNAL(contextMenu(QWidget*,const QPoint &)), this, SLOT(updateTab(QWidget*,const QPoint &)));

    /////////FileBrowser and DocViewer////
    connect(filebrowser, SIGNAL(visibilityChanged(bool)), m_show_filebrowser, SLOT(setChecked(bool)));
    connect(docViewer, SIGNAL(visibilityChanged(bool)), m_show_docviewer, SLOT(setChecked(bool)));
    /////////CONSOLE////////
    connect(terminalDock, SIGNAL(visibilityChanged(bool)), m_show_console, SLOT(setChecked(bool)));
}

void MainWindow::editorCreated(KTextEditor::View* view) {
    m_tab->addTab(view, view->document()->documentName());
    m_tab->setCurrentWidget(view);
    setupContextMenu(view);
}

void MainWindow::setupContextMenu(KTextEditor::View *view) {
    //setup extra methods in the views contextmenu
    QMenu *viewContextMenu;
    viewContextMenu = view->defaultContextMenu();
    viewContextMenu->addAction(actSendLine);
    viewContextMenu->addAction(actSendBlock);
    viewContextMenu->addAction(actSendDoc);
    view->setContextMenu(viewContextMenu);
}


void MainWindow::setupActions()
{
    KStandardAction::quit(kapp, SLOT(quit()), actionCollection());
    KStandardAction::open(this, SLOT(openFile()), actionCollection());
    KStandardAction::clear(this, SLOT(clear()), actionCollection());
    KStandardAction::openNew(this, SLOT(openNewTab()), actionCollection());
    KStandardAction::close(this, SLOT(closeTab()), actionCollection());

    m_show_console = new KToggleAction(i18n("Show Console"), this);
    m_show_filebrowser = new KToggleAction(i18n("Show filebrowser"), this);
    m_show_docviewer = new KToggleAction(i18n("Show documentation viewer"), this);
    m_output_to_view = new KToggleAction(i18n("Output"), this);
    m_output_to_view->setChecked(false);
    m_output_to_view->setToolTip("Send Macaulay2 ouput to a new document");

    m_show_console->setChecked(true);
    m_show_filebrowser->setChecked(true);
    m_show_docviewer->setChecked(true);

    KAction *m_m2Settings = new KAction(i18n("M2E settings"), this);

    actSendLine = new KAction("Send line to Console", this);
    connect(actSendLine, SIGNAL(triggered()), this, SLOT(m_sendLineToConsole()));
    actSendLine->setShortcut(QKeySequence("Ctrl+F11"));
    actSendBlock = new KAction("Send block to Console", this);
    connect(actSendBlock, SIGNAL(triggered()), this, SLOT(m_sendBlockToConsole()));
    actSendBlock->setShortcut(QKeySequence("Ctrl+F10"));
    actSendDoc = new KAction("Send document to Console", this);
    connect(actSendDoc, SIGNAL(triggered()), this, SLOT(m_sendDocToConsole()));
    actSendDoc->setShortcut(QKeySequence("Ctrl+F9"));

    actionCollection()->addAction("sendLine", actSendLine);
    actionCollection()->addAction("sendBlock", actSendBlock);
    actionCollection()->addAction("sendDoc", actSendDoc);

    actionCollection()->addAction("show_console", m_show_console);
    actionCollection()->addAction("show_filebrowser", m_show_filebrowser);
    actionCollection()->addAction("show_docviewer", m_show_docviewer);
    actionCollection()->addAction("output_to_doc", m_output_to_view);
    actionCollection()->addAction("macaulay2_configure", m_m2Settings);

    connect(m_m2Settings, SIGNAL(triggered()), this, SLOT(m2Settings()));
    connect(m_output_to_view, SIGNAL(triggered(bool)), this, SLOT(sendOuputToDoc(bool)));
    connect(m_show_console, SIGNAL(triggered(bool)), this, SLOT(showConsole(bool)));
    connect(m_show_filebrowser, SIGNAL(triggered(bool)), this, SLOT(showFilebrowser(bool)));
    connect(m_show_docviewer, SIGNAL(triggered(bool)), this, SLOT(showDocViewer(bool)));
    connect(filebrowser, SIGNAL(fileOpen(const QString&)), this, SLOT(openFile(const QString&)));

    KSharedConfig::Ptr config = KGlobal::config();
    KConfigGroup cgMacaulay2 = KConfigGroup(config, "M2E Settings");

    setStandardToolBarMenuEnabled(true);

    KAction *a = actionCollection()->addAction( KStandardAction::KeyBindings, this, SLOT(editKeys()) );
    a->setWhatsThis(i18n("Configure the application's keyboard shortcut assignments."));
}

void MainWindow::clear()
{
    activeDoc->clear();
}

void MainWindow::editKeys() {
    KShortcutsDialog dlg(KShortcutsEditor::AllActions, KShortcutsEditor::LetterShortcutsAllowed, this);
    QList<KXMLGUIClient*> clients = guiFactory()->clients();

    foreach(KXMLGUIClient *client, clients) {
#ifdef __GNUC__
#warning there appear to be invalid clients after session switching
#endif
        //     kDebug(13001)<<"adding client to shortcut editor";
        //     kDebug(13001)<<client;
        //     kDebug(13001)<<client->actionCollection();
        //     kDebug(13001)<<client->componentData().aboutData();
        //     kDebug(13001)<<client->componentData().aboutData()->programName();
        dlg.addCollection ( client->actionCollection(), client->componentData().aboutData()->programName() );
    }
    /*
      dlg.insert( externalTools->actionCollection(), i18n("External Tools") );
    */
    dlg.configure();

    QList<MEditor*> editors = editorManager->getAllEditors();
    for (int i=0;i<editors.count();i++) {
        editors.at(i)->document()->reloadXML();
        editors.at(i)->view()->reloadXML();
    }
}

void MainWindow::openFile()
{
    editorManager->openFile();
}

void MainWindow::openFile(const QString& path) {
    editorManager->createEditor(path);
}

void MainWindow::changeTab(int index) {
    guiFactory()->removeClient(editorManager->getCurrentView());
    editorManager->changeIndex(index);
    guiFactory()->addClient(editorManager->getCurrentView());
}

void MainWindow::closeTab(int index) {
    editorManager->closeEditor(index);
    m_tab->removeTab(index);
    if (m_tab->count() > 0) {
        m_tab->setCurrentIndex(index-1);
    }
}

void MainWindow::openNewTab() {
    editorManager->createEditor();
}

void MainWindow::m2Settings() {
    MSettings *settingswnd = new MSettings(this);
    settingswnd->show();
}

void MainWindow::showConsole(bool checked) {
    terminalDock->setVisible(checked);
}

void MainWindow::showFilebrowser(bool checked) {
    filebrowser->setVisible(checked);
}

void MainWindow::showDocViewer(bool checked) {
    docViewer->setVisible(checked);
}

void MainWindow::sendOuputToDoc(bool checked) {
    if (checked) {
        terminalDock->sendOutputToDoc(editorManager->createEditor());
        m_tab->setTabText(editorManager->editorCount()-1, "Output");
    }
    else {
        terminalDock->disableOutputToDoc();
    }
}

void MainWindow::m_sendLineToConsole() {
    KTextEditor::Cursor currentpos = editorManager->getCurrentView()->cursorPosition();
    QString line = editorManager->getCurrentDocument()->line(currentpos.line());
    line.append("\r");
    terminalDock->sendInput(line);
    currentpos.setLine(currentpos.line()+1);
    if (currentpos.line() <= editorManager->getCurrentDocument()->lines()) {
        editorManager->getCurrentView()->setCursorPosition(currentpos);
    }
}


void MainWindow::m_sendBlockToConsole() {
    if (editorManager->getCurrentView()->selection()) {
        QString block = editorManager->getCurrentView()->selectionText();
        block.append("\r");
        terminalDock->sendInput(block);
    }
}


void MainWindow::m_sendDocToConsole() {
    QString docText = editorManager->getCurrentDocument()->text();
    docText.append("\r");
    terminalDock->sendInput(docText);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.endGroup();

    QList<MEditor*> editors = editorManager->getAllEditors();
    for (int i=0;i<editors.count();i++) {
        editors.at(i)->document()->closeUrl();
        editors.at(i)->view()->close();
    }
    KParts::MainWindow::closeEvent(event);
}

void MainWindow::firstRun() {
    bool showSettings = false;
    bool run_parser = false;

    QSettings settings;
    QDir macaulayDir("/usr/lib/Macaulay2");
    QFileInfo macaulayExe(QDir("/usr/bin"), "M2");

    settings.beginGroup("Autocomplete");
    settings.setValue("Autocomplete/onoff", "true");
    settings.endGroup();

    if (!settings.value("MainWindow/Directory").toString().isEmpty()) {
        showSettings = false;
    }
    else if (macaulayDir.exists()) {
        settings.beginGroup("MainWindow");
        settings.setValue("Directory", "/usr/lib/Macaulay2");
        settings.endGroup();
    }
    else {
        showSettings = true;
    }

    if (!settings.value("MainWindow/Executable").toString().isEmpty()) {
        showSettings = false;
    }

    else if (macaulayExe.exists()) {
        settings.beginGroup("MainWindow");
        settings.setValue("Executable", "/usr/bin/M2");
        settings.endGroup();
    }

    else {
        showSettings = true;
    }

    QString homepath = QDir::homePath();
    QFileInfo maculay2Database(homepath + "/.M2E", "Macaulay2EditorDB.db");

    if (maculay2Database.exists()) {
        run_parser = false;
    }
    else {
        QDir macHome(QDir::homePath() + "/.M2E");
        if (!macHome.exists()) {
            macHome.mkpath(QDir::homePath() + "/.M2E");
        }
        qDebug() << "run_parser = true";
        run_parser = true;
    }

    if (showSettings) {
        MSettings *msettings = new MSettings();
        msettings->show();
    }
    else if (run_parser) {
        runParser();
        qDebug() << "runParser();";
    }
}

void MainWindow::parserFinished() {
    qDebug() << "Finished generating html links for documentation";
    KMessageBox::information(this, "Finished generating completion database.\nPlease restart application.");
}

void MainWindow::restartShell() {
    terminalDock->start();
}

void MainWindow::runParser() {
    MParser *parser = new MParser();
    connect(parser, SIGNAL(finishedHtmlLinks()), this, SLOT(parserFinished()));
    parser->parse();
}
