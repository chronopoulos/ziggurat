#include "Delta.h"

Delta::Delta(void) {

    m_state = false;

}

void Delta::setState(bool state) {

    m_state = state;
    emit stateChanged(m_state);

}

bool Delta::state(void) {

    return m_state;

}
