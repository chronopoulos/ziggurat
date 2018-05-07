#include "GroupManager.h"

#include <QMenu>
#include <QAction>

GroupManager::GroupManager(void) {

    layout = new QHBoxLayout();
    layout->setAlignment(Qt::AlignLeft);
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

}

void GroupManager::addGroupWidget(GroupWidget *groupWidget) {

    layout->addWidget(groupWidget);

}

void GroupManager::contextMenuEvent(QContextMenuEvent*) {

    QMenu managerMenu;
    managerMenu.addAction("Add Group");
    QAction *action = managerMenu.exec(QCursor::pos());
    if (action && (action->text().contains("Add Group"))) {
        emit newGroupRequested();
    }

}
