#include "ButtonCell.h"

#include <QDebug>

Button::Button(bool dummy) {

    setCheckable(!dummy);
    setFocusPolicy(Qt::NoFocus);

}

Indicator::Indicator() {

    m_hasPlayhead = false;

}

void Indicator::setPlayhead(bool hasPlayhead) {

    m_hasPlayhead = hasPlayhead;

    update();

}

void Indicator::paintEvent(QPaintEvent*) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);


    qreal w,h;
    if (m_hasPlayhead) {
        w = width();
        h = height();
        painter.drawRect(0.1*w, 0.4*h, 0.8*w, 0.2*h);
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

