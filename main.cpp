#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

#include "MainWindow/mainwindow.h"
int main (int argc, char *argv[])
{
    KAboutData aboutData( "Macaulay2Editor", "M2E",
                          ki18n("Macaulay2Editor"), "0.001",
                          ki18n("Advanced text editor for Macaulay2"),
                          KAboutData::License_GPL,
                          ki18n("Copyright (c) 2010 Developer") );
    KCmdLineArgs::init( argc, argv, &aboutData );
    KApplication app;

    MainWindow* window = MainWindow::getInstance();//new MainWindow();
    window->show();
    return app.exec();
}
