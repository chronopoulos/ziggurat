#include "Button.h"

#include <QDebug>
#include <QPalette>

Button::Button(int step) {

    m_step = step;

    QPalette palette = QPalette();
    palette.setColor(QPalette::Background, QColor(100, 100, 100));
    setAutoFillBackground(true);
    setPalette(palette);

    setMinimumSize(50,50);
    setMaximumSize(50,50);

    m_isActive = false;

}

void Button::mousePressEvent(QMouseEvent *e) {

    if (e->buttons() == Qt::LeftButton) m_isActive = !m_isActive;

    if (m_isActive) {
        m_trig.setNote(60); // default value
    } else {
        m_trig.setNull();
    }

    emit trigSet(m_step, &m_trig);
    update();

}

void Button::wheelEvent(QWheelEvent *e) {

    if (m_isActive) {

        int note = m_trig.note();
        if (e->angleDelta().y() > 0) {
            m_trig.setNote(note + 1);
        } else if (e->angleDelta().y() < 0) {
            m_trig.setNote(note - 1);
        }

        emit trigSet(m_step, &m_trig);
        update();

    }

}

void Button::setTrig(Trigger trig) {

    m_trig = trig;

    if (m_trig.type() == Trigger::Type_Null) {
        m_isActive = false;
    } else {
        m_isActive = true;
    }

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
        painter.drawRect(0.3*w, 0.1*h, 0.4*w, 0.2*h);
        painter.drawText(QRect(0.3*w,0.5*h,0.4*w,0.2*h), Qt::AlignCenter,
                            QString::number(m_trig.note()));
    } else {
        painter.setBrush(Qt::black);
        painter.drawRect(0.3*w, 0.1*h, 0.4*w, 0.2*h);
    }

}


Indicator::Indicator(int step) {

    m_step = step;

    m_hasPlayhead = false;
    m_hasLBracket= false;
    m_hasRBracket= false;

    setMinimumHeight(30);
    setMaximumHeight(30);

}

void Indicator::setPlayhead(bool hasPlayhead) {

    m_hasPlayhead = hasPlayhead;
    update();

}

void Indicator::setLBracket(bool hasLBracket) {

    if (hasLBracket == m_hasLBracket) return;

    m_hasLBracket = hasLBracket;
    update();

    if (m_hasLBracket) emit lBracketSet(m_step);

}

void Indicator::setRBracket(bool hasRBracket) {

    if (hasRBracket == m_hasRBracket) return;

    m_hasRBracket = hasRBracket;
    update();

    if (m_hasRBracket) emit rBracketSet(m_step);

}

void Indicator::paintEvent(QPaintEvent*) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);

    qreal w,h;
    w = width();
    h = height();

    if (m_hasPlayhead) {
        painter.drawRect(0.1*w, 0.8*h, 0.8*w, 0.1*h);
    }

    if (m_hasLBracket) {
        QPolygonF triangle;
        triangle.append(QPointF(0, -5));
        triangle.append(QPointF(5, 0));
        triangle.append(QPointF(0, 5));
        triangle.append(QPointF(0,-5));
        triangle.translate(w/2 - 5, h/2);
        painter.drawPolygon(triangle);
    }

    if (m_hasRBracket) {
        QPolygonF triangle;
        triangle.append(QPointF(0, -5));
        triangle.append(QPointF(-5, 0));
        triangle.append(QPointF(0, 5));
        triangle.append(QPointF(0,-5));
        triangle.translate(w/2 + 5, h/2);
        painter.drawPolygon(triangle);
    }

}

void Indicator::mousePressEvent(QMouseEvent *e) {

    if (e->buttons() == Qt::LeftButton) {
        setLBracket(true);
    } else if (e->buttons() == Qt::RightButton) {
        setRBracket(true);
    }

}
