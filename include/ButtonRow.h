#ifndef BUTTONROW_H
#define BUTTONROW_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include "ClickLabel.h"
#include "ButtonCell.h"

class ButtonRow : public QWidget
{
    Q_OBJECT

    public:
        ButtonRow(int);
        void setName(QString);

    private:
        int m_nsteps;
        ClickLabel *nameLabel;
        ClickLabel *lengthLabel;
        ClickLabel *clockDivLabel;
        ClickLabel *transposeLabel;
        ClickLabel *midiChanLabel;
        ClickLabel *directionLabel;
        std::vector<Indicator*> indicators;
        Indicator *playheadIndicator;

    public slots:
        void updatePlayhead(int);

    signals:
        void trigSet(int, Trigger*);
        void nameChanged(QString);
        void clockDivChanged(int);
        void transposeChanged(int);
        void midiChanChanged(int);
        void directionChanged(QString);

};

#endif
