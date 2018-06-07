#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QGridLayout>
#include <QJsonObject>

#include "ClickLabel.h"
#include "Sequence.h"

class ConfigPage : public QWidget
{
    Q_OBJECT

    public:
        ConfigPage(int);
        ConfigPage(const QJsonObject&);
        ConfigPage(Sequence*);
        void phocusEvent(QKeyEvent*);
        void setName(QString);

    private:

        int m_nsteps;
        QGridLayout *layout;

        ClickLabel *nameLabel;
        ClickLabel *lengthLabel;
        ClickLabel *clockDivLabel;
        ClickLabel *transposeLabel;
        ClickLabel *midiChanLabel;
        ClickLabel *directionLabel;

        QLabel *defaultLabel;

    signals:
        void nameChanged(QString);
        void clockDivChanged(int);
        void transposeChanged(int);
        void midiChanChanged(int);
        void directionChanged(QString);

};

#endif
