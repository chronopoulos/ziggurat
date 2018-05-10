#include "Trigger.h"

int Trigger::Type_Null = -1;
int Trigger::Type_Note = 0;

Trigger::Trigger(void) {

    setNull();

}

Trigger::Trigger(const QJsonObject &trigJSO) {

    setType(trigJSO["type"].toInt());
    if (m_type == Trigger::Type_Note) {
        setNote(trigJSO["note"].toInt());
    }

}

void Trigger::setNote(int note) {

    m_note = note;
    m_type = Trigger::Type_Note;

}

// convenience function
void Trigger::setNull(void) {

    m_type = Trigger::Type_Null;

}

void Trigger::setType(int type) {

    m_type = type;

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

    if (trig.type() == Trigger::Type_Note) {
        return (trig.note() == m_note);
    }

    if (trig.type() == Trigger::Type_Null) {
        return true;
    }

    return false;

}

void Trigger::write(QJsonObject &trigJsonObject) {

    trigJsonObject["type"] = m_type;
    trigJsonObject["note"] = m_note;

}
