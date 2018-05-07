#ifndef SESSION_H
#define SESSION_H

#include "SequenceContainer.h"
#include "GroupContainer.h"

class Session : public QObject
{
    Q_OBJECT

    public:
        Session(void);

    private:
        std::vector<SequenceContainer*> sconts;
        std::vector<SequenceContainer*>::iterator scontIter;
        std::vector<GroupContainer*> gconts;
        std::vector<GroupContainer*>::iterator gcontIter;
        Thumbnail *selectedThumbnail;

    public slots:
        void createGroup(void);
        void createSequenceInGroup(int, QString);
        void updateSelectedThumbnail(Thumbnail*);
        void tick(void);
        void resetAll(void);
        void deleteScont(SequenceContainer*);

    signals:
        void groupWidgetCreated(GroupWidget*);
        void pageSelected(ConfigPage*);
        void rowSelected(ButtonRow*);
       
};

#endif
