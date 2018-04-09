#include "ButtonRow.h"

#include <QDebug>

ButtonRow::ButtonRow(int nsteps) : QWidget() {

    m_nsteps = nsteps;

    QVBoxLayout *layout = new QVBoxLayout();

    if (m_nsteps > 0) {

        QGridLayout *topLayout = new QGridLayout();
        QHBoxLayout *middleLayout = new QHBoxLayout();
        QHBoxLayout *bottomLayout = new QHBoxLayout();

        nameLabel = new ClickLabel("Name: %1", ClickLabel::Type_QString);
        QObject::connect(nameLabel, SIGNAL(valueChanged(QString)),
                            this, SIGNAL(nameChanged(QString)));

        lengthLabel = new ClickLabel("Length: %1", ClickLabel::Type_Int);
        lengthLabel->setValue(m_nsteps);
        lengthLabel->setHardwired(true); // hardwired for now

        clockDivLabel = new ClickLabel("Clock Divide: %1", ClickLabel::Type_Int);
        clockDivLabel->setMin(1);
        clockDivLabel->setValue(1);
        QObject::connect(clockDivLabel, SIGNAL(valueChanged(int)),
                            this, SIGNAL(clockDivChanged(int)));

        transposeLabel = new ClickLabel("Transpose: %1", ClickLabel::Type_Int);
        transposeLabel->setValue(0);
        QObject::connect(transposeLabel, SIGNAL(valueChanged(int)),
                            this, SIGNAL(transposeChanged(int)));

        midiChanLabel = new ClickLabel("MIDI Channel: %1", ClickLabel::Type_Int);
        midiChanLabel->setMin(1);
        midiChanLabel->setMax(16);
        midiChanLabel->setValue(1);
        QObject::connect(midiChanLabel, SIGNAL(valueChanged(int)),
                            this, SIGNAL(midiChanChanged(int)));

        directionLabel = new ClickLabel("Direction: %1", ClickLabel::Type_Item);
        QStringList items;
        items << "Forward" << "Backward" << "Bounce";
        directionLabel->setItems(items);
        directionLabel->setValue("Forward");
        QObject::connect(directionLabel, SIGNAL(valueChanged(QString)),
                            this, SIGNAL(directionChanged(QString)));

        topLayout->addWidget(nameLabel, 0,0, 1,1);
        topLayout->addWidget(lengthLabel, 0,1, 1,1);
        topLayout->addWidget(transposeLabel, 1,0, 1,1);
        topLayout->addWidget(midiChanLabel, 1,1, 1,1);
        topLayout->addWidget(clockDivLabel, 2,0, 1,1);
        topLayout->addWidget(directionLabel, 2,1, 1,1);

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

        layout->addLayout(topLayout);
        layout->addLayout(middleLayout);
        layout->addLayout(bottomLayout);

    } else {

        QLabel *dummyLabel = new QLabel("No Sequence Selected");
        dummyLabel->setAlignment(Qt::AlignCenter);
        dummyLabel->setFont(QFont("Helvetica", 20));
        layout->addWidget(dummyLabel);

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

void ButtonRow::setName(QString name) {

    nameLabel->setValue(name);

}

