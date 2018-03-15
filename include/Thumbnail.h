#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include <QPalette>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>

#include "TrigRequest.h"
#include "Led.h"

class Thumbnail : public QFrame
{

    Q_OBJECT

    public:
        Thumbnail(int);
        int nsteps;
        void setInterest(bool);
        bool hasInterest(void);

    private:
        bool interest, m_enabled;
        QLabel *label;
        QPalette palette_light, palette_dark;
        std::vector<Led*> leds;
        Led *playheadLed;
        QString m_name;
        void toggleEnabling(void);

    protected:
        void mousePressEvent(QMouseEvent*);
        void contextMenuEvent(QContextMenuEvent*);

    public slots:
        void updatePlayhead(int);
        void setActivation(int, bool);
        void setName(QString);

    signals:
        void interestRequested(void);
        void deleteRequested(void);
        void enablingChanged(bool);

};

#endif
