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
        MainWindow(const QString&);
        void togglePlayState(void);
        void setScope(int);

    private:
        int state;
        Session *session;
        QGridLayout *layout;
        GroupManager *groupManager;
        TransportWidget *transport;
        Configurator *config;
        RowEditor *rowEditor;

        void routePhocus(QKeyEvent*);

    protected:
        void keyPressEvent(QKeyEvent*);
        void closeEvent(QCloseEvent*);

    public slots:
        void handleDelta(bool);

};
