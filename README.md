# ziggurat
a standalone MIDI step sequencer

![Ziggurat Screenshot](https://static1.squarespace.com/static/54d8093fe4b02ef1156307b5/t/5ad45c9a03ce641bc8be47c2/1523866799332/ziggurat_20180416.png)

## Build Instructions

First, install dependencies:
```
sudo apt-get install qt5-default libasound2-dev libjack-jackd2-dev
```

Then, clone the source code and initialize the submodule:
```
git clone https://github.com/chronopoulos/ziggurat
cd ziggurat
git submodule init && git submodule update
```

NOTE: If your version of libjack-jackd2-dev is older than 1.9.11, then you
should comment out the line containing -DJACK_HAS_PORT_RENAME in ziggurat.pro

Finally, compile and run:

```
qmake
make
./bin/ziggurat
```

## Basic Usage

Ziggurat starts up with 3 empty groups, represented by the 3 rectangles near the
top of the main window. To add a sequence, right-click on a group, choose "Add
Sequence", choose a sequence name and length and click "OK".

Along the bottom of the window, you will find the Button-Row Editor, where you
can enter notes by clicking on the buttons. The default note has MIDI value 60
(middle C) and velocity 100. To change this, mouse-wheel over the button;
holding Shift will increase the rate of change. You can switch between editing
note value or velocity using the dropdown menu in the Row Editor.

In the Transport section (middle-left), you can set the tempo, start,
stop, and pause the sequencer. Hitting spacebar toggles start/stop. Each
sequence has a set of parameters (Name, Tranpose, MIDI channel, etc.) which can
be edited in the Config section (middle-right).

Sessions can be saved (Ctrl-S) to project files, which can later be re-opened
(Ctrl-O). The file format is stadard JSON, with file extension \*.zig
