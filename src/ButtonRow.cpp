#include "ButtonRow.h"

#include <QLabel>
#include <QJsonArray>
#include <QApplication>

#include <QDebug>

ButtonRow::ButtonRow(int nsteps) : QWidget() {

    m_nsteps = nsteps;

    QVBoxLayout *layout = new QVBoxLayout();

    if (m_nsteps > 0) {

        QHBoxLayout *topLayout = new QHBoxLayout();
        QHBoxLayout *middleLayout = new QHBoxLayout();
        QHBoxLayout *bottomLayout = new QHBoxLayout();

        playheadIndicator = nullptr;
        lBracketIndicator = nullptr;
        rBracketIndicator = nullptr;

        editParameterCombo = new QComboBox();
        editParameterCombo->addItem("Editing: Note Value");
        editParameterCombo->addItem("Editing: Note Velocity");
        editParameterCombo->setFocusPolicy(Qt::NoFocus);
        editParameterCombo->setMaximumWidth(200);
        connect(editParameterCombo, SIGNAL(currentIndexChanged(int)),
                this, SLOT(setEditParameter(int)));

        topLayout->addWidget(editParameterCombo);
        topLayout->setAlignment(Qt::AlignRight);

        QSpacerItem *spacer = new QSpacerItem(0,10);

        phocusIndex = -1;

        Button *tmpButton;
        Indicator *tmpIndicator;
        for (int i=0; i < m_nsteps; i++) {

            tmpButton = new Button(i);
            if (i==0) {
                tmpButton->setPhocus(true);
                phocusIndex = i;
            }
            buttons.push_back(tmpButton);
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
        layout->addItem(spacer);
        layout->addLayout(middleLayout);
        layout->addLayout(bottomLayout);

    } else {

        QLabel *defaultLabel = new QLabel("No Sequence Selected");
        defaultLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(defaultLabel);

    }

    setLayout(layout);

}

ButtonRow::ButtonRow(const QJsonObject &seqJSO) : ButtonRow(seqJSO["nsteps"].toInt()) {

    QJsonArray trigJSA = seqJSO["trigs"].toArray();
    for (int i = 0; i < trigJSA.size(); i++) {

        buttons[i]->setTrig(Trigger(trigJSA[i].toObject()));

        if (i == seqJSO["subloopStart"].toInt()) {
            indicators[i]->setLBracket(true);
        } else if (i == seqJSO["subloopStop"].toInt()) {
            indicators[i]->setRBracket(true);
        }

    }

}

// pseudo-copy constructor
ButtonRow::ButtonRow(Sequence *seq) : ButtonRow(seq->nsteps()) {

    for (int i = 0; i < m_nsteps; i++) {

        buttons[i]->setTrig(seq->trig(i));

        if (i == seq->subloopStart()) {
            indicators[i]->setLBracket(true);
        } else if (i == seq->subloopStop()) {
            indicators[i]->setRBracket(true);
        }

    }

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

void ButtonRow::setEditParameter(int index) {

    std::vector<Button*>::iterator buttonIter;
    for (buttonIter = buttons.begin(); buttonIter != buttons.end(); buttonIter++) {
        (*buttonIter)->setEditParameter(index);
    }
    
}

void ButtonRow::phocusEvent(QKeyEvent *e) {

    if (e->key() == Qt::Key_Right) {
        advancePhocus(1);
    } else if (e->key() == Qt::Key_Left) {
        advancePhocus(-1);
    } else if (e->key() == Qt::Key_Up) {

        if (QApplication::keyboardModifiers() == Qt::ShiftModifier) {
            buttons[phocusIndex]->adjustEditParameter(4);
        } else {
            buttons[phocusIndex]->adjustEditParameter(1);
        }

    } else if (e->key() == Qt::Key_Down) {

        if (QApplication::keyboardModifiers() == Qt::ShiftModifier) {
            buttons[phocusIndex]->adjustEditParameter(-4);
        } else {
            buttons[phocusIndex]->adjustEditParameter(-1);
        }

    } else if (!e->isAutoRepeat()) {

        if (e->key() == Qt::Key_T) {
            buttons[phocusIndex]->toggle();
        } else if (e->key() == Qt::Key_Tab) {

            int editIndex = editParameterCombo->currentIndex();
            editIndex = (editIndex + 1) % 2;
            editParameterCombo->setCurrentIndex(editIndex);

        }

    }

}

void ButtonRow::advancePhocus(int increment) {

    if (phocusIndex >= 0) {

        buttons[phocusIndex]->setPhocus(false);
        phocusIndex = phocusIndex + increment;
        if (phocusIndex >= m_nsteps) phocusIndex = m_nsteps - 1;
        if (phocusIndex < 0) phocusIndex = 0;
        buttons[phocusIndex]->setPhocus(true);

    }

}
