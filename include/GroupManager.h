#include <QHBoxLayout>
#include <QFrame>

#include "GroupWidget.h"

class GroupManager : public QFrame
{
    Q_OBJECT

    public:
        GroupManager(void);
        void addGroup(void);

    private:
        QHBoxLayout *layout;
        std::vector<GroupWidget*> groupWidgets;
        std::vector<GroupWidget*>::iterator gwIter;

    protected:
        void contextMenuEvent(QContextMenuEvent*);

    signals:
        void pageSelected(ConfigPage*);
        void rowSelected(ButtonRow*);
        void tick_passthrough(void);
        void resetAll_passthrough(void);

};
