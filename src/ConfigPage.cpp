#include <QDebug>

#include "ConfigPage.h"


ConfigPage::ConfigPage(int nsteps) : m_nsteps(nsteps) {

    layout = new QGridLayout;

    if (nsteps > 0) {

        nameLabel = new ClickLabel("Name: %1", ClickLabel::Type_QString,
                                    "Ziggurat", "Name");
        connect(nameLabel, SIGNAL(valueChanged(QString)), this, SIGNAL(nameChanged(QString)));

        lengthLabel = new ClickLabel("Length: %1", ClickLabel::Type_Int,
                                        "Ziggurat", "Length");
        lengthLabel->setValue(m_nsteps);
        lengthLabel->setHardwired(true); // hardwired for now

        clockDivLabel = new ClickLabel("Clock Divide: %1", ClickLabel::Type_Int,
                                        "Ziggurat", "Clock Divide");
        clockDivLabel->setMin(1);
        clockDivLabel->setValue(1);
        connect(clockDivLabel, SIGNAL(valueChanged(int)), this, SIGNAL(clockDivChanged(int)));

        transposeLabel = new ClickLabel("Transpose: %1", ClickLabel::Type_Int,
                                        "Ziggurat", "Transpose");
        transposeLabel->setValue(0);
        connect(transposeLabel, SIGNAL(valueChanged(int)), this, SIGNAL(transposeChanged(int)));

        midiChanLabel = new ClickLabel("MIDI Channel: %1", ClickLabel::Type_Int,
                                        "Ziggurat", "MIDI Channel");
        midiChanLabel->setMin(1);
        midiChanLabel->setMax(16);
        midiChanLabel->setValue(1);
        connect(midiChanLabel, SIGNAL(valueChanged(int)), this, SIGNAL(midiChanChanged(int)));

        directionLabel = new ClickLabel("Direction: %1", ClickLabel::Type_Item,
                                        "Ziggurat", "Direction");
        QStringList items;
        items << "Forward" << "Backward" << "Bounce";
        directionLabel->setItems(items);
        directionLabel->setValue("Forward");
        connect(directionLabel, SIGNAL(valueChanged(QString)), this, SIGNAL(directionChanged(QString)));

        connect(nameLabel, SIGNAL(valueChanged(QString)), nameLabel, SLOT(setTitle(QString)));
        connect(nameLabel, SIGNAL(valueChanged(QString)), lengthLabel, SLOT(setTitle(QString)));
        connect(nameLabel, SIGNAL(valueChanged(QString)), clockDivLabel, SLOT(setTitle(QString)));
        connect(nameLabel, SIGNAL(valueChanged(QString)), transposeLabel, SLOT(setTitle(QString)));
        connect(nameLabel, SIGNAL(valueChanged(QString)), midiChanLabel, SLOT(setTitle(QString)));
        connect(nameLabel, SIGNAL(valueChanged(QString)), directionLabel, SLOT(setTitle(QString)));

        layout->addWidget(nameLabel, 0,0, 1,1);
        layout->addWidget(lengthLabel, 0,1, 1,1);
        layout->addWidget(clockDivLabel, 2,0, 1,1);
        layout->addWidget(transposeLabel, 1,0, 1,1);
        layout->addWidget(midiChanLabel, 1,1, 1,1);
        layout->addWidget(directionLabel, 2,1, 1,1);

    } else {

        QLabel *defaultLabel = new QLabel("No Sequence Selected");
        defaultLabel->setAlignment(Qt::AlignCenter);
        defaultLabel->setFont(QFont("Helvetica", 14));
        layout->addWidget(defaultLabel);

    }

    setLayout(layout);

}

ConfigPage::ConfigPage(const QJsonObject &seqJSO) : ConfigPage(seqJSO["nsteps"].toInt()) {

    nameLabel->setValue(seqJSO["name"].toString());
    clockDivLabel->setValue(seqJSO["div"].toInt());
    transposeLabel->setValue(seqJSO["transpose"].toInt());
    midiChanLabel->setValue(seqJSO["midiChan"].toInt());
    directionLabel->setValue(seqJSO["direction"].toInt());

}

void ConfigPage::setName(QString name) {

    nameLabel->setValue(name);

}
