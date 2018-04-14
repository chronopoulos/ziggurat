#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QContextMenuEvent>

#include "ManagerWidget.h"
#include "Group.h"
#include "SequenceContainer.h"

class GroupWidget : public QWidget
{
    Q_OBJECT

    public:
        GroupWidget(void);
        void addSequence(int, QString);

    private:
        QComboBox *typeSelector;
        QVBoxLayout *seqLayout;
        ManagerWidget* seqManager;
        Group group;

        std::vector<SequenceContainer*> sconts;
        std::vector<SequenceContainer*>::iterator scontIter;

    public slots:
        void addSequence(void);
        void deleteSequence(SequenceContainer*);

    signals:
        void pageSelected(ConfigPage*);
        void rowSelected(ButtonRow*);

};
