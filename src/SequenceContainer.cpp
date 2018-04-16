#include "SequenceContainer.h"

#include <QDebug>

SequenceContainer::SequenceContainer(int nsteps, QString name) {

    seq = new Sequence(nsteps);
    thumb = new Thumbnail(nsteps);
    page = new ConfigPage(nsteps);
    row = new ButtonRow(nsteps);

    // interest
    QObject::connect(thumb, SIGNAL(interestRequested(void)),
                        this, SLOT(select(void)));

    // trig requests
    QObject::connect(row, SIGNAL(trigSet(int, Trigger*)), seq, SLOT(setTrig(int, Trigger*)));
    QObject::connect(seq, SIGNAL(stepActivationChanged(int, bool)),
                        thumb, SLOT(setActivation(int,bool)));

    // clock divide
    QObject::connect(page, SIGNAL(clockDivChanged(int)),
                        seq, SLOT(setClockDiv(int)));

    // transpose
    QObject::connect(page, SIGNAL(transposeChanged(int)),
                        seq, SLOT(setTranspose(int)));

    // MIDI Channel 
    QObject::connect(page, SIGNAL(midiChanChanged(int)),
                        seq, SLOT(setMidiChan(int)));

    // Direction
    QObject::connect(page, SIGNAL(directionChanged(QString)),
                        seq, SLOT(setDirection(QString)));

    // ticks
    QObject::connect(seq, SIGNAL(playheadUpdated(int)),
                        thumb, SLOT(updatePlayhead(int)));
    QObject::connect(seq, SIGNAL(playheadUpdated(int)),
                        row, SLOT(updatePlayhead(int)));

    // subloops
    QObject::connect(row, SIGNAL(lBracketChanged(int)),
                        seq, SLOT(setSubloop_start(int)));
    QObject::connect(row, SIGNAL(rBracketChanged(int)),
                        seq, SLOT(setSubloop_stop(int)));

    // mute and queue 
    connect(thumb, SIGNAL(muteChanged(bool)), seq, SLOT(setMute(bool)));
    connect(thumb, SIGNAL(queueChanged(bool)), seq, SLOT(setQueue(bool)));
    connect(seq, SIGNAL(muteChanged(bool)), thumb, SLOT(setMute(bool)));
    connect(seq, SIGNAL(queueChanged(bool)), thumb, SLOT(setQueue(bool)));

    // group hacks
    connect(seq, SIGNAL(muteChanged(bool)), this, SIGNAL(muteChanged_passthrough(bool)));
    connect(thumb, SIGNAL(muteChanged(bool)), this, SIGNAL(muteChanged_passthrough(bool)));
    connect(seq, SIGNAL(subloopCompleted(void)), this, SIGNAL(subloopCompleted_passthrough(void)));
    

    // name change
    QObject::connect(page, SIGNAL(nameChanged(QString)),
                        thumb, SLOT(setName(QString)));
    QObject::connect(page, SIGNAL(nameChanged(QString)),
                        seq, SLOT(setName(QString)));

    // delete
    QObject::connect(thumb, SIGNAL(deleteRequested(void)),
                        this, SLOT(routeDelete(void)));

    page->setName(name); // propagates to thumbnail
    seq->reset(); // call this to set the thumbnail playhead

}

SequenceContainer::~SequenceContainer(void) {

    delete seq;
    delete thumb;
    delete page;
    delete row;

}

void SequenceContainer::select(void) {

    emit pageSelected(page);
    emit thumbnailSelected(thumb);
    emit rowSelected(row);

}

bool SequenceContainer::selected(void) {

    return thumb->hasInterest();

}

void SequenceContainer::routeDelete(void) {

    emit deleteRequested(this);

}

void SequenceContainer::setMute(bool mute) {

    thumb->setMute(mute);
    seq->setMute(mute);

}
