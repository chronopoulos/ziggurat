#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QObject>
#include <QString>
#include <QJsonObject>

#include "RtMidi.h"

#include "Trigger.h"

class Sequence : public QObject
{
    Q_OBJECT

    public:
        Sequence(int);
        Sequence(const QJsonObject&);
        Sequence(Sequence*);
        ~Sequence(void);
        bool isMuted(void);
        bool isQueued(void);

        static int Forward;
        static int Backward;
        static int Bounce;

        void write(QJsonObject&);

    private:
        int m_nsteps;
        QString m_name;
        int playhead;
        int m_div, idiv;
        bool m_mute;
        bool m_queue;
        int m_transpose;
        int m_midiChan;
        int m_direction;
        bool m_bounceForward;
        int m_subloopStart, m_subloopStop;

        std::vector<Trigger> trigs;
        std::vector<Trigger>::iterator trigIter;

        RtMidiOut *midiout;
        unsigned char midiBuf[3];
        void sendNoteOn(int, int);
        void configureMidi(void);


    public slots:

        void tick(void);
        void reset(void);

        // setters
        void setTrig(int, Trigger*);
        void setMute(bool);
        void setQueue(bool);
        void setName(QString);
        void setSubloop_start(int);
        void setSubloop_stop(int);
        void setClockDiv(int);
        void setTranspose(int);
        void setMidiChan(int);
        void setDirection(QString);

        // getters
        int nsteps(void) {return m_nsteps;}
        QString name(void) {return m_name;}
        int div(void) {return m_div;}
        bool mute(void) {return m_mute;}
        int transpose(void) {return m_transpose;}
        int midiChan(void) {return m_midiChan;}
        int direction(void) {return m_direction;}
        int subloopStart(void) {return m_subloopStart;}
        int subloopStop(void) {return m_subloopStop;}
        Trigger trig(int i) {return trigs[i];}

    signals:
        void playheadUpdated(int);
        void stepActivationChanged(int, bool);
        void subloopCompleted(void);
        void muteChanged(bool);
        void queueChanged(bool);

};

#endif
