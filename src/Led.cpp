#include "Led.h"

#include <QDebug>

Led::Led(void) : QWidget() {

    isActive = false;
    hasPlayhead = false;

}

void Led::setActive(bool isActive) {

    this->isActive = isActive;

    update();
}

void Led::setPlayhead(bool hasPlayhead) {

    this->hasPlayhead = hasPlayhead;

    update();
}

void Led::paintEvent(QPaintEvent*) {

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if (isActive) {
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
    } else {
        painter.setPen(Qt::black);
        painter.setBrush(Qt::black);
    }

    qreal w(width());
    qreal h(height());
    QPointF center(w/2.,h/2.);

    painter.drawEllipse(center, LED_RADIUS, LED_RADIUS);

    if (hasPlayhead) painter.drawRect((w/2.-LED_RADIUS),0.95*h, 2*LED_RADIUS, h/16);

}


