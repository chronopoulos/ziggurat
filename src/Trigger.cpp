#include "Trigger.h"

int Trigger::Type_Null = -1;
int Trigger::Type_Note = 0;

Trigger::Trigger(int step) {

    m_step = step;
    setNull();

}

void Trigger::setNote(int note) {

    m_note = note;
    m_type = Trigger::Type_Note;

}

void Trigger::setNull(void) {

    m_type = Trigger::Type_Null;

}

int Trigger::step(void) {

    return m_step;

}

int Trigger::note(void) {

    if (m_type == Trigger::Type_Note) {
        return m_note;
    } else {
        return -1;
    }

}

int Trigger::type(void) {

    return m_type;

}

bool Trigger::operator==(Trigger trig) {

    if (trig.step() != m_step) return false;

    if (trig.type() == Trigger::Type_Note) {
        return (trig.note() == m_note);
    }

    return false;

}
