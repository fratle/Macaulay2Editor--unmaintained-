#include "msettings.h"
#include "MainWindow/mainwindow.h"
#include "MParser/mparser.h"
MSettings::MSettings ( QWidget *parent ) : QDialog ( parent )
{
    // Setting the name and location of the QSettings file in home/.config
    QCoreApplication::setOrganizationName ( "Macaulay2" );
    QCoreApplication::setApplicationName ( "Macaulay2Editor" );

    browseFolderButton = createPushButton ( tr ( "&Browse..." ), SLOT ( browseM2InstallLocation ( void ) ) );
    browseExecButton = createPushButton ( tr ( "&Browse..." ), SLOT ( browseExecLocation ( void ) ) );
    applyButton = createPushButton ( tr ( "&Apply" ), SLOT ( saveChangestoConfig ( void ) ) );
    okButton = createPushButton ( tr ( "&Ok" ), SLOT ( saveChangestoConfigWithClosing ( void ) ) );
    cancelButton = createPushButton ( tr ( "&Cancel" ), SLOT ( close() ) );

    reCreateDatabase = createPushButton ( tr ( "&Recreate database" ), SLOT ( buildDatabase ( void ) ) );

    chk_autoAutoCompletion = new QCheckBox ( "Automatic autocompletion", this );
    //Creates a checkbox, checked or unchecked based on settings value.
    chk_autoAutoCompletion->setChecked ( getAutoAutoCompletion() );

    chk_autocompletion = new QCheckBox ( "Autocompletion", this );
    chk_autocompletion->setChecked ( getAutocompletion() );

    //automatic autocompletion (hidden if autocompletion is off)
    if ( getAutocompletion() )
    {
        chk_autoAutoCompletion->setVisible ( true );
    }
    else
    {
        chk_autoAutoCompletion->setVisible ( false );
    }
    connect ( chk_autocompletion, SIGNAL ( clicked ( bool ) ),chk_autoAutoCompletion, SLOT ( setVisible ( bool ) ) );
    //connect(checkbox2, SIGNAL(clicked(bool)),checkbox, SLOT(setChecked(bool)));

    hasmDirectory = !getExecFolder().isEmpty();
    hasExecutable = !getM2InstallFolder().isEmpty();

    executableEdit = createLineEdit ( getExecFolder() );
    directoryEdit = createLineEdit ( getM2InstallFolder() );
    directoryL = new QLabel ( tr ( "Macaulay2 lib directory : " ) );
    directoryL->setToolTip ( "example: /usr/lib/Macaulay2 on ubuntu" );
    executableL = new QLabel ( tr ( "Macaulay2 Executable File in Directory : " ) );

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget ( okButton );
    buttonsLayout->addWidget ( applyButton );
    buttonsLayout->addWidget ( cancelButton );

    QHBoxLayout *autocompleteLayout = new QHBoxLayout;
    autocompleteLayout->addWidget ( chk_autocompletion );
    autocompleteLayout->addWidget ( chk_autoAutoCompletion );

    QHBoxLayout *recreatelayout = new QHBoxLayout;
    recreatelayout->addWidget ( reCreateDatabase );

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout ( autocompleteLayout,1,0,0 );
    mainLayout->addWidget ( directoryL, 2, 0 );
    mainLayout->addWidget ( directoryEdit, 3, 0 );
    mainLayout->addWidget ( browseFolderButton, 3, 1 );
    mainLayout->addWidget ( executableL, 4, 0 );
    mainLayout->addWidget ( executableEdit, 5, 0 );
    mainLayout->addWidget ( browseExecButton, 5, 1 );
    mainLayout->addLayout ( recreatelayout,6, 0 );
    mainLayout->addLayout ( buttonsLayout, 7, 1 );
    setLayout ( mainLayout );

    setWindowTitle ( tr ( "Settings for Macaulay2 in Macualay2Editor" ) );
    int widht = this->x();
    int height = this->y();
    setFixedSize ( widht, height );
}

MSettings::~MSettings()
{

}

/*
 * Member function for creating a new QPushButton.
 */
QPushButton *MSettings::createPushButton ( const QString &text, const char *member )
{
    QPushButton *button = new QPushButton ( text );
    connect ( button, SIGNAL ( clicked() ), this, member );
    return button;
}
/*
 * Member function for creating a new QComboBox.
 */
QLineEdit *MSettings::createLineEdit ( const QString &text )
{
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setText ( text );

    lineEdit->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Preferred );
    return lineEdit;
}


void MSettings::browseM2InstallLocation()
{
    QString directory = QFileDialog::getExistingDirectory ( this, tr ( "Directory for Macaulay2 installation" ),
                        QDir::homePath(), QFileDialog::DontUseNativeDialog|QFileDialog::ShowDirsOnly );

    if ( !directory.isEmpty() )
    {
        directoryEdit->setText ( directory );
    }
    mDirectory = directory;
}

void MSettings::browseExecLocation()
{
    QString executable = QFileDialog::getOpenFileName ( this, tr ( "Choose directoryEdit of M2 Executable" ),  QDir::homePath() );
    if ( !executable.isEmpty() )
    {
        executableEdit->setText ( executable );
    }

    mExecutable = executable;
}

const QString MSettings::returnExecutable()
{
    return mExecutable;
}
const QString MSettings::returnM2InstalFolder()
{
    return mDirectory;
}

void MSettings::saveChangestoConfig()
{
    writeSettings();
    close();
}
void MSettings::saveChangestoConfigWithClosing()
{
    saveChangestoConfig();
    this->close();
}

void MSettings::writeSettings()
{
    mExecutable = executableEdit->text();
    mDirectory = directoryEdit->text();
    //saves the automatic autocompletion value in the settings file, based on the checked status of the settings checkbox.
    QSettings settings;
    settings.beginGroup ( "Autocomplete" );
    settings.setValue ( "automatic", chk_autoAutoCompletion->isChecked() );
    //Connects and runs a method in the main function that changes the actual current status of the automatic autocompletion.
    MainWindow *main = MainWindow::getInstance();
    //main->setAutomaticAutocomplete();

    settings.setValue ( "onoff", chk_autocompletion->isChecked() );
    //Calls the main function for disabling or enabling the current autocompletion status.
    //main->setAutocompletionOnOff();

    settings.endGroup();

    settings.beginGroup ( "MainWindow" );
    settings.setValue ( "Executable", mExecutable );
    settings.setValue ( "Directory", mDirectory );
    settings.setValue ( "Completer", MAutoCompleteOnOffValue );
    settings.endGroup();

    if ( !hasExecutable && !executableEdit->text().isEmpty() )
    {
        qDebug() << hasExecutable << ":" << executableEdit->text();
        main->restartShell();
    }
    if ( !hasmDirectory && !directoryEdit->text().isEmpty() )
    {
        qDebug() << hasmDirectory << ":" << directoryEdit->text();
        main->runParser();
    }
}
const QString MSettings::getExecFolder()
{
    QSettings settings;
    mExecutable =  settings.value ( "MainWindow/Executable" ).toString();
    return mExecutable;
}
const QString MSettings::getM2InstallFolder()
{
    QSettings settings;
    mDirectory =  settings.value ( "MainWindow/Directory" ).toString();
    return mDirectory;
}
bool MSettings::getAutoAutoCompletion()
{
    QSettings settings;
    MAutoCompleteAutomaticValue = settings.value ( "Autocomplete/automatic" ).toBool();
    return MAutoCompleteAutomaticValue;
}
bool MSettings::getAutocompletion()
{
    QSettings settings;
    MAutoCompleteOnOffValue = settings.value ( "Autocomplete/onoff" ).toBool();
    return MAutoCompleteOnOffValue;
}
void MSettings::buildDatabase()
{
    QMessageBox msgBox;
    msgBox.setText ( "Please wait, this could take some time" );
    msgBox.exec();
    MainWindow *main = MainWindow::getInstance();
    MParser *parser = new MParser();
    connect ( parser, SIGNAL ( finishedHtmlLinks() ), main, SLOT ( parserFinished() ) );
    parser->parse();
}
