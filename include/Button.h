#ifndef BUTTONCELL_H
#define BUTTONCELL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>

#include "Trigger.h"

class Button : public QWidget
{
    Q_OBJECT

    public:
        Button(int);

    private:
        bool m_isActive;
        int m_step;
        Trigger *m_trig;

    protected:
        void mousePressEvent(QMouseEvent*);
        void paintEvent(QPaintEvent*);

    signals:
        void trigSet(int, Trigger*);

};

class Indicator : public QWidget
{
    Q_OBJECT

    public:
        Indicator(void);
        void setPlayhead(bool);
        void setLBracket(bool);
        void setRBracket(bool);

    private:
        bool m_hasPlayhead;
        bool m_hasLBracket;
        bool m_hasRBracket;
        bool m_hasLBracketGhost;
        bool m_hasRBracketGhost;

    protected:
        void paintEvent(QPaintEvent*);
        void mouseMoveEvent(QMouseEvent*);
        void dragEnterEvent(QDragEnterEvent*);
        void dragLeaveEvent(QDragLeaveEvent*);
        void dropEvent(QDropEvent*);

};

#endif
