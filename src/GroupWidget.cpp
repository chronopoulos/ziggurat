#include "GroupWidget.h"
#include "Dialogs.h"

#include <QDebug>

GroupWidget::GroupWidget(void) : QFrame() {

    layout = new QVBoxLayout();
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    emptySetIcon = new QLabel();
    QPixmap pm(":/img/empty_set.png");
    emptySetIcon->setPixmap(pm.scaledToWidth(100, Qt::SmoothTransformation));
    emptySetIcon->setAlignment(Qt::AlignHCenter);

    setNullState();

    setMinimumWidth(340);

    setAcceptDrops(true);

}

void GroupWidget::addThumbnail(Thumbnail *thumb) {

    if (nullState) {

        layout->removeWidget(emptySetIcon);
        emptySetIcon->setParent(NULL);
        layout->setAlignment(Qt::AlignTop);
        nullState = false;

    }

    layout->addWidget(thumb);

}

void GroupWidget::addThumbnailAt(Thumbnail *thumb, int insertIndex) {

    if (nullState) {

        layout->removeWidget(emptySetIcon);
        emptySetIcon->setParent(NULL);
        layout->setAlignment(Qt::AlignTop);
        nullState = false;

    }

    layout->insertWidget(insertIndex, thumb);

}

void GroupWidget::removeThumbnail(Thumbnail *thumb) {

    if (layout->indexOf(thumb) >= 0) {
        layout->removeWidget(thumb);
        thumb->setParent(NULL);
    }

    if (layout->count() == 0) {
        setNullState();
    }

}

void GroupWidget::setNullState(void) {

    layout->setAlignment(Qt::AlignCenter);
    layout->addWidget(emptySetIcon);
    nullState = true;

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

void GroupWidget::dragEnterEvent(QDragEnterEvent *e) {

    if (e->mimeData()->hasFormat("text/plain")) {
        if (e->mimeData()->text() == "thumb") {
            e->accept();
        }
    }

}

void GroupWidget::dropEvent(QDropEvent *e) {

    // determine if this is a transfer or re-order
    Thumbnail *srcThumb = qobject_cast<Thumbnail*>(e->source());
    int oldIndex = layout->indexOf(srcThumb);
    bool transfer = (oldIndex < 0);

    // find the insertIndex
    int ydrop = e->pos().y();
    int insertIndex = 0;
    Thumbnail *tmpThumb;
    if (!nullState) {

        for (int i=0; i<layout->count(); i++) {
            tmpThumb = qobject_cast<Thumbnail*>(layout->itemAt(i)->widget());
            if (ydrop > tmpThumb->geometry().center().y()) {
                insertIndex = i + 1;
            }
        }

    }

    // two different procedures depending on whether it's a transfer or reorder
    if (transfer) {

        emit transferRequested(srcThumb, insertIndex);

    } else {

        emit reorderRequested(srcThumb, oldIndex, insertIndex);

    }

    e->acceptProposedAction();

}
