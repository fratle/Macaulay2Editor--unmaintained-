#ifndef MSETTINGS_H
#define MSETTINGS_H
#include <QtGui>
#include <KFileDialog>
#include <klistwidget.h>
#include "kcolorbutton.h"

class QComboBox;
class QLabel;
class QPushButton;
class QCheckBox;

class MSettings : public QDialog
{
    Q_OBJECT

public:
    //! Make a new MSettings
    /*!
      Constructs a MSettings Dialog

      If the parent is 0, the msettings becomes a new window. If the parent is another widget, this widget becomes
      a child window inside the parent. This window will be deleted when its parent is deleted.

      If you try to add a child widget to an already visible widget, you must explicitly show the child to make
      it visible.
    */
    MSettings ( QWidget *parent = 0 );

    //! Destroy the this widget

    /*!
      The children of this widget is destroyed first. If this widget is the main widget, the application will exit.
      */
    ~MSettings();

    //! Return the given location for the Macaulay2 executable file.
    const QString returnExecutable();

    //! Return the given location for the Macaulay2 installation file.
    const QString returnM2InstalFolder();

    //! Public autocomplete values and get methods
    bool MAutoCompleteOnOffValue;
    bool MAutoCompleteAutomaticValue;
    bool getAutoAutoCompletion();
    bool getAutocompletion();

    //! Write new settings given from the user, to a local config file.
    void writeSettings();

private slots:
    /*! Opens a new QFileDialog where the user can browse through the computer to find the location of the
        Macaulay2 installation folder.

        The user will start the search from home path of the system. The user can only search through folders,
        which means that files will not be shown in the new file dialog.
        */
    void browseM2InstallLocation();
    /*! Opens a new QFileDialog where the user can browse through the computer to find the folder location of the
        Macaulay2 executable file.

        The user will start the search from the home path of the system. The user can only search through folders,
        which means that files will not be shown in the new file dialog.
        */
    void browseExecLocation();
    //! This slot calls the writesettings() function.
    void saveChangestoConfig();
    //! This slot calls the writesettings() function, and terminates the settings dialog.
    void saveChangestoConfigWithClosing();
    //! build the database
    void buildDatabase();


private:
    //! A Member function for creating a new QPushButton.
    /*!
      \param text is the text given to the button.
      \param member is the slot that the button is taking.

      when creating a new button it is connected with a SIGNAL when clicked and a SLOT member to
      initialize what function the button should have when it is clicked.
      */
    QPushButton *createPushButton ( const QString &text, const char *member );
    //! A member function for creating a new QLineEdit.
    /*!
      \param text is a QString with the absolute path for the given folder. This path is gathered from QSettings config file.
      */
    QLineEdit *createLineEdit ( const QString &text );
    //! checkbox for automatic autocompletion
    QCheckBox *chk_autoAutoCompletion;
    //! checkbox for autocompletion
    QCheckBox *chk_autocompletion;
    //! button to recreate database
    QPushButton *reCreateDatabase;

    //! Read the executable information from the QSettings config file.
    /*!
      \return QString of the current location of the M2 executable.
      */
    const QString getExecFolder();
    //! Read the Macaulay2 installation information from the QSettings config file.
    /*!
      \return QString of the current location of the M2 installation folder.
      */
    const QString getM2InstallFolder();


    //! QLineEdit for showing the absolute path of the M2 installation folder
    QLineEdit *directoryEdit;
    //! QLineEdit for showing the absolute path of the M2 executable folder
    QLineEdit *executableEdit;

    //! QLabel which is a headline for the M2 installation folder search.
    QLabel *directoryL;
    //! QLabel which is a headline for the executable folder search.
    QLabel *executableL;

    //! QPushButton which enables the user to search for the M2 installation folder.
    QPushButton *browseFolderButton;
    //! QPushButton which enables the user to search for the M2 executable in a given folder.
    QPushButton *browseExecButton;
    //! QPushButton which applies the current QSettings.
    QPushButton *applyButton;
    //! QPushButton which applies the current QSettings, and terminates the dialog.
    QPushButton *okButton;
    //! QPushButton which closes the dialog.
    QPushButton *cancelButton;

    //! QString that contains the absolute path for the executable folder.
    QString mExecutable;
    //! QString that contains the absolute path for the Macaulay2 installation folder.
    QString mDirectory;

    bool hasExecutable;
    bool hasmDirectory;

};
#endif // MSETTINGS_H
