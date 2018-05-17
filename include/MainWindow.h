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

    private:
        int state;
        Session *session;
        QGridLayout *layout;
        GroupManager *groupManager;
        TransportWidget *transport;
        Configurator *config;
        RowEditor *rowEditor;

    protected:
        void keyPressEvent(QKeyEvent*);
        void closeEvent(QCloseEvent*);

};
