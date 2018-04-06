#include "ButtonCell.h"

#include <QDebug>

Button::Button(bool dummy) {

    setCheckable(!dummy);
    setFocusPolicy(Qt::NoFocus);

}

Indicator::Indicator() {

    m_hasPlayhead = false;
    m_hasLBracket= false;
    m_hasRBracket= false;

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
        painter.drawRect(0.1*w, 0.4*h, 0.8*w, 0.2*h);
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

ButtonCell::ButtonCell(bool dummy) {

    QVBoxLayout *layout = new QVBoxLayout();

    button = new Button(dummy);
    indicator = new Indicator();

    layout->addWidget(button);
    layout->addWidget(indicator);

    setLayout(layout);

}

void ButtonCell::setPlayhead(bool hasPlayhead) {

    indicator->setPlayhead(hasPlayhead);

}

void ButtonCell::setLBracket(bool hasLBracket) {

    indicator->setLBracket(hasLBracket);

}

void ButtonCell::setRBracket(bool hasRBracket) {

    indicator->setRBracket(hasRBracket);

}

