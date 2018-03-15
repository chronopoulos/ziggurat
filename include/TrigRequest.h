#ifndef TRIGREQUEST_H
#define TRIGREQUEST_H

#include <QObject>

#include "Trigger.h"

// forward declaration
class Cell;

class TrigRequest
{

    public:
        TrigRequest(Trigger, Cell*, bool);
        Trigger trig();
        Cell *cell();
        bool request();
        static bool Add;
        static bool Remove;

    private:
        Trigger m_trig;
        Cell *m_cell;
        bool m_request;

};

#endif
