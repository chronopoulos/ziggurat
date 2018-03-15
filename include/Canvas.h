#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

#include "Thumbnail.h"
#include "TrigRequest.h"
#include "GraphicsItems.h"

//

class Canvas : public QGraphicsScene
{
    Q_OBJECT

    public:
        Canvas(int);
        SeqNameItem *nameLabel;
        BracketItem *lBracket, *rBracket;

    private:
        int nsteps;
        int cursor_row;
        int cursor_col;
        QGraphicsRectItem *cursor, *playhead;
        QString m_name;
        QGraphicsTextItem *defaultText;

    public slots:
        void registerTrigRequest(TrigRequest*);
        void updatePlayhead(int);
        void setBrackets(int, int);

    signals:
        void trigRequested(TrigRequest*);
        void interestRequested(Thumbnail*);
        void bracketChanged(int, bool);


};

#endif
