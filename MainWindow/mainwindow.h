#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <KParts/MainWindow>
#include <ktabwidget.h>
#include <KPluginFactory>
#include <KPluginLoader>
#include <klibloader.h>
#include <KApplication>
#include <KAction>
#include <KLocale>
#include <KActionCollection>
#include <KStandardAction>
#include <KFileDialog>
#include <KMessageBox>
#include <KXMLGUIFactory>
#include <KTextEditor/Document>
#include <KTextEditor/View>
#include <KTextEditor/Editor>
#include <KTextEditor/EditorChooser>
#include <KTextEditor/SmartInterface>
#include <KTextEditor/SmartRangeNotifier>
#include <KTextEditor/Attribute>
#include <QDockWidget>
#include <ktabbar.h>
#include <KToggleAction>
#include "FileBrowserWidget/filebrowserwidget.h"
#include <KProcess>
#include "MProcess/mprocessshellwidget.h"
#include "MAutocomplete/mautocomplete.h"
#include "MAutocomplete/mautocompletewidget.h"
#include "MDocViewer/mdocviewer.h"
#include "MStoreDoc/mstoredoc.h"
#include "meditormanager.h"

//! MainWindow of the Macaulay2Editor
/*!
*/
class MainWindow : public KParts::MainWindow
{
    Q_OBJECT
private slots:
    //! Functions is run automatically when the parser is finished.
    void parserFinished();
    //! Close tab.
    /*!
      \param index index of the tab to close
    */
    void closeTab(int index);

    //! Change focus to other tab.
    /*!
      \param index index of the tab to focus.
    */
    void changeTab(int index);


    void clear();

    //! Open File from dialog
    void openFile();

    //! Open File from filebrowser
    /*!
      \param path path to the file to open.
    */
    void openFile(const QString& path);

    //! open new document tab
    /*!
      Create a new document and a new view.
      Add new tab, for the new document/view, and set focus to the new tab.
    */
    void openNewTab();

    //main menu
    //! toggle visible Console
    void showConsole(bool checked);
    //! toggle visible filebrowser
    void showFilebrowser(bool checked);
    //! toggle visible filebrowser
    void showDocViewer(bool checked);

    //! Open new document for macaulay2 output
    /*!
        everything evaluated in the console will be displayed in the new document
    */
    void sendOuputToDoc(bool checked);

    //Macaulay2 integration
    //! Send current line to Console
    void m_sendLineToConsole();
    //! Send current selected text to Console
    void m_sendBlockToConsole();
    //! Send entire document to Console
    void m_sendDocToConsole();
    //! Config dialog for keyboard shortcuts
    void editKeys();

    //! Show settings window for the application
    /*!
      settings include paths for Macaulay2 executable and directory for macaulay2 documentation databases.
    */
    void m2Settings();

    void editorCreated(KTextEditor::View*);


public:
    //! get mainwindow instance
    /*!
        \return instance of MainWindow.
        MainWindow is a singleton class.
        This is the static method get the singleton instance.
    */
    static MainWindow* getInstance();

    //! toggle automatic autocompletion.
    void setAutomaticAutocomplete();

    //! toggle auto completion.
    void setAutocompletionOnOff();

    //! run the parser.
    void runParser();

    //! restart the macaulay2 process in the shell.
    void restartShell();

private:
    MainWindow(QWidget *parent=0);
    MainWindow(const MainWindow&);
    MainWindow& operator= (const MainWindow&);
    KStatusBar *m_statusBar;

    MEditorManager* editorManager;

    void closeEvent(QCloseEvent *event);
    //! MainWindow instance.
    static MainWindow* instance;
    //! setup internal actions
    void setupActions();
    //! setup custom context menu.
    void setupContextMenu(KTextEditor::View*);
    //! connect signals
    void setupSignals();
    //! initialize and setup dock widgets
    void setupDockWidgets();
    //! initialize and setup editor and views
    void setupEditorDocsViews();

    //! Dock widget that holds the terminal.
    MProcessShellWidget *terminalDock;

    //! text hint widget.
    MAutocompleteWidget *textHintToolTip;

    //! currently active document.
    KTextEditor::Document *activeDoc;


    //! tab widget
    /*!
        the widget that holds all the tabs, one for each view/document.
    */
    KTabWidget *m_tab;

    //! Action to toogle the console widgets visibility
    KToggleAction *m_show_console;

    //! Action to toogle the filebrowser widgets visibility
    KToggleAction *m_show_filebrowser;

    //! Action to toogle the documentation widgets visibility
    KToggleAction *m_show_docviewer;

    //! Action to toogle macaulay2 output to new view
    KToggleAction *m_output_to_view;

    //! Filebrowser widget.
    FileBrowserWidget *filebrowser;
    //! Documentation viewer
    /*!
      QTextBrowser for browsing the Macaulay2 documentation.
    */
    MDocViewer *docViewer;

    KAction *actSendLine;
    KAction *actSendBlock;
    KAction *actSendDoc;

public slots:
    //! open url in the documentation viewer.
    void openUrl(const QUrl& url);
    //! first run.
    /*!
      different check to perform on the applications first run.
    */
    void firstRun();
};


#endif
