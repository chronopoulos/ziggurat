#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QGridLayout>
#include <QJsonObject>

#include "ClickLabel.h"
#include "Sequence.h"

struct coord
{
    int row;
    int col;

    bool  operator==(const coord &o) const {
        return (row == o.row) && (col == o.col);
    }

    bool operator<(const coord &o) const {
        return (row < o.row) || ((row == o.row) && (col < o.col));
    }

};

class ConfigPage : public QWidget
{
    Q_OBJECT

    public:
        ConfigPage(int);
        ConfigPage(const QJsonObject&);
        ConfigPage(Sequence*);
        void phocusEvent(QKeyEvent*);
        void advancePhocusRow(int);
        void advancePhocusCol(int);
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

        coord m_phocusCoords;
        std::map <coord, ClickLabel*> labelMap;

    signals:
        void nameChanged(QString);
        void clockDivChanged(int);
        void transposeChanged(int);
        void midiChanChanged(int);
        void directionChanged(QString);

};

#endif
