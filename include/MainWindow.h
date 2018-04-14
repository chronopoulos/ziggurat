#include <QWidget>
#include <QGridLayout>
#include <QString>

#include "ManagerWidget.h"
#include "GroupManager.h"
#include "TransportWidget.h"
#include "Configurator.h"
#include "RowEditor.h"
#include "Dialogs.h"

#include "SequenceContainer.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    public:
        MainWindow();
        void togglePlayState(void);

    private:
        int state;
        QGridLayout *layout;
        ManagerWidget *manager;
        GroupManager *groupManager;
        TransportWidget *transport;
        Configurator *config;
        RowEditor *rowEditor;

        std::vector<SequenceContainer*> sconts;
        std::vector<SequenceContainer*>::iterator scontIter;
        void addSequence(int, QString);
        void deleteSequenceOfInterest(void);

    public slots:
        void keyPressEvent(QKeyEvent*);
        void resetAll(void);
        void addSequence(void);
        void deleteSequence(SequenceContainer*);

};
