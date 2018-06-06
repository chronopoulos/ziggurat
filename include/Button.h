#ifndef BUTTONCELL_H
#define BUTTONCELL_H

#include <QFrame>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>

#include "Trigger.h"

class Button : public QFrame
{
    Q_OBJECT

    public:
        Button(int);
        void setTrig(Trigger);
        void toggle(void);
        void setEditParameter(int);

        static int Edit_NoteValue;
        static int Edit_NoteVelocity;

        void setPhocus(bool);

    private:
        bool m_isActive;
        int m_step;
        Trigger m_trig;
        int m_editParameter;

        // to avoid repeated declaration
        int noteValue, noteVelocity;
        int wheelIncrement;
        QString editText;

        bool m_phocus;

    protected:
        void mousePressEvent(QMouseEvent*);
        void paintEvent(QPaintEvent*);
        void wheelEvent(QWheelEvent*);

    signals:
        void trigSet(int, Trigger*);


};

class Indicator : public QWidget
{
    Q_OBJECT

    public:
        Indicator(int);
        void setPlayhead(bool);
        void setLBracket(bool);
        void setRBracket(bool);

    private:
        int m_step;
        bool m_hasPlayhead;
        bool m_hasLBracket;
        bool m_hasRBracket;

    protected:
        void paintEvent(QPaintEvent*);
        void mousePressEvent(QMouseEvent*);

    signals:
        void lBracketSet(int);
        void rBracketSet(int);

};

#endif
