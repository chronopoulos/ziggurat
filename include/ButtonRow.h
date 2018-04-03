#ifndef BUTTONROW_H
#define BUTTONROW_H

#include <QWidget>
#include <QHBoxLayout>

#include "ButtonCell.h"

class ButtonRow : public QWidget
{
    Q_OBJECT

    public:
        ButtonRow(int);

    private:
        int m_nsteps;
        std::vector<ButtonCell*> cells;
        ButtonCell *playheadCell;

    public slots:
        void updatePlayhead(int);

};

#endif
