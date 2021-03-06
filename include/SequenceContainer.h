#ifndef SEQUENCECONTAINER_H
#define SEQUENCECONTAINER_H

#include <QObject>
#include <QString>

#include "Sequence.h"
#include "Thumbnail.h"
#include "ConfigPage.h"
#include "ButtonRow.h"

class SequenceContainer : public QObject
{
    Q_OBJECT

    public:
        SequenceContainer(int, QString);
        ~SequenceContainer(void);
        SequenceContainer(const QJsonObject&);
        SequenceContainer(SequenceContainer*);

        Sequence *seq;
        Thumbnail *thumb;
        ConfigPage *page;
        ButtonRow *row;

        bool selected(void);

        void setMute(bool);
        void setQueue(bool);
        void toggleMute(void);
        void toggleQueue(void);

    private:
        void makeConnections(void);

    signals:
        void pageSelected(ConfigPage*);
        void thumbnailSelected(Thumbnail*);
        void rowSelected(ButtonRow*);
        void deleteRequested(SequenceContainer*);

        void muteChanged_passthrough(bool);
        void subloopCompleted_passthrough(void);

    public slots:
        void select(void);
        void routeDelete(void);


};

#endif
