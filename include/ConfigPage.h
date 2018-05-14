#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QGridLayout>
#include <QJsonObject>

#include "ClickLabel.h"

class ConfigPage : public QWidget
{
    Q_OBJECT

    public:
        ConfigPage(int);
        ConfigPage(const QJsonObject&);
        void setName(QString);

        ClickLabel *nameLabel; // tmp here

        int m_nsteps; // tmp here
    private:

        QGridLayout *layout;

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
