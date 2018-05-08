#ifndef TRIGGER_H
#define TRIGGER_H

#include <QJsonObject>

class Trigger
{

    public:
        Trigger(void);
        void setNote(int);
        void setNull(void);

        int note(void);
        int type(void);

        static int Type_Null;
        static int Type_Note;

        bool operator==(Trigger);

        void write(QJsonObject&);

    private:

        int m_note;
        int m_type;

};

#endif
