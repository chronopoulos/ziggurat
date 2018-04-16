#include <QWidget>
#include <QGridLayout>
#include <QString>

#include "ManagerWidget.h"
#include "GroupManager.h"
#include "TransportWidget.h"
#include "Configurator.h"
#include "RowEditor.h"

#include "Group.h"

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

    public slots:
        void keyPressEvent(QKeyEvent*);

};
