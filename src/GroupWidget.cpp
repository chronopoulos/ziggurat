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
    typeSelector->addItem("Exclusive");
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

    setMinimumWidth(320);

    group = Group();

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

    connect(scont, SIGNAL(pageSelected(ConfigPage*)), this, SIGNAL(pageSelected(ConfigPage*)));
    connect(scont, SIGNAL(thumbnailSelected(Thumbnail*)), seqManager, SLOT(selectThumbnail(Thumbnail*)));
    connect(scont, SIGNAL(rowSelected(ButtonRow*)), this, SIGNAL(rowSelected(ButtonRow*)));

    connect(scont, SIGNAL(deleteRequested(SequenceContainer*)),
            this, SLOT(deleteSequence(SequenceContainer*)));

    /*
    connect(transport, SIGNAL(ticked(void)), scont->seq, SLOT(tick(void)));
    */

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
