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

void GroupManager::addGroup(void) {

    GroupWidget *groupWidget = new GroupWidget();
    layout->addWidget(groupWidget);
    connect(groupWidget, SIGNAL(pageSelected(ConfigPage*)), this, SIGNAL(pageSelected(ConfigPage*)));
    connect(groupWidget, SIGNAL(rowSelected(ButtonRow*)), this, SIGNAL(rowSelected(ButtonRow*)));

    connect(this, SIGNAL(tick_passthrough(void)), groupWidget, SIGNAL(tick_passthrough(void)));
    connect(this, SIGNAL(resetAll_passthrough(void)), groupWidget, SLOT(resetAll(void)));

    groupWidgets.push_back(groupWidget);

}

void GroupManager::contextMenuEvent(QContextMenuEvent*) {

    QMenu managerMenu;
    managerMenu.addAction("Add Group");
    QAction *action = managerMenu.exec(QCursor::pos());
    if (action && (action->text().contains("Add Group"))) {
        addGroup();
    }

}
