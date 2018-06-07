#include "GroupManager.h"

#include <QMenu>
#include <QAction>

GroupManager::GroupManager(void) {

    layout = new QHBoxLayout();
    layout->setAlignment(Qt::AlignLeft);
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);

    setScoped(false);

}

void GroupManager::paintEvent(QPaintEvent *e) {

    if (m_scoped) {
        setLineWidth(2);
    } else {
        setLineWidth(1);
    }

    QFrame::paintEvent(e);

}

void GroupManager::setScoped(bool scoped) {

    m_scoped = scoped;
    update();

}

void GroupManager::addGroupWidget(GroupWidget *groupWidget) {

    layout->addWidget(groupWidget);

}

void GroupManager::removeGroupWidget(GroupWidget *groupWidget) {

    if (layout->indexOf(groupWidget) >= 0) {
        layout->removeWidget(groupWidget);
        groupWidget->setParent(NULL);
    }

}

void GroupManager::contextMenuEvent(QContextMenuEvent*) {

    QMenu managerMenu;
    managerMenu.addAction("Add Group");
    QAction *action = managerMenu.exec(QCursor::pos());
    if (action && (action->text().contains("Add Group"))) {
        emit newGroupRequested();
    }

}
