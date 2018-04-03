#ifndef BUTTONCELL_H
#define BUTTONCELL_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QPainter>

class Button : public QPushButton
{
    Q_OBJECT

    public:
        Button(bool dummy=false);

};

class Indicator : public QWidget
{
    Q_OBJECT

    public:
        Indicator();
        void setPlayhead(bool);

    private:
        bool m_hasPlayhead;

    protected:
        void paintEvent(QPaintEvent*);

};

class ButtonCell : public QWidget
{
    Q_OBJECT

    public:
        ButtonCell(bool dummy=false);
        void setPlayhead(bool);

    private:
        Button *button;
        Indicator *indicator;

};

#endif
