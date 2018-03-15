/*
[ziggurat]
Chris Chronopoulos 20180226
*/

#include <QApplication>

#include "MainWindow.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
