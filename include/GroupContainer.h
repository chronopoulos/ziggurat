#include "Group.h"
#include "GroupWidget.h"

class GroupContainer : public QObject
{
    Q_OBJECT

    public:
        GroupContainer(void);
        ~GroupContainer(void);

        Group *group;
        GroupWidget *groupWidget;

    private:
        std::vector<SequenceContainer*> sconts;
        std::vector<SequenceContainer*>::iterator scontIter;

    signals:
        void newSequenceRequested(int, QString);

};
