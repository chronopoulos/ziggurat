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

        labelMap[(struct coord){.row=0, .col=0}] = nameLabel;
        labelMap[(struct coord){.row=0, .col=1}] = lengthLabel;
        labelMap[(struct coord){.row=1, .col=0}] = transposeLabel;
        labelMap[(struct coord){.row=1, .col=1}] = midiChanLabel;
        labelMap[(struct coord){.row=2, .col=0}] = clockDivLabel;
        labelMap[(struct coord){.row=2, .col=1}] = directionLabel;

        nameLabel->setPhocus(true);
        m_phocusCoords.row = 0;
        m_phocusCoords.col = 0;

    } else {

        QLabel *defaultLabel = new QLabel("No Sequence Selected");
        defaultLabel->setAlignment(Qt::AlignCenter);
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

ConfigPage::ConfigPage(Sequence *seq) : ConfigPage(seq->nsteps()) {

    nameLabel->setValue(seq->name());
    clockDivLabel->setValue(seq->div());
    transposeLabel->setValue(seq->transpose());
    midiChanLabel->setValue(seq->midiChan());
    directionLabel->setValue(seq->direction());

}

void ConfigPage::phocusEvent(QKeyEvent *e) {

    if (e->key() == Qt::Key_Right) {
        advancePhocusCol(1);
    } else if (e->key() == Qt::Key_Left) {
        advancePhocusCol(-1);
    } else if (e->key() == Qt::Key_Up) {
        advancePhocusRow(-1);
    } else if (e->key() == Qt::Key_Down) {
        advancePhocusRow(1);
    } else if (!e->isAutoRepeat()) {

        if (e->key() == Qt::Key_Return) {
            labelMap[m_phocusCoords]->runDialog();
        }

    }

}

void ConfigPage::advancePhocusRow(int increment) {

    labelMap[m_phocusCoords]->setPhocus(false);

    m_phocusCoords.row += increment;
    if (m_phocusCoords.row > 2) m_phocusCoords.row = 2;
    if (m_phocusCoords.row < 0) m_phocusCoords.row = 0;

    labelMap[m_phocusCoords]->setPhocus(true);

}

void ConfigPage::advancePhocusCol(int increment) {

    labelMap[m_phocusCoords]->setPhocus(false);

    m_phocusCoords.col += increment;
    if (m_phocusCoords.col > 1) m_phocusCoords.col = 1;
    if (m_phocusCoords.col < 0) m_phocusCoords.col = 0;

    labelMap[m_phocusCoords]->setPhocus(true);

}

void ConfigPage::setName(QString name) {

    nameLabel->setValue(name);

}

