#include <QPalette>
#include <QGridLayout>
#include <QDebug>
#include <QApplication>
#include <QFileDialog>

#include "MainWindow.h"
#include "Dialogs.h"

#define STATE_STOPPED 0
#define STATE_PLAYING 1
#define STATE_PAUSED 2

MainWindow::MainWindow(void) : QWidget() {

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

    layout->addWidget(groupManager, 0,0, 5,6);
    layout->addWidget(transport, 5,0, 1,1);
    layout->addWidget(config, 5,1, 1,5);
    layout->addWidget(rowEditor, 6,0, 1,6);

    this->setLayout(layout);
    this->setWindowTitle("z i g g u r a t");
    this->setWindowIcon(QIcon(":/img/ziggurat_icon.png"));

    state = STATE_STOPPED;

    // initialize with 3 groups
    /*
    for (int i=0; i<3; i++) {
        session->createGroup();
    }
    */

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

bool MainWindow::saveSession(void) {

    QString filename = QFileDialog::getSaveFileName(this, "Save Session", QDir::homePath());

    bool saved = false;
    if (!filename.isNull()) {
        session->save(filename);
        saved = true;
    }

    return saved;
}

void MainWindow::openSession(void) {

    QString filename = QFileDialog::getOpenFileName(this, "Open Session", QDir::homePath());
    
    if (!filename.isNull()) session->load(filename);

}

void MainWindow::keyPressEvent(QKeyEvent* k) {

    if (!k->isAutoRepeat()) {
        switch (k->key()) {
            case Qt::Key_Space:
                togglePlayState();
                break;
            case Qt::Key_S:
                if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
                    saveSession();
                }
                break;
            case Qt::Key_O:
                if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
                    openSession();
                }
                break;
            case Qt::Key_Escape:
                session->selectNothing();
                break;
        }
    }

}

void MainWindow::closeEvent(QCloseEvent *e) {

    MaybeSaveDialog dlg;

    switch (dlg.exec()) {
        case -1:
            e->accept();
            break;
        case 0:
            e->ignore();
            break;
        case 1:
            if (saveSession()) {
                e->accept();
            } else {
                e->ignore();
            }
            break;
    }


}
