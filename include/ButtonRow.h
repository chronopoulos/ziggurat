#ifndef BUTTONROW_H
#define BUTTONROW_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QJsonObject>

#include "ClickLabel.h"
#include "Button.h"

class ButtonRow : public QWidget
{
    Q_OBJECT

    public:
        ButtonRow(int);
        ButtonRow(const QJsonObject&);
        void setName(QString);

    private:
        int m_nsteps;
        ClickLabel *nameLabel;
        ClickLabel *lengthLabel;
        ClickLabel *clockDivLabel;
        ClickLabel *transposeLabel;
        ClickLabel *midiChanLabel;
        ClickLabel *directionLabel;
        std::vector<Button*> buttons;
        std::vector<Indicator*> indicators;

        Indicator *playheadIndicator;
        Indicator *lBracketIndicator;
        Indicator *rBracketIndicator;

    public slots:
        void updatePlayhead(int);
        void updateLBracket(int);
        void updateRBracket(int);

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
