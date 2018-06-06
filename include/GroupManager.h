#include <QHBoxLayout>
#include <QFrame>

#include "GroupWidget.h"

class GroupManager : public QFrame
{
    Q_OBJECT

    public:
        GroupManager(void);
        void setScoped(bool);

    public slots:
        void addGroupWidget(GroupWidget*);
        void removeGroupWidget(GroupWidget*);

    private:
        QHBoxLayout *layout;
        bool m_scoped;

    protected:
        void contextMenuEvent(QContextMenuEvent*);
        void paintEvent(QPaintEvent*);

    signals:
        void newGroupRequested(void);

};
