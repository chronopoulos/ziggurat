#include "TrigRequest.h"

bool TrigRequest::Add= true;
bool TrigRequest::Remove = false;

TrigRequest::TrigRequest(Trigger trig, Cell *cell, bool request) :
    m_trig(trig), m_cell(cell), m_request(request) {};

Trigger TrigRequest::trig(void) {

    return m_trig;

}

Cell* TrigRequest::cell(void) {

    return m_cell;

}

bool TrigRequest::request(void) {

    return m_request;

}
