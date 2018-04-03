#include "RowEditor.h"

RowEditor::RowEditor(void) {

    layout = new QVBoxLayout();
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    currentRow = nullptr;

    defaultRow = new ButtonRow(0);

}

void RowEditor::setRow(ButtonRow *newRow) {

    if (currentRow != newRow) {
        if (currentRow) {
            layout->replaceWidget(currentRow, newRow);
            currentRow->setParent(NULL);
        } else {
            layout->addWidget(newRow);
        }
        currentRow = newRow;
    }

}

void RowEditor::setDefaultRow(void) {

    setRow(defaultRow);

}
