#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QObject>
#include <QString>

#include "RtMidi.h"

#include "Trigger.h"
#include "TrigRequest.h"

class Sequence : public QObject
{
    Q_OBJECT

    public:
        Sequence(int);
        ~Sequence();

        static int Forward;
        static int Backward;
        static int Bounce;

    private:
        int m_nsteps;
        QString m_name;
        int playhead;
        int m_div, idiv;
        bool m_enabled;
        int m_transpose;
        int m_midiChan;
        int m_direction;
        bool m_bounceForward;

        std::vector<Trigger> trigs;

        RtMidiOut *midiout;
        void sendNoteOn(int);

        int subloop_start, subloop_stop;

    public slots:
        void setTrig(int, Trigger*);
        void tick(void);
        void setName(QString);
        void setSubloop_start(int);
        void setSubloop_stop(int);
        void setEnabling(bool);
        void setClockDiv(int);
        void setTranspose(int);
        void setMidiChan(int);
        void setDirection(QString);
        void reset(void);

    signals:
        void trigRequestAccepted(TrigRequest*);
        void playheadUpdated(int);
        void subloopChanged(int, int);
        void stepActivationChanged(int, bool);

};

#endif
