#include "Trigger.h"

int Trigger::Type_Note = 0;

Trigger::Trigger(int step) {

    m_step = step;

}

void Trigger::setNote(int note) {

    m_note = note;

    m_type = Trigger::Type_Note;

}

int Trigger::step(void) {

    return m_step;

}

int Trigger::note(void) {

    return m_note;

}

int Trigger::type(void) {

    return m_type;

}

//bool Trigger::operator==(const Trigger& trig) {
bool Trigger::operator==(Trigger trig) {

    if (trig.step() != m_step) return false;

    if (trig.type() == Trigger::Type_Note) {
        return (trig.note() == m_note);
    }

    return false;

}
