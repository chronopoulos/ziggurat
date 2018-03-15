#ifndef LED_H
#define LED_H

#include <QWidget>
#include <QPainter>
#include <QPaintEngine> // todo remove
#include <QPaintEvent>

#define LED_RADIUS 3

class Led : public QWidget
{
    Q_OBJECT

    public:
        Led(void);
        void setActive(bool);
        void setPlayhead(bool);

    private:
        bool isActive;
        bool hasPlayhead;

    protected:
        void paintEvent(QPaintEvent*);
};

#endif
