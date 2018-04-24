/*
[ziggurat]
Chris Chronopoulos 20180226
*/

#include <QApplication>
#include <QString>

#include "MainWindow.h"

QString MIDI_BACKEND;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // MIDI backend selection (default is ALSA)
    QStringList args = app.arguments();
    if (args.contains(QString("--jack-midi"))) {
        MIDI_BACKEND = "jack";
    } else {
        MIDI_BACKEND = "alsa";
    }

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
