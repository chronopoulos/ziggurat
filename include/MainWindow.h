#include <QWidget>
#include <QGridLayout>
#include <QString>

#include "GroupManager.h"
#include "TransportWidget.h"
#include "Configurator.h"
#include "RowEditor.h"

#include "Session.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    public:
        MainWindow(void);
        void togglePlayState(void);

    private:
        int state;
        Session *session;
        QGridLayout *layout;
        GroupManager *groupManager;
        TransportWidget *transport;
        Configurator *config;
        RowEditor *rowEditor;
        bool saveSession(void);
        void openSession(void);

    protected:
        void keyPressEvent(QKeyEvent*);
        void closeEvent(QCloseEvent*);

};
