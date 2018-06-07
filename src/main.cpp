/*

    [ziggurat]

    Chris Chronopoulos 20180226

*/

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QString>

#include "MainWindow.h"
#include "Delta.h"

#include <QDebug>

// globals
QString MIDI_BACKEND;
Delta DELTA;

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    app.setApplicationName("ziggurat");
    app.setApplicationVersion("0.0.1");

    QFont font("Monospace");
    font.setBold(true);
    font.setPointSize(10);
    app.setFont(font);

    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("filename", "Ziggurat session file to load");

    QCommandLineOption jackOption(QStringList() << "j" << "jack-midi", "Use JACK MIDI");
    parser.addOption(jackOption);

    QCommandLineOption alsaOption(QStringList() << "a" << "alsa-midi", "Use ALSA MIDI (default)");
    parser.addOption(alsaOption);

    parser.process(app);

    bool jackMidi = parser.isSet(jackOption);
    bool alsaMidi = parser.isSet(alsaOption);

    const QStringList args = parser.positionalArguments();
    QString filename;
    if (args.count() > 0) {
        filename = args[0];
    }

    if (jackMidi && !alsaMidi) {
        MIDI_BACKEND = "jack";
    } else {
        MIDI_BACKEND = "alsa";
    }

    MainWindow mainWindow(filename);
    mainWindow.show();

    return app.exec();

}
