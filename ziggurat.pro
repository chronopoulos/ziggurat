######################################################################
# Automatically generated by qmake (3.1) Thu Feb 8 18:42:49 2018
######################################################################

TEMPLATE = app
TARGET = ziggurat

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Directory structure
VPATH += src include
INCLUDEPATH += include
OBJECTS_DIR = build
MOC_DIR = build
DESTDIR = bin

# Extra flags
QMAKE_CXXFLAGS += -D__LINUX_ALSA__ # needed for RtMidi "dummy" error
LIBS += -lasound
QMAKE_CXXFLAGS += -D__UNIX_JACK__ # needed for RtMidi "dummy" error
LIBS += -ljack
QMAKE_CXXFLAGS += -std=gnu++0x # needed for RtMidi "dummy" error

# Input
SOURCES += main.cpp
SOURCES += MainWindow.cpp
SOURCES += ManagerWidget.cpp
SOURCES += EditorWidget.cpp
SOURCES += Sequence.cpp
SOURCES += Thumbnail.cpp
SOURCES += Canvas.cpp
SOURCES += Led.cpp
SOURCES += Trigger.cpp
SOURCES += TrigRequest.cpp
SOURCES += GraphicsItems.cpp
SOURCES += Configurator.cpp
SOURCES += TransportWidget.cpp
SOURCES += ConfigPage.cpp
SOURCES += SequenceContainer.cpp
SOURCES += ClickLabel.cpp
SOURCES += Dialogs.cpp
SOURCES += RtMidi.cpp

HEADERS += MainWindow.h
HEADERS += ManagerWidget.h
HEADERS += EditorWidget.h
HEADERS += Sequence.h
HEADERS += Thumbnail.h
HEADERS += Canvas.h
HEADERS += Led.h
HEADERS += Trigger.h
HEADERS += TrigRequest.h
HEADERS += GraphicsItems.h
HEADERS += Configurator.h
HEADERS += TransportWidget.h
HEADERS += ConfigPage.h
HEADERS += SequenceContainer.h
HEADERS += ClickLabel.h
HEADERS += Dialogs.h
HEADERS += RtMidi.h

# vim:syntax=sh