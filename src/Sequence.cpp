#include <QDebug>
#include <QJsonArray>

#include "Sequence.h"

extern QString MIDI_BACKEND;
extern bool DELTA;

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

    m_subloopStart = 0;
    m_subloopStop = m_nsteps - 1;

    // fill will null-type Trigs
    for (int i=0; i < m_nsteps; i++) {
        trigs.push_back(Trigger());
    }

    configureMidi();

}

Sequence::Sequence(const QJsonObject &seqJSO) {

    // sequence parameters, from JSON
    m_nsteps = seqJSO["nsteps"].toInt();
    m_name = seqJSO["name"].toString();
    m_div = seqJSO["div"].toInt();
    m_mute = seqJSO["mute"].toBool();
    m_transpose = seqJSO["transpose"].toInt();
    m_midiChan = seqJSO["midiChan"].toInt();
    m_direction = seqJSO["direction"].toInt();
    m_subloopStart = seqJSO["subloopStart"].toInt();
    m_subloopStop = seqJSO["subloopStop"].toInt();

    // dynamic parameters, default initialization
    playhead = 0;
    m_queue = false;
    idiv = 0;
    m_bounceForward = true;

    // trigs
    QJsonArray trigJSA = seqJSO["trigs"].toArray();
    for (int i = 0; i < trigJSA.size(); i++) {
        trigs.push_back(Trigger(trigJSA[i].toObject()));
    }

    configureMidi();

}

void Sequence::configureMidi(void) {

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

    DELTA = true;

}

void Sequence::setMute(bool mute) {

    m_mute = mute;

    DELTA = true;

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

    DELTA = true;

}

void Sequence::tick(void) {

    if (idiv == 0) {

        // handle queue, if applicable
        if (m_queue && (playhead == m_subloopStart)) {

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

            if (playhead == m_subloopStop) emit subloopCompleted();

        }

        // advance playhead according to direction mode
        if (m_direction == Sequence::Forward) {

            if (playhead == m_subloopStop) {
                playhead = m_subloopStart;
            } else if (playhead == (m_nsteps-1)) {
                playhead = 0;
            } else {
                playhead++;
            }

        } else if (m_direction == Sequence::Backward) {

            if (playhead == m_subloopStart) {
                playhead = m_subloopStop;
            } else if (playhead == 0) {
                playhead = m_nsteps - 1;
            } else {
                playhead--;
            }

        } else if (m_direction == Sequence::Bounce) {

            if (playhead == m_subloopStop) {
                playhead--;
                m_bounceForward = false;
            } else if (playhead == m_subloopStart) {
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

    midiBuf[0] = 143 + m_midiChan; // 144=chan1, 145=chan2, etc.
    midiBuf[1] = note;
    midiBuf[2] = 100; // velocity 100 (hard-wired for now)
    midiout->sendMessage(midiBuf, 3);
}

void Sequence::setSubloop_start(int step) {

    m_subloopStart = step;

    DELTA = true;

}

void Sequence::setSubloop_stop(int step) {

    m_subloopStop = step;

    DELTA = true;

}

void Sequence::setClockDiv(int div) {

    m_div = div;

    DELTA = true;

}

void Sequence::setTranspose(int transpose) {

    m_transpose = transpose;

    DELTA = true;

}

void Sequence::setMidiChan(int chan) {

    m_midiChan = chan;

    DELTA = true;

}

void Sequence::setDirection(QString direction) {

    if (direction == "Forward") {
        m_direction = Sequence::Forward;
    } else if (direction == "Backward") {
        m_direction = Sequence::Backward;
    } else if (direction == "Bounce") {
        m_direction = Sequence::Bounce;
    }

    DELTA = true;

}

void Sequence::reset(void) {

    playhead = m_subloopStart;
    idiv = 0;
    emit playheadUpdated(playhead);    

}

bool Sequence::isMuted(void) {

    return m_mute;

}

void Sequence::write(QJsonObject &seqJsonObject) {

    /* don't save playhead, m_queue, idiv, or m_bounceForward,
        as these will change during playback
    */

    seqJsonObject["nsteps"] = m_nsteps;
    seqJsonObject["name"] = m_name;
    seqJsonObject["div"] = m_div;
    seqJsonObject["mute"] = m_mute;
    seqJsonObject["transpose"] = m_transpose;
    seqJsonObject["midiChan"] = m_midiChan;
    seqJsonObject["direction"] = m_direction;
    seqJsonObject["subloopStart"] = m_subloopStart;
    seqJsonObject["subloopStop"] = m_subloopStop;

    QJsonArray trigJsonArray;
    QJsonObject trigJsonObject;
    for (trigIter = trigs.begin(); trigIter != trigs.end(); trigIter++) {
        (*trigIter).write(trigJsonObject);
        trigJsonArray.append(trigJsonObject);
    }

    seqJsonObject["trigs"] = trigJsonArray;

}
