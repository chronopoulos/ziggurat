#include "SequenceContainer.h"

#include <QDebug>

SequenceContainer::SequenceContainer(int nsteps, QString name) {

    seq = new Sequence(nsteps);
    thumb = new Thumbnail(nsteps);
    page = new ConfigPage(nsteps);
    row = new ButtonRow(nsteps);

    makeConnections();
    page->setName(name); // propagates to thumbnail and sequence
    seq->reset(); // call this to set the thumbnail playhead

}

// JSON constructor
SequenceContainer::SequenceContainer(const QJsonObject &seqJSO) {

    seq = new Sequence(seqJSO);
    thumb = new Thumbnail(seqJSO);
    page = new ConfigPage(seqJSO);
    row = new ButtonRow(seqJSO);

    makeConnections();
    seq->reset();

}


// pseudo-copy constructor
SequenceContainer::SequenceContainer(SequenceContainer *scont) {

    seq = new Sequence(scont->seq);
    thumb = new Thumbnail(scont->seq);
    page = new ConfigPage(scont->seq);
    row = new ButtonRow(scont->seq);

    makeConnections();
    seq->reset();

}

SequenceContainer::~SequenceContainer(void) {

    delete seq;
    delete thumb;
    delete page;
    delete row;

}

void SequenceContainer::makeConnections(void) {

    // selection
    connect(thumb, SIGNAL(selected(void)), this, SLOT(select(void)));

    // trig requests
    connect(row, SIGNAL(trigSet(int, Trigger*)), seq, SLOT(setTrig(int, Trigger*)));
    connect(seq, SIGNAL(stepActivationChanged(int, bool)), thumb, SLOT(setActivation(int,bool)));

    // clock divide
    connect(page, SIGNAL(clockDivChanged(int)), seq, SLOT(setClockDiv(int)));

    // transpose
    connect(page, SIGNAL(transposeChanged(int)),
                        seq, SLOT(setTranspose(int)));

    // MIDI Channel 
    connect(page, SIGNAL(midiChanChanged(int)), seq, SLOT(setMidiChan(int)));

    // Direction
    connect(page, SIGNAL(directionChanged(QString)), seq, SLOT(setDirection(QString)));

    // ticks
    connect(seq, SIGNAL(playheadUpdated(int)), thumb, SLOT(updatePlayhead(int)));
    connect(seq, SIGNAL(playheadUpdated(int)), row, SLOT(updatePlayhead(int)));

    // subloops
    connect(row, SIGNAL(lBracketChanged(int)), seq, SLOT(setSubloop_start(int)));
    connect(row, SIGNAL(rBracketChanged(int)), seq, SLOT(setSubloop_stop(int)));

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
    connect(page, SIGNAL(nameChanged(QString)), thumb, SLOT(setName(QString)));
    connect(page, SIGNAL(nameChanged(QString)), seq, SLOT(setName(QString)));

    // delete
    connect(thumb, SIGNAL(deleteRequested(void)), this, SLOT(routeDelete(void)));

}

void SequenceContainer::select(void) {

    thumb->select();
    emit pageSelected(page);
    emit thumbnailSelected(thumb);
    emit rowSelected(row);

}

bool SequenceContainer::selected(void) {

    return thumb->isSelected;

}

void SequenceContainer::routeDelete(void) {

    emit deleteRequested(this);

}

void SequenceContainer::setMute(bool mute) {

    thumb->setMute(mute);
    seq->setMute(mute);

}

void SequenceContainer::setQueue(bool queue) {

    thumb->setQueue(queue);
    seq->setQueue(queue);

}

void SequenceContainer::toggleMute(void) {

    setMute(!seq->isMuted());

}

void SequenceContainer::toggleQueue(void) {

    setQueue(!seq->isQueued());

}
