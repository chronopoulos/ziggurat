#include "Canvas.h"

#include <QDebug>

Canvas::Canvas(int nsteps) : QGraphicsScene() {

    this->nsteps = nsteps;

    setBackgroundBrush(QColor(Qt::black));

    if (nsteps > 0) {

        // make a grid of cells
        for (int step=0; step<nsteps; step++) {
            for (int note=0; note<NNOTES; note++) {
                Cell *cell = new Cell(step, note);
                addItem(cell);
                QObject::connect(cell, SIGNAL(trigRequested(TrigRequest*)),
                                    this, SIGNAL(trigRequested(TrigRequest*)));
            }
        }

        // make a playhead
        playhead = addRect(0,0, CELLSIZE, CELLSIZE/10);
        playhead->setPos(0, (NNOTES+1)*CELLSIZE);
        playhead->setPen(QPen(Qt::gray));

        // make subloop brackets
        lBracket = new BracketItem(0, nsteps, BracketItem::Type_Left);
        addItem(lBracket);
        rBracket = new BracketItem(nsteps-1, nsteps, BracketItem::Type_Right);
        addItem(rBracket);

    } else { // default canvas

        defaultText = new QGraphicsTextItem("No Sequence Selected");
        defaultText->setDefaultTextColor(QColor(Qt::gray));
        addItem(defaultText);

    }

}

void Canvas::registerTrigRequest(TrigRequest* tr) {

    if (tr->request()==TrigRequest::Add) {
        tr->cell()->setBrush(QBrush(QColor("red")));
    } else if (tr->request()==TrigRequest::Remove) {
        tr->cell()->setBrush(QBrush(Qt::NoBrush));
    }

}

void Canvas::updatePlayhead(int step) {

    playhead->setPos(step*CELLSIZE, (NNOTES+1)*CELLSIZE);

}

void Canvas::setBrackets(int lstep, int rstep) {

    lBracket->setStep(lstep);
    rBracket->setStep(rstep);

}
