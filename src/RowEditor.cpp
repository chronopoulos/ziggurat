#include "RowEditor.h"

RowEditor::RowEditor(void) {

    layout = new QVBoxLayout();
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    defaultRow = new ButtonRow(0);

    currentRow = defaultRow;
    layout->addWidget(currentRow);

}

void RowEditor::setRow(ButtonRow *newRow) {

    // case of newRow == nullptr
    if (!newRow) newRow = defaultRow;

    if (newRow != currentRow) {

        layout->replaceWidget(currentRow, newRow);

        currentRow->hide();
        currentRow = newRow;
        currentRow->show();

    }

}

