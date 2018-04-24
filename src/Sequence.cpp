#include <QDebug>

#include "Sequence.h"

extern QString MIDI_BACKEND;

int Sequence::Forward = 0;
int Sequence::Backward = 1;
int Sequence::Bounce = 2;

Sequence::Sequence(int nsteps) {

    m_nsteps = nsteps;

    // initialize sequence parameters
    playhead = 0;
    m_div = 1;
    idiv = 0;
    m_transpose = 0;
    m_midiChan = 1;

    m_mute = false;
    m_queue = false;

    m_direction = Sequence::Forward;
    m_bounceForward = true;

    subloop_start = 0;
    subloop_stop = m_nsteps - 1;

    // fill will null-type Trigs
    for (int i=0; i < m_nsteps; i++) {
        trigs.push_back(Trigger());
    }

    // RtMidi config
    try {

        if (MIDI_BACKEND == "jack") {
            midiout = new RtMidiOut(RtMidi::UNIX_JACK, "ziggurat");
        } else {
            midiout = new RtMidiOut(RtMidi::LINUX_ALSA, "ziggurat");
        }

        midiout->openPort(0, "default");

    } catch (RtMidiError &e) {

        qDebug() << "RtMidiError: " << e.what();

    }

}

Sequence::~Sequence(void) {

    delete midiout;

}

void Sequence::setName(QString name) {

    m_name = name;
    midiout->setPortName(name.toStdString());

}

void Sequence::setMute(bool mute) {

    m_mute = mute;

}

void Sequence::setQueue(bool queue) {

    m_queue = queue;

}

void Sequence::setTrig(int step, Trigger *trig) {

    // only emit a signal if the activation state has changed
    // i.e. from Null to Note, Note to Null, CC to Null, etc.
    if ((trigs[step].type()==Trigger::Type_Null)
            != (trig->type()==Trigger::Type_Null)) {

        emit stepActivationChanged(step, (trig->type() != Trigger::Type_Null));

    }

    trigs[step] = *trig;

}

void Sequence::tick(void) {

    if (idiv == 0) {

        // handle queue, if applicable
        if (m_queue && (playhead == subloop_start)) {

            m_mute = !m_mute;
            emit muteChanged(m_mute);

            m_queue = false;
            emit queueChanged(m_queue);

        }

        // only output MIDI if not muted
        // emit subloopCompleted signal for chaining
        if (!m_mute) {

            if (trigs[playhead].type() == Trigger::Type_Note) {
                sendNoteOn(trigs[playhead].note() + m_transpose);
            }

            if (playhead == subloop_stop) emit subloopCompleted();

        }

        // advance playhead according to direction mode
        if (m_direction == Sequence::Forward) {

            if (playhead == subloop_stop) {
                playhead = subloop_start;
            } else if (playhead == (m_nsteps-1)) {
                playhead = 0;
            } else {
                playhead++;
            }

        } else if (m_direction == Sequence::Backward) {

            if (playhead == subloop_start) {
                playhead = subloop_stop;
            } else if (playhead == 0) {
                playhead = m_nsteps - 1;
            } else {
                playhead--;
            }

        } else if (m_direction == Sequence::Bounce) {

            if (playhead == subloop_stop) {
                playhead--;
                m_bounceForward = false;
            } else if (playhead == subloop_start) {
                playhead++;
                m_bounceForward = true;
            } else {
                if (m_bounceForward) {
                    if (playhead == (m_nsteps-1)) {
                        playhead = 0;
                    } else {
                        playhead++;
                    }
                } else {
                    if (playhead == 0) {
                        playhead = m_nsteps - 1;
                    } else {
                        playhead--;
                    }
                }
            }

        }

        emit playheadUpdated(playhead);    

    }

    if (++idiv >= m_div) idiv = 0;

}

void Sequence::sendNoteOn(int note) {

    std::vector<unsigned char> message;
    message.push_back(143 + m_midiChan); // 144=chan1, 145=chan2, etc.
    message.push_back(note);
    message.push_back(100); // velocity 100 (hard-wired)
    midiout->sendMessage( &message );

}

void Sequence::setSubloop_start(int step) {

    subloop_start = step;

}

void Sequence::setSubloop_stop(int step) {

    subloop_stop = step;

}

void Sequence::setClockDiv(int div) {

    m_div = div;

}

void Sequence::setTranspose(int transpose) {

    m_transpose = transpose;

}

void Sequence::setMidiChan(int chan) {

    m_midiChan = chan;

}

void Sequence::setDirection(QString direction) {

    if (direction == "Forward") {
        m_direction = Sequence::Forward;
    } else if (direction == "Backward") {
        m_direction = Sequence::Backward;
    } else if (direction == "Bounce") {
        m_direction = Sequence::Bounce;
    }

}

void Sequence::reset(void) {

    playhead = subloop_start;
    idiv = 0;
    emit playheadUpdated(playhead);    

}

bool Sequence::isMuted(void) {

    return m_mute;

}
