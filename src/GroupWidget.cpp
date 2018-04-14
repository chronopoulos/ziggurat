#include "GroupWidget.h"

#include <QLabel>
#include <QDebug>
#include <QMenu>
#include <QAction>

GroupWidget::GroupWidget(void) {

    typeSelector = new QComboBox();
    typeSelector->addItem("Free");
    typeSelector->addItem("Scene");
    typeSelector->addItem("Exclusive");
    typeSelector->addItem("Chain");
    typeSelector->setFocusPolicy(Qt::NoFocus);

    seqLayout = new QVBoxLayout();
    seqLayout->setAlignment(Qt::AlignTop);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignTop);
    mainLayout->addWidget(typeSelector);
    mainLayout->addLayout(seqLayout);
    setLayout(mainLayout);

    setMinimumWidth(250);
    setMaximumWidth(300);

}

void GroupWidget::addSequence(void) {

    seqLayout->addWidget(new QLabel("Placeholder"));

}

void GroupWidget::contextMenuEvent(QContextMenuEvent*) {

    QMenu managerMenu;
    managerMenu.addAction("Add Sequence");
    QAction *action = managerMenu.exec(QCursor::pos());
    if (action && (action->text().contains("Add Sequence"))) {
        addSequence();
    }

}
