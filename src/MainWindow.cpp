#include <QPalette>
#include <QGridLayout>
#include <QDebug>
#include <QApplication>
#include <QFileDialog>

#include "MainWindow.h"
#include "Dialogs.h"
#include "Delta.h"

#define STATE_STOPPED 0
#define STATE_PLAYING 1
#define STATE_PAUSED 2

// TODO make a global enum
int SCOPE;
#define SCOPE_NULL 0
#define SCOPE_MANAGER 1
#define SCOPE_EDITOR  2
#define SCOPE_CONFIG 3

extern Delta DELTA;

MainWindow::MainWindow(const QString &filename) : QWidget() {

    session = new Session();

    layout = new QGridLayout();

    groupManager = new GroupManager();
    transport = new TransportWidget();
    config = new Configurator();
    rowEditor = new RowEditor();

    connect(transport, SIGNAL(ticked(void)), session, SLOT(tick(void)));
    connect(transport, SIGNAL(stopped(void)), session, SLOT(resetAll(void)));

    connect(groupManager, SIGNAL(newGroupRequested(void)), session, SLOT(createGroup(void)));
    connect(session, SIGNAL(groupWidgetCreated(GroupWidget*)), groupManager, SLOT(addGroupWidget(GroupWidget*)));
    connect(session, SIGNAL(groupWidgetDeleted(GroupWidget*)), groupManager, SLOT(removeGroupWidget(GroupWidget*)));
    connect(session, SIGNAL(pageSelected(ConfigPage*)), config, SLOT(setPage(ConfigPage*)));
    connect(session, SIGNAL(rowSelected(ButtonRow*)), rowEditor, SLOT(setRow(ButtonRow*)));

    connect(&DELTA, SIGNAL(stateChanged(bool)), this, SLOT(handleDelta(bool)));

    layout->addWidget(groupManager, 0,0, 5,6);
    layout->addWidget(transport, 5,0, 1,1);
    layout->addWidget(config, 5,1, 1,5);
    layout->addWidget(rowEditor, 6,0, 1,6);

    this->setLayout(layout);
    this->setWindowTitle("ziggurat");
    this->setWindowIcon(QIcon(":/img/ziggurat_icon.png"));

    state = STATE_STOPPED;

    if (filename.isNull()) {

        // initialize with 3 empty groups
        for (int i=0; i<3; i++) {
            session->createGroup();
        }

    } else {

        session->load(filename);

    }

    setScope(SCOPE_MANAGER);

    DELTA.setState(false);

    resize(700,700);

}

void MainWindow::togglePlayState(void) {

    if ((transport->state == TransportWidget::Stopped) ||
        (transport->state == TransportWidget::Paused)) {
        transport->playButton->click();
    } else if (transport->state == TransportWidget::Playing) {
        transport->stopButton->click();
    }

}

void MainWindow::keyPressEvent(QKeyEvent *e) {

    if (!e->isAutoRepeat()) {

        if (e->key() == Qt::Key_Space) {

                togglePlayState();
                return;

        }

        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {

            if (e->key() == Qt::Key_S) {

                if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
                    session->saveAs();
                } else {
                    session->save();
                }

                return;

            } else if (e->key() == Qt::Key_O) {

                session->load();
                return;

            } else if (e->key() == Qt::Key_Left) {

                setScope(SCOPE_NULL);
                return;

            } else if (e->key() == Qt::Key_Up) {

                setScope(SCOPE_MANAGER);
                return;

            } else if (e->key() == Qt::Key_Down) {

                setScope(SCOPE_EDITOR);
                return;

            } else if (e->key() == Qt::Key_Right) {

                setScope(SCOPE_CONFIG);
                return;

            }

        }


    }

    routePhocus(e); // phocus events have the possibility of auto-repeat

}

void MainWindow::routePhocus(QKeyEvent *e) {

    if (SCOPE == SCOPE_NULL) {


    } else if (SCOPE == SCOPE_MANAGER) {

        session->phocusEvent(e);

    } else if (SCOPE == SCOPE_EDITOR) {

        rowEditor->currentRow->phocusEvent(e);

    } else if (SCOPE == SCOPE_CONFIG) {

        //config->phocusEvent(e);

    }

}

void MainWindow::setScope(int scope) {

    SCOPE = scope;

    if (SCOPE == SCOPE_NULL) {

        groupManager->setScoped(false);
        rowEditor->setScoped(false);
        config->setScoped(false);

    } else if (SCOPE == SCOPE_MANAGER) {

        groupManager->setScoped(true);
        rowEditor->setScoped(false);
        config->setScoped(false);

    } else if (SCOPE == SCOPE_EDITOR) {

        groupManager->setScoped(false);
        rowEditor->setScoped(true);
        config->setScoped(false);

    } else if (SCOPE == SCOPE_CONFIG) {

        groupManager->setScoped(false);
        rowEditor->setScoped(false);
        config->setScoped(true);

    }

}

void MainWindow::closeEvent(QCloseEvent *e) {

    if (DELTA.state()) { // if there are changes, then ask to save

        MaybeSaveDialog dlg;
        switch (dlg.exec()) {
            case -1: // Discard
                e->accept();
                break;
            case 0: // Cancel
                e->ignore();
                break;
            case 1: // Save
                if (session->save()) {
                    e->accept();
                } else {
                    e->ignore();
                }
                break;
        }

    } else { // otherwise, just close

        e->accept();

    }

}

void MainWindow::handleDelta(bool delta) {

    QString sessionName = QFileInfo(session->sessionFile).fileName();

    if (delta) {

        setWindowTitle(QString("%1*  -  ziggurat").arg(sessionName));

    } else {

        if (session->sessionFile.isNull()) {

            setWindowTitle(QString("z i g g u r a t"));

        } else {

            setWindowTitle(QString("%1  -  ziggurat").arg(sessionName));

        }

    }

}
