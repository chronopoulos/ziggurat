#include <QDebug>

#include "ConfigPage.h"


ConfigPage::ConfigPage(int nsteps) : m_nsteps(nsteps) {

    layout = new QGridLayout;

    if (nsteps > 0) {

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

        layout->addWidget(nameLabel, 0,0, 1,1);
        layout->addWidget(lengthLabel, 0,1, 1,1);
        layout->addWidget(clockDivLabel, 2,0, 1,1);
        layout->addWidget(transposeLabel, 1,0, 1,1);
        layout->addWidget(midiChanLabel, 1,1, 1,1);
        layout->addWidget(directionLabel, 2,1, 1,1);

    } else {

        defaultLabel = new QLabel("No Sequence Selected");
        layout->addWidget(defaultLabel);

    }

    setLayout(layout);

}

void ConfigPage::setName(QString name) {

    nameLabel->setValue(name);

}
