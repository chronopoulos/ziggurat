#include "GroupManager.h"

#include <QMenu>
#include <QAction>

GroupManager::GroupManager(void) {

    layout = new QHBoxLayout();
    layout->setAlignment(Qt::AlignLeft);
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    for (int i=0; i<2; i++) {
        addGroup();
    }

}

void GroupManager::addGroup(void) {

    GroupWidget *group = new GroupWidget();
    layout->addWidget(group);
    groups.push_back(group);

}

void GroupManager::contextMenuEvent(QContextMenuEvent*) {

    QMenu managerMenu;
    managerMenu.addAction("Add Group");
    QAction *action = managerMenu.exec(QCursor::pos());
    if (action && (action->text().contains("Add Group"))) {
        addGroup();
    }

}
