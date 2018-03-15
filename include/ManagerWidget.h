#include <QFrame>
#include <QFocusEvent>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QMenu>
#include <QAction>
#include <QCursor>

#include "Sequence.h"
#include "Thumbnail.h"
#include "Canvas.h"
#include "ConfigPage.h"

class ManagerWidget : public QFrame
{
    Q_OBJECT

    public:
        ManagerWidget();
        void addSequence(int);
        void addThumbnail(Thumbnail*);
        void removeThumbnail(Thumbnail*);

    private:
        QVBoxLayout *layout;
        std::vector<Sequence*> seqs;
        std::vector<Sequence*>::iterator seqIter;
        std::vector<Canvas*> canvs;
        std::vector<ConfigPage*> pages;
        Thumbnail *interestingThumb;

        void makeConnections(Sequence*, Thumbnail*, Canvas*);

    protected:
        void focusInEvent(QFocusEvent*);
        void focusOutEvent(QFocusEvent*);
        void contextMenuEvent(QContextMenuEvent*);

    public slots:
        void selectThumbnail(Thumbnail*);

    signals:
        void editorViewRequested(Canvas*);
        void newSequenceRequested(void);
        void tick(void);

};
