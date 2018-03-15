#include <QObject>
#include <QString>

#include "Sequence.h"
#include "Thumbnail.h"
#include "Canvas.h"
#include "ConfigPage.h"

class SequenceContainer : public QObject
{
    Q_OBJECT

    public:
        SequenceContainer(int, QString);
        ~SequenceContainer();

        Sequence *seq;
        Thumbnail *thumb;
        Canvas *canv;
        ConfigPage *page;

        bool selected(void);

    signals:
        void canvasSelected(Canvas*);
        void pageSelected(ConfigPage*);
        void thumbnailSelected(Thumbnail*);
        void deleteRequested(SequenceContainer*);

    public slots:
        void select(void);
        void routeDelete(void);


};
