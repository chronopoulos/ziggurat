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
