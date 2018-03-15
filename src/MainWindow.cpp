#include <QPalette>
#include <QGridLayout>
#include <QDebug>


#include "MainWindow.h"

#define STATE_STOPPED 0
#define STATE_PLAYING 1
#define STATE_PAUSED 2

#define NSEQS_INIT 4

MainWindow::MainWindow(void) : QWidget() {

    layout = new QGridLayout();

    manager = new ManagerWidget(); editor = new EditorWidget();
    transport = new TransportWidget();
    config = new Configurator();

    QObject::connect(manager, SIGNAL(newSequenceRequested(void)),
                        this, SLOT(addSequence(void)));
    QObject::connect(transport, SIGNAL(stopped(void)),
                        this, SLOT(resetAll(void)));

    layout->addWidget(manager, 0,0, 5,1);
    layout->addWidget(editor, 0,1, 5,5);
    layout->addWidget(transport, 5,0, 1,1);
    layout->addWidget(config, 5,1, 1,5);

    this->setLayout(layout);
    this->setWindowTitle("z i g g u r a t");
    this->setWindowIcon(QIcon("img/ziggurat_icon.png"));

    state = STATE_STOPPED;

    // initialize with 16-step sequences
    for (int i=0; i<NSEQS_INIT; i++) {
        addSequence(16, QString("sequence %1").arg(i));
    }

    resize(700,500);

}

void MainWindow::togglePlayState(void) {

    if ((transport->state == TransportWidget::Stopped) ||
        (transport->state == TransportWidget::Paused)) {
        transport->playButton->click();
    } else if (transport->state == TransportWidget::Playing) {
        transport->stopButton->click();
    }

}

void MainWindow::addSequence(void) {

    NewSequenceDialog dlg;
    if (dlg.exec() == QDialog::Accepted) {
        addSequence(dlg.length(), dlg.name());
    }

}

void MainWindow::addSequence(int nsteps, QString name) {

    SequenceContainer *scont = new SequenceContainer(nsteps, name);

    sconts.push_back(scont);
    manager->addThumbnail(scont->thumb);

    QObject::connect(scont, SIGNAL(canvasSelected(Canvas*)), editor, SLOT(setCanvas(Canvas*)));
    QObject::connect(scont, SIGNAL(pageSelected(ConfigPage*)), config, SLOT(setPage(ConfigPage*)));
    QObject::connect(scont, SIGNAL(thumbnailSelected(Thumbnail*)), manager, SLOT(selectThumbnail(Thumbnail*)));

    QObject::connect(scont, SIGNAL(deleteRequested(SequenceContainer*)),
                        this, SLOT(deleteSequence(SequenceContainer*)));

    if (sconts.size() == 1) scont->select();

    QObject::connect(transport, SIGNAL(ticked(void)),
                        scont->seq, SLOT(tick(void)));
}

void MainWindow::resetAll(void) {

    for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
        (*scontIter)->seq->reset();
    }

}

void MainWindow::deleteSequence(SequenceContainer* scont) {

    manager->removeThumbnail(scont->thumb);

    scontIter = std::find(sconts.begin(), sconts.end(), scont);
    sconts.erase(scontIter);

    // if this sequence was selected, select another sequence
    // or, if no sequences remain, set defaults
    if (scont->selected()) {
        if (!sconts.empty()) {
            scontIter = sconts.begin();
            (*scontIter)->select();
        } else {
            editor->setDefaultCanvas();
            config->setDefaultPage();
        }
    }

    delete scont;

}

void MainWindow::deleteSequenceOfInterest(void) {

    for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
        if ((*scontIter)->selected()) {
            deleteSequence(*scontIter);
            break;
        }
    }

}

void MainWindow::keyPressEvent(QKeyEvent* k) {

    if (!k->isAutoRepeat()) {
        switch (k->key()) {
            case Qt::Key_Space:
                togglePlayState();
                break;
            case Qt::Key_N:
                addSequence();
                break;
            case Qt::Key_Delete:
                deleteSequenceOfInterest();
                break;
        }
    }

}