#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QPalette>
#include <QHBoxLayout>
#include <QMenu>
#include <QAction>
#include <QJsonObject>

#include "Led.h"

class Thumbnail : public QFrame
{

    Q_OBJECT

    public:
        Thumbnail(int);
        Thumbnail(const QJsonObject&);
        int nsteps;
        bool isSelected;
        void select(void);
        void deselect(void);

    private:
        bool m_enabled;
        QLabel *label;
        QPushButton *muteButton, *queueButton;
        QPalette palette_light, palette_dark;
        std::vector<Led*> leds;
        Led *playheadLed;
        QString m_name;

    protected:
        void mousePressEvent(QMouseEvent*);
        void contextMenuEvent(QContextMenuEvent*);

    public slots:
        void updatePlayhead(int);
        void setActivation(int, bool);
        void setName(QString);

        void setMute(bool);
        void setQueue(bool);

    private slots:
        void handleMuteClick(bool);
        void handleQueueClick(bool);

    signals:
        void selected(void);
        void deleteRequested(void);

        void muteChanged(bool);
        void queueChanged(bool);

};

#endif
