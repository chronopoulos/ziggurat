#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>

#include "TrigRequest.h"

#define NNOTES 12
#define CELLSIZE 10

class Cell : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

    public:
        Cell(int, int);
        int step();
        int note();

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent*);

    private:
        int m_step, m_note;

    signals:
        void trigRequested(TrigRequest*);

};


////


class SeqNameItem : public QGraphicsTextItem
{
    Q_OBJECT

    public:
        SeqNameItem();

    public:
        void keyPressEvent(QKeyEvent*);

    signals:
        void nameChanged(QString);

};


////


class BracketItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT

    public:
        BracketItem(int, int, bool);

        static bool Type_Left;
        static bool Type_Right;

    private:
        QPointF oldPos, newPos;
        int xFromStep(int);
        int m_nsteps, m_step;
        bool m_type;

    protected:
        QVariant itemChange(GraphicsItemChange, const QVariant&);
        void mousePressEvent(QGraphicsSceneMouseEvent*);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent*);

    public slots:
        void setStep(int);

    signals:
        void stepChanged(int);

};
