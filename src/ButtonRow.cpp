#include "ButtonRow.h"

#include <QDebug>

ButtonRow::ButtonRow(int nsteps) : QWidget() {

    m_nsteps = nsteps;

    QVBoxLayout *layout = new QVBoxLayout();

    if (m_nsteps > 0) {

        QHBoxLayout *middleLayout = new QHBoxLayout();
        QHBoxLayout *bottomLayout = new QHBoxLayout();

        playheadIndicator = nullptr;
        lBracketIndicator = nullptr;
        rBracketIndicator = nullptr;

        Button *tmpButton;
        Indicator *tmpIndicator;
        for (int i=0; i < m_nsteps; i++) {

            tmpButton = new Button(i);
            connect(tmpButton, SIGNAL(trigSet(int, Trigger*)), this, SIGNAL(trigSet(int, Trigger*)));
            middleLayout->addWidget(tmpButton);

            tmpIndicator = new Indicator(i);
            indicators.push_back(tmpIndicator);
            connect(tmpIndicator, SIGNAL(lBracketSet(int)), this, SLOT(updateLBracket(int)));
            connect(tmpIndicator, SIGNAL(rBracketSet(int)), this, SLOT(updateRBracket(int)));
            if (i==0) tmpIndicator->setLBracket(true);
            if (i==(m_nsteps-1)) tmpIndicator->setRBracket(true);
            bottomLayout->addWidget(tmpIndicator);

        }

        layout->addLayout(middleLayout);
        layout->addLayout(bottomLayout);

    } else {

        QLabel *defaultLabel = new QLabel("No Sequence Selected");
        defaultLabel->setAlignment(Qt::AlignCenter);
        defaultLabel->setFont(QFont("Helvetica", 14));
        layout->addWidget(defaultLabel);

    }

    setLayout(layout);

}

void ButtonRow::updatePlayhead(int step) {

    if (playheadIndicator) playheadIndicator->setPlayhead(false);
    playheadIndicator = indicators[step];
    playheadIndicator->setPlayhead(true);

}

void ButtonRow::updateLBracket(int step) {


    if (lBracketIndicator) lBracketIndicator->setLBracket(false);
    lBracketIndicator = indicators[step];
    emit lBracketChanged(step);

}

void ButtonRow::updateRBracket(int step) {

    if (rBracketIndicator) rBracketIndicator->setRBracket(false);
    rBracketIndicator = indicators[step];
    emit rBracketChanged(step);

}

