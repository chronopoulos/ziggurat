#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QFrame>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QCursor>

#include "Thumbnail.h"

class GroupWidget : public QFrame
{
    Q_OBJECT

    public:
        GroupWidget(void);
        void addThumbnail(Thumbnail*);
        void removeThumbnail(Thumbnail*);

    private:
        QVBoxLayout *layout;
        Thumbnail *interestingThumb;

    protected:
        void contextMenuEvent(QContextMenuEvent*);

    signals:
        void newSequenceRequested(int, QString);

};

#endif
