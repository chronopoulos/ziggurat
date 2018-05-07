#include <QHBoxLayout>
#include <QFrame>

#include "GroupWidget.h"

class GroupManager : public QFrame
{
    Q_OBJECT

    public:
        GroupManager(void);

    public slots:
        void addGroupWidget(GroupWidget*);

    private:
        QHBoxLayout *layout;

    protected:
        void contextMenuEvent(QContextMenuEvent*);

    signals:
        void newGroupRequested(void);

};
