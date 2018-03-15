#include "SequenceContainer.h"

#include <QDebug>

SequenceContainer::SequenceContainer(int nsteps, QString name) {

    seq = new Sequence(nsteps);
    thumb = new Thumbnail(nsteps);
    canv = new Canvas(nsteps);
    page = new ConfigPage(nsteps);

    // interest
    QObject::connect(thumb, SIGNAL(interestRequested(void)),
                        this, SLOT(select(void)));

    // trig requests
    QObject::connect(canv, SIGNAL(trigRequested(TrigRequest*)),
                        seq, SLOT(handleTrigRequest(TrigRequest*)));
    QObject::connect(seq, SIGNAL(trigRequestAccepted(TrigRequest*)),
                        canv, SLOT(registerTrigRequest(TrigRequest*)));
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
                        canv, SLOT(updatePlayhead(int)));

    // subloops
    QObject::connect(canv->lBracket, SIGNAL(stepChanged(int)),
                        seq, SLOT(setSubloop_start(int)));
    QObject::connect(canv->rBracket, SIGNAL(stepChanged(int)),
                        seq, SLOT(setSubloop_stop(int)));
    QObject::connect(seq, SIGNAL(subloopChanged(int, int)),
                        canv, SLOT(setBrackets(int,int)));

    // enabling
    QObject::connect(thumb, SIGNAL(enablingChanged(bool)),
                        seq, SLOT(setEnabling(bool)));

    // name change
    QObject::connect(page, SIGNAL(nameChanged(QString)),
                        thumb, SLOT(setName(QString)));

    // delete
    QObject::connect(thumb, SIGNAL(deleteRequested(void)),
                        this, SLOT(routeDelete(void)));

    page->setName(name); // propagates to thumbnail
    seq->reset(); // call this to set the thumbnail playhead

}

SequenceContainer::~SequenceContainer(void) {

    delete seq;
    delete thumb;
    delete canv;
    delete page;

}

void SequenceContainer::select(void) {

    emit canvasSelected(canv);
    emit pageSelected(page);
    emit thumbnailSelected(thumb);

}

bool SequenceContainer::selected(void) {

    return thumb->hasInterest();

}

void SequenceContainer::routeDelete(void) {

    emit deleteRequested(this);

}
