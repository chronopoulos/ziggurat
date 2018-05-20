#include "Group.h"
#include "GroupWidget.h"

class GroupContainer : public QObject
{
    Q_OBJECT

    public:
        GroupContainer(void);
        GroupContainer(const QJsonObject&);
        ~GroupContainer(void);
        void addScont(SequenceContainer*);
        void removeScont(SequenceContainer*);

        Group *group;
        GroupWidget *groupWidget;

    private:
        std::vector<SequenceContainer*> sconts;
        std::vector<SequenceContainer*>::iterator scontIter;

    signals:
        void newSequenceRequested(int, QString);
        void deleteRequested(GroupContainer*);
        void transferRequested(Thumbnail*, GroupContainer*);

    public slots:
        void routeDelete(void);
        void routeTransfer(Thumbnail*);

};
