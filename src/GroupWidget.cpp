#include "GroupWidget.h"

#include <QLabel>
#include <QDebug>
#include <QMenu>
#include <QAction>

#include "Dialogs.h"

GroupWidget::GroupWidget(void) {

    typeSelector = new QComboBox();
    typeSelector->addItem("Free");
    typeSelector->addItem("Scene");
    typeSelector->addItem("Mutex");
    typeSelector->addItem("Chain");
    typeSelector->setFocusPolicy(Qt::NoFocus);

    seqLayout = new QVBoxLayout();
    seqLayout->setAlignment(Qt::AlignTop);

    seqManager = new ManagerWidget();

    connect(seqManager, SIGNAL(newSequenceRequested(void)), this, SLOT(addSequence(void)));

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(typeSelector);
    mainLayout->addWidget(seqManager);
    setLayout(mainLayout);

    setMinimumWidth(340);

    group = new Group();
    connect(typeSelector, SIGNAL(currentIndexChanged(int)), group, SLOT(setType(int)));

}

void GroupWidget::addSequence(void) {

    NewSequenceDialog dlg;
    if (dlg.exec() == QDialog::Accepted) {
        addSequence(dlg.length(), dlg.name());
    }

}

void GroupWidget::addSequence(int nsteps, QString name) {

    SequenceContainer *scont = new SequenceContainer(nsteps, name);

    sconts.push_back(scont);
    seqManager->addThumbnail(scont->thumb);
    group->addScont(scont);

    connect(scont, SIGNAL(pageSelected(ConfigPage*)), this, SIGNAL(pageSelected(ConfigPage*)));
    connect(scont, SIGNAL(thumbnailSelected(Thumbnail*)), seqManager, SLOT(selectThumbnail(Thumbnail*)));
    connect(scont, SIGNAL(rowSelected(ButtonRow*)), this, SIGNAL(rowSelected(ButtonRow*)));

    connect(this, SIGNAL(tick_passthrough(void)), scont->seq, SLOT(tick(void)));

    connect(scont, SIGNAL(deleteRequested(SequenceContainer*)),
            this, SLOT(deleteSequence(SequenceContainer*)));

    connect(scont, SIGNAL(muteChanged_passthrough(bool)), group, SLOT(handleMuteChange(bool)));
    connect(scont, SIGNAL(subloopCompleted_passthrough(void)), group, SLOT(handleSubloopCompleted(void)));

    if (sconts.size() == 1) scont->select();

}

void GroupWidget::deleteSequence(SequenceContainer* scont) {

    seqManager->removeThumbnail(scont->thumb);

    scontIter = std::find(sconts.begin(), sconts.end(), scont);
    sconts.erase(scontIter);

    // if this sequence was selected, select another sequence
    // or, if no sequences remain, set defaults
    if (scont->selected()) {
        if (!sconts.empty()) {
            scontIter = sconts.begin();
            (*scontIter)->select();
        } else {
            emit pageSelected(nullptr);
            emit rowSelected(nullptr);
        }
    }

    delete scont;

}

void GroupWidget::resetAll(void) {

    for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
        (*scontIter)->seq->reset();
    }

}
