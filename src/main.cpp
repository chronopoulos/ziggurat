/*

    [ziggurat]

    Chris Chronopoulos 20180226

*/

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QString>

#include "MainWindow.h"

#include <QDebug>

QString MIDI_BACKEND;
bool DELTA = false;

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    app.setApplicationName("ziggurat");
    app.setApplicationVersion("0.0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption jackOption(QStringList() << "j" << "jack-midi", "Use JACK MIDI");
    parser.addOption(jackOption);

    QCommandLineOption alsaOption(QStringList() << "a" << "alsa-midi", "Use ALSA MIDI (default)");
    parser.addOption(alsaOption);

    QCommandLineOption fileOption(QStringList() << "f" << "file",
            "Open session file <filename>", "filename");
    parser.addOption(fileOption);

    parser.process(app);

    bool jackMidi = parser.isSet(jackOption);
    bool alsaMidi = parser.isSet(alsaOption);
    QString filename = parser.value(fileOption);

    if (jackMidi && !alsaMidi) {
        MIDI_BACKEND = "jack";
    } else {
        MIDI_BACKEND = "alsa";
    }

    MainWindow mainWindow(filename);
    mainWindow.show();

    return app.exec();

}
