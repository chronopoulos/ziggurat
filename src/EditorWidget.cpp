#include <QDebug>

#include "EditorWidget.h"

EditorWidget::EditorWidget(void) : QFrame() {

    graphicsView = new QGraphicsView();
    transform = new QTransform();

    layout = new QVBoxLayout();
    layout->addWidget(graphicsView);
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    setMinimumSize(500, 460);

    defaultCanvas = new Canvas(0);

}

void EditorWidget::setCanvas(Canvas* canv) {

    
    graphicsView->setScene(canv);

}

void EditorWidget::setDefaultCanvas(void) {

    qDebug() << "editor :: set default canvas";
    
    graphicsView->setScene(defaultCanvas);

}


void EditorWidget::focusInEvent(QFocusEvent*) {

    setLineWidth(4);

}

void EditorWidget::focusOutEvent(QFocusEvent*) {

    setLineWidth(0);

}

void EditorWidget::resizeEvent(QResizeEvent*) {

    qreal hscale = graphicsView->width() / (160.+10);
    qreal vscale = graphicsView->height() / (135.+10);
    // apply an isometric scaling
    transform->setMatrix(qMin(hscale, vscale), 0,0,
                            0,qMin(hscale,vscale),0,
                            0,0,1);
    graphicsView->setTransform(*transform);

}
