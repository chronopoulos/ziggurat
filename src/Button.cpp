#include "Button.h"

#include <QDebug>
#include <QPalette>
#include <QDrag>
#include <QMimeData>

#include <unistd.h>

Button::Button(int step) {

    m_step = step;

    QPalette palette = QPalette();
    palette.setColor(QPalette::Background, QColor(100, 100, 100));
    setAutoFillBackground(true);
    setPalette(palette);

    setMinimumSize(50,50);
    setMaximumSize(50,50);

    m_isActive = false;

    m_trig = new Trigger(m_step);    

}

void Button::mousePressEvent(QMouseEvent*) {

    m_isActive = !m_isActive;

    if (m_isActive) {
        m_trig->setNote(60); // temporary default
    } else {
        m_trig->setNull();
    }

    emit trigSet(m_step, m_trig);

    update();
}

void Button::paintEvent(QPaintEvent*) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);

    qreal w,h;
    w = width();
    h = height();

    // led
    if (m_isActive) {
        painter.setBrush(Qt::red);
    } else {
        painter.setBrush(Qt::black);
    }
    painter.drawRect(0.3*w, 0.1*h, 0.4*w, 0.2*h);

}


Indicator::Indicator() {

    m_hasPlayhead = false;
    m_hasLBracket = false;
    m_hasRBracket = false;
    m_hasLBracketGhost = false;
    m_hasRBracketGhost = false;

    setMinimumHeight(30);
    setMaximumHeight(30);

    setAcceptDrops(true);

}

void Indicator::setPlayhead(bool hasPlayhead) {

    m_hasPlayhead = hasPlayhead;
    update();

}

void Indicator::setLBracket(bool hasLBracket) {

    m_hasLBracket = hasLBracket;
    update();

}

void Indicator::setRBracket(bool hasRBracket) {

    m_hasRBracket = hasRBracket;
    update();

}

void Indicator::paintEvent(QPaintEvent*) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    qreal w,h;
    w = width();
    h = height();

    if (m_hasPlayhead) {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::black);
        painter.drawRect(0.1*w, 0.8*h, 0.8*w, 0.1*h);
    }

    if (m_hasLBracket) {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::black);
        QPolygonF triangle;
        triangle.append(QPointF(0, -5));
        triangle.append(QPointF(5, 0));
        triangle.append(QPointF(0, 5));
        triangle.append(QPointF(0,-5));
        triangle.translate(w/2 - 5, h/2);
        painter.drawPolygon(triangle);
    }

    if (m_hasRBracket) {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::black);
        QPolygonF triangle;
        triangle.append(QPointF(0, -5));
        triangle.append(QPointF(-5, 0));
        triangle.append(QPointF(0, 5));
        triangle.append(QPointF(0,-5));
        triangle.translate(w/2 + 5, h/2);
        painter.drawPolygon(triangle);
    }

    if (m_hasLBracketGhost) {
        painter.setPen(Qt::gray);
        painter.setBrush(Qt::gray);
        QPolygonF triangle;
        triangle.append(QPointF(0, -5));
        triangle.append(QPointF(5, 0));
        triangle.append(QPointF(0, 5));
        triangle.append(QPointF(0,-5));
        triangle.translate(w/2 - 5, h/2);
        painter.drawPolygon(triangle);
    }

    if (m_hasRBracketGhost) {
        painter.setPen(Qt::gray);
        painter.setBrush(Qt::gray);
        QPolygonF triangle;
        triangle.append(QPointF(0, -5));
        triangle.append(QPointF(-5, 0));
        triangle.append(QPointF(0, 5));
        triangle.append(QPointF(0,-5));
        triangle.translate(w/2 + 5, h/2);
        painter.drawPolygon(triangle);
    }


}

void Indicator::mouseMoveEvent(QMouseEvent *e) {
    /*
    this event handler starts the drag operation to move brackets around
    if an indicator has BOTH brackets, then the LBracket is selected
    */

    if (e->buttons() == Qt::LeftButton) {
        if (m_hasLBracket || m_hasRBracket) {

            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;

            if (m_hasLBracket) {
                mimeData->setText("L");
            } else if (m_hasRBracket) {
                mimeData->setText("R");
            }
            drag->setMimeData(mimeData);

            setAcceptDrops(false);
            Qt::DropAction dropAction = drag->exec();
            if (dropAction == Qt::MoveAction) {
                if (mimeData->text() == "L") {
                    m_hasLBracket = false;
                } else if (mimeData->text() == "R") {
                    m_hasRBracket = false;
                }
                update();
            }
            setAcceptDrops(true);

        }
    }

}

void Indicator::dragEnterEvent(QDragEnterEvent *e) {

    if (e->mimeData()->hasFormat("text/plain")) {
        if (e->mimeData()->text() == "L") {
            m_hasLBracketGhost = true;
            update();
            e->accept();
        } else if (e->mimeData()->text() == "R") {
            m_hasRBracketGhost = true;
            update();
            e->accept();
        }
    }

}

void Indicator::dragLeaveEvent(QDragLeaveEvent *e) {

    m_hasLBracketGhost = false;
    m_hasRBracketGhost = false;

    update();
    e->accept();

}

void Indicator::dropEvent(QDropEvent *e) {

    if (e->mimeData()->hasFormat("text/plain")) {

        if (e->mimeData()->text() == "L") {
            m_hasLBracket= true;
        } else if (e->mimeData()->text() == "R") {
            m_hasRBracket= true;
        }

        m_hasLBracketGhost = false;
        m_hasRBracketGhost = false;

        update();
        e->accept();

    }

}
