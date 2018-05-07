#include "GroupContainer.h"

#include <QDebug>

GroupContainer::GroupContainer(void) {

    group = new Group();
    groupWidget = new GroupWidget();

    connect(groupWidget, SIGNAL(newSequenceRequested(int, QString)),
            this, SIGNAL(newSequenceRequested(int, QString)));

}

GroupContainer::~GroupContainer(void) {

    delete group;
    delete groupWidget;

}

