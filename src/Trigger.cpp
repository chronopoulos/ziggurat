#include "Trigger.h"

#include <QDebug>

int Trigger::Type_Null = -1;
int Trigger::Type_Note = 0;

#define DEFAULT_NOTEVALUE 60
#define DEFAULT_NOTEVELOCITY 100

Trigger::Trigger(void) {

    m_type = Trigger::Type_Null;

    m_noteValue = DEFAULT_NOTEVALUE;
    m_noteVelocity = DEFAULT_NOTEVELOCITY;

}

Trigger::Trigger(const QJsonObject &trigJSO) {

    setType(trigJSO["type"].toInt(Trigger::Type_Null));
    setNoteValue(trigJSO["note"].toInt(DEFAULT_NOTEVALUE));
    setNoteVelocity(trigJSO["velocity"].toInt(DEFAULT_NOTEVELOCITY));

}

void Trigger::setType(int type) {

    m_type = type;

}

void Trigger::setNoteValue(int noteVal) {

    m_noteValue = noteVal;

}

void Trigger::setNoteVelocity(int noteVel) {

    m_noteVelocity = noteVel;

}

int Trigger::type(void) {

    return m_type;

}

int Trigger::noteValue(void) {

        return m_noteValue;

}

int Trigger::noteVelocity(void) {

        return m_noteVelocity;

}

bool Trigger::operator==(Trigger trig) {

    if (trig.type() == Trigger::Type_Note) {
        return (trig.noteValue() == m_noteValue);
        return (trig.noteVelocity() == m_noteVelocity);
    }

    if (trig.type() == Trigger::Type_Null) {
        return true;
    }

    return false;

}

void Trigger::write(QJsonObject &trigJsonObject) {

    trigJsonObject["type"] = m_type;
    trigJsonObject["note"] = m_noteValue;
    trigJsonObject["velocity"] = m_noteVelocity;

}

