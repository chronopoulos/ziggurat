#include "ManagerWidget.h"

#include <QDebug>

ManagerWidget::ManagerWidget(void) : QFrame() {

    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    interestingThumb = NULL;

}

void ManagerWidget::focusInEvent(QFocusEvent*) {

    setLineWidth(4);

}

void ManagerWidget::focusOutEvent(QFocusEvent*) {

    setLineWidth(0);

}

void ManagerWidget::addThumbnail(Thumbnail *thumb) {

    layout->addWidget(thumb);

}

void ManagerWidget::removeThumbnail(Thumbnail *thumb) {

    layout->removeWidget(thumb);
    thumb->setParent(NULL);
    if (layout->count() == 0) interestingThumb = NULL;

}

void ManagerWidget::selectThumbnail(Thumbnail* thumb) {

    // TODO: change 'interesting' naming to 'selected' (consistent with SequenceContainter)
    if (interestingThumb) interestingThumb->setInterest(false);
    interestingThumb = thumb;
    interestingThumb->setInterest(true);

}

void ManagerWidget::contextMenuEvent(QContextMenuEvent*) {

    QMenu managerMenu;
    managerMenu.addAction("Add Sequence");
    QAction *action = managerMenu.exec(QCursor::pos());
    if (action && (action->text().contains("Add Sequence"))) {
        emit newSequenceRequested();
    }

}
