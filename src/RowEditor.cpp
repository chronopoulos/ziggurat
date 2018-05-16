#include "RowEditor.h"

RowEditor::RowEditor(void) {

    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    defaultRow = new ButtonRow(0);
    currentRow = defaultRow;

    mainLayout->addWidget(currentRow);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

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

