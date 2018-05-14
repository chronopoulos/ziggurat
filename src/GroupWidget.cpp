#include "GroupWidget.h"
#include "Dialogs.h"

#include <QDebug>

GroupWidget::GroupWidget(void) : QFrame() {

    layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    setMinimumWidth(340);

}

GroupWidget::~GroupWidget(void) {

    // this is to prevent double deletion with scont in Session::load()
    /*
    Thumbnail *thumb;
    for (int i=0; i<layout->count(); i++) {
        thumb = qobject_cast<Thumbnail*> layout->itemAt(i)->widget();
        removeThumbnail(thumb);
    }
    */

}

void GroupWidget::addThumbnail(Thumbnail *thumb) {

    layout->addWidget(thumb);

}

void GroupWidget::removeThumbnail(Thumbnail *thumb) {

    if (layout->indexOf(thumb) >= 0) {
        layout->removeWidget(thumb);
        thumb->setParent(NULL);
    }

}

void GroupWidget::contextMenuEvent(QContextMenuEvent*) {

    QMenu managerMenu;
    managerMenu.addAction("Add Sequence");
    managerMenu.addAction("Delete Group");
    QAction *action = managerMenu.exec(QCursor::pos());
    if (action) {
        if (action->text().contains("Add Sequence")) {
            NewSequenceDialog dlg;
            if (dlg.exec() == QDialog::Accepted) {
                emit newSequenceRequested(dlg.length(), dlg.name());
            }
        } else if (action->text().contains("Delete Group")) {
            emit deleteRequested();
        }
    }

}
