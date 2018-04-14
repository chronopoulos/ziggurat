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
        std::vector<GroupWidget*> groups;

    protected:
        void contextMenuEvent(QContextMenuEvent*);

};
