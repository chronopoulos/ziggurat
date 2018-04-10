#include <QFrame>
#include <QFocusEvent>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QMenu>
#include <QAction>
#include <QCursor>

#include "Sequence.h"
#include "Thumbnail.h"
#include "ConfigPage.h"

class ManagerWidget : public QFrame
{
    Q_OBJECT

    public:
        ManagerWidget();
        void addThumbnail(Thumbnail*);
        void removeThumbnail(Thumbnail*);

    private:
        QVBoxLayout *layout;
        Thumbnail *interestingThumb;

    protected:
        void focusInEvent(QFocusEvent*);
        void focusOutEvent(QFocusEvent*);
        void contextMenuEvent(QContextMenuEvent*);

    public slots:
        void selectThumbnail(Thumbnail*);

    signals:
        void newSequenceRequested(void);

};
