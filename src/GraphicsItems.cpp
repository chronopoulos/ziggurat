#include "GraphicsItems.h"

#include <QDebug>

Cell::Cell(int step, int note) : QGraphicsRectItem() {

    m_step = step;
    m_note = note;

    setRect(0,0, CELLSIZE,CELLSIZE);
    setPos(step*CELLSIZE, (NNOTES-note-1)*CELLSIZE);

    setPen(QPen(Qt::gray));

}

int Cell::step(void) {

    return m_step;

}

int Cell::note(void) {

    return m_note;

}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent* e) {

    Trigger trig(m_step);
    trig.setNote(m_note);
    if (e->buttons() == Qt::LeftButton) {
        TrigRequest *tr = new TrigRequest(trig, this, true);
        emit trigRequested(tr);
    } else if (e->buttons() == Qt::RightButton) {
        TrigRequest *tr = new TrigRequest(trig, this, false);
        emit trigRequested(tr);
    }

}


////


SeqNameItem::SeqNameItem(void) : QGraphicsTextItem(QString()) {

    setTextInteractionFlags(Qt::TextEditorInteraction);

    setDefaultTextColor(QColor(Qt::gray));

};

void SeqNameItem::keyPressEvent(QKeyEvent *e) {

    // filter out return and tab
    switch (e->key()) {
        case Qt::Key_Return:
            clearFocus();
            emit nameChanged(this->toPlainText());
            break;
        case Qt::Key_Tab:
            // this doesn't seem to work?
            // check out: http://www.qtcentre.org/threads/
            // 19839-Customizing-QGraphicsTextItem-keyPressEvent-problem
            clearFocus();
            break;
        default:
            QGraphicsTextItem::keyPressEvent(e);
            break;
    }

};


////


BracketItem::BracketItem(int step, int nsteps, bool type) : QGraphicsPolygonItem() {

    m_nsteps = nsteps;
    m_type = type;

    QPolygonF triangle;
    if (m_type == BracketItem::Type_Left) {
        triangle.append(QPointF(0, -5));
        triangle.append(QPointF(5, 0));
        triangle.append(QPointF(0, 5));
        triangle.append(QPointF(0,-5));
    } else if (m_type == BracketItem::Type_Right) {
        triangle.append(QPointF(0, -5));
        triangle.append(QPointF(-5, 0));
        triangle.append(QPointF(0, 5));
        triangle.append(QPointF(0,-5));
    };

    setScale(0.6);
    setPolygon(triangle);
    setBrush(QBrush(QColor("black")));
    //m_step = step;
    //setPos(xFromStep(m_step), NNOTES*CELLSIZE+5);
    setStep(step);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

    setPen(QPen(Qt::gray));
    setBrush(QBrush(Qt::gray));

}

bool BracketItem::Type_Left = true;
bool BracketItem::Type_Right = false;

int BracketItem::xFromStep(int step)  {

    if (m_type == BracketItem::Type_Left) {
        return step*CELLSIZE + 3;
    } else if (m_type == BracketItem::Type_Right) {
        return step*CELLSIZE + 7;
    }

    return -1;

}

QVariant BracketItem::itemChange(GraphicsItemChange change, const QVariant &value) {

    if (change == ItemScenePositionHasChanged) {
        return QVariant();
    }

    if (change == ItemPositionChange) {
        newPos = value.toPointF();
        newPos.setY(oldPos.y()); // restrict to horizontal motion
        m_step = int(newPos.x())/10; // discretize x positioning
        if (m_step < 0) m_step = 0;
        if (m_step > m_nsteps-1) m_step = m_nsteps - 1;
        newPos.setX(xFromStep(m_step));
        return newPos;
    }

    return QGraphicsItem::itemChange(change, value);

}

void BracketItem::mousePressEvent(QGraphicsSceneMouseEvent*) {

    oldPos = pos();

}

void BracketItem::mouseReleaseEvent(QGraphicsSceneMouseEvent*) {

    emit stepChanged(m_step);

}

void BracketItem::setStep(int step) {

    m_step = step;
    oldPos = pos();
    setPos(xFromStep(m_step), NNOTES*CELLSIZE+5);
    return;

}
