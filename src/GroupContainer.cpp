#include "GroupContainer.h"

#include <QDebug>
#include <QJsonArray>

GroupContainer::GroupContainer(void) {

    group = new Group();
    groupWidget = new GroupWidget();

    connect(groupWidget, SIGNAL(newSequenceRequested(int, QString)),
            this, SIGNAL(newSequenceRequested(int, QString)));

    connect(groupWidget, SIGNAL(deleteRequested(void)),
            this, SLOT(routeDelete(void)));

    connect(groupWidget, SIGNAL(transferRequested(Thumbnail*, int)),
            this, SLOT(routeTransfer(Thumbnail*, int)));

    connect(groupWidget, SIGNAL(reorderRequested(Thumbnail*, int, int)),
            this, SLOT(handleReorder(Thumbnail*, int, int)));

}

GroupContainer::GroupContainer(const QJsonObject &groupJSO) : GroupContainer() {

    group->setType(groupJSO["type"].toInt());

    QJsonArray seqJSA = groupJSO["seqs"].toArray();
    SequenceContainer *scont;
    for (int i = 0; i < seqJSA.size(); i++) {
        scont = new SequenceContainer(seqJSA[i].toObject());
        group->addScont(scont);
        groupWidget->addThumbnail(scont->thumb);
    }

}

GroupContainer::~GroupContainer(void) {

    delete group;
    delete groupWidget;

}

void GroupContainer::addScont(SequenceContainer *scont) {

    group->addScont(scont);
    groupWidget->addThumbnail(scont->thumb);

}

void GroupContainer::addScontAt(SequenceContainer *scont, int insertIndex) {

    group->addScontAt(scont, insertIndex);
    groupWidget->addThumbnailAt(scont->thumb, insertIndex);

}

void GroupContainer::removeScont(SequenceContainer *scont) {

    group->removeScont(scont);
    groupWidget->removeThumbnail(scont->thumb);

}

void GroupContainer::routeDelete(void) {

    emit deleteRequested(this);

}

void GroupContainer::routeTransfer(Thumbnail *thumb, int insertIndex) {

    emit transferRequested(thumb, this, insertIndex);

}

void GroupContainer::handleReorder(Thumbnail *thumb, int oldIndex, int insertIndex) {

    // find the scont
    SequenceContainer *scont = nullptr;
    for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {

        if ((*scontIter)->thumb == thumb) {
            scont = *scontIter;
            break;
        }

    }

    // do the move, according to oldIndex, insertIndex
    if (oldIndex == (insertIndex-1)) {

        return; // no change

    } else if (insertIndex <= oldIndex) {

        group->removeScont(scont);
        group->addScontAt(scont, insertIndex);

        groupWidget->removeThumbnail(thumb);
        groupWidget->addThumbnailAt(thumb, insertIndex);

    } else { // insertIndex > (oldIndex + 1)

        group->removeScont(scont);
        group->addScontAt(scont, insertIndex - 1);

        groupWidget->removeThumbnail(thumb);
        groupWidget->addThumbnailAt(thumb, insertIndex - 1);

    }


}
