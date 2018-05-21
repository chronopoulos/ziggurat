#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QFrame>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>

#include "Thumbnail.h"

class GroupWidget : public QFrame
{
    Q_OBJECT

    public:
        GroupWidget(void);
        void addThumbnail(Thumbnail*);
        void addThumbnailAt(Thumbnail*, int);

    public slots:
        void removeThumbnail(Thumbnail*);

    private:
        QVBoxLayout *layout;
        Thumbnail *interestingThumb;
        QLabel *emptySetIcon;
        bool nullState;
        void setNullState(void);

    protected:
        void contextMenuEvent(QContextMenuEvent*);
        void dragEnterEvent(QDragEnterEvent*);
        void dropEvent(QDropEvent*);

    signals:
        void newSequenceRequested(int, QString);
        void deleteRequested(void);
        void transferRequested(Thumbnail*, int);
        void reorderRequested(Thumbnail*, int, int);

};

#endif
