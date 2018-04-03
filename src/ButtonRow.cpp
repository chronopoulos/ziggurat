#include "ButtonRow.h"

#include <QDebug>

ButtonRow::ButtonRow(int nsteps) : QWidget() {

    m_nsteps = nsteps;

    QHBoxLayout *layout = new QHBoxLayout();

    ButtonCell *tmpCell;
    if (m_nsteps > 0) {
        for (int i=0; i<m_nsteps; i++) {
            tmpCell = new ButtonCell();
            cells.push_back(tmpCell);
            layout->addWidget(tmpCell);
        }
    } else { // dummy/default row indicated
        for (int i=0; i<16; i++) {
            tmpCell = new ButtonCell(true);
            cells.push_back(tmpCell);
            layout->addWidget(tmpCell);
        }
    }

    setLayout(layout);

    playheadCell = nullptr;

}

void ButtonRow::updatePlayhead(int step) {

    if (playheadCell) playheadCell->setPlayhead(false);
    playheadCell = cells[step];
    playheadCell->setPlayhead(true);

}
