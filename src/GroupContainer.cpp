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

void GroupContainer::routeDelete(void) {

    emit deleteRequested(this);

}

