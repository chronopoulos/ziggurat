#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QFrame>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QLabel>

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
        QLabel *emptySetIcon;
        bool nullState;
        void setNullState(void);

    protected:
        void contextMenuEvent(QContextMenuEvent*);

    signals:
        void newSequenceRequested(int, QString);
        void deleteRequested(void);

};

#endif
