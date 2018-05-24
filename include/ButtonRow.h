#ifndef BUTTONROW_H
#define BUTTONROW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QJsonObject>

#include "Button.h"
#include "Sequence.h"

class ButtonRow : public QWidget
{
    Q_OBJECT

    public:
        ButtonRow(int);
        ButtonRow(const QJsonObject&);
        ButtonRow(Sequence*);
        void setName(QString);

    private:
        int m_nsteps;
        std::vector<Button*> buttons;
        std::vector<Indicator*> indicators;

        QComboBox *editParameterCombo;

        Indicator *playheadIndicator;
        Indicator *lBracketIndicator;
        Indicator *rBracketIndicator;

    public slots:
        void updatePlayhead(int);
        void updateLBracket(int);
        void updateRBracket(int);
        void setEditParameter(int index);

    signals:
        void trigSet(int, Trigger*);
        void nameChanged(QString);
        void clockDivChanged(int);
        void transposeChanged(int);
        void midiChanChanged(int);
        void directionChanged(QString);
        void lBracketChanged(int);
        void rBracketChanged(int);

};

#endif
