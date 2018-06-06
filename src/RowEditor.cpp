#include "RowEditor.h"

RowEditor::RowEditor(void) {

    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    defaultRow = new ButtonRow(0);
    currentRow = defaultRow;

    mainLayout->addWidget(currentRow);

    setFrameStyle(QFrame::Box | QFrame::Plain);

    setScoped(false);

}

void RowEditor::paintEvent(QPaintEvent *e) {

    if (m_scoped) {
        setLineWidth(2);
    } else {
        setLineWidth(1);
    }

    QFrame::paintEvent(e);

}

void RowEditor::setScoped(bool scoped) {

    m_scoped = scoped;
    update();

}

void RowEditor::setRow(ButtonRow *newRow) {

    // case of newRow == nullptr
    if (!newRow) newRow = defaultRow;

    if (newRow != currentRow) {

        mainLayout->replaceWidget(currentRow, newRow);

        currentRow->hide();
        currentRow = newRow;
        currentRow->show();

    }

}

