#ifndef TRIGGER_H
#define TRIGGER_H

#include <QJsonObject>

class Trigger
{

    public:
        Trigger(void);
        Trigger(const QJsonObject&);

        void setType(int);
        void setNoteValue(int);
        void setNoteVelocity(int);

        int type(void);
        int noteValue(void);
        int noteVelocity(void);

        static int Type_Null;
        static int Type_Note;

        bool operator==(Trigger);
        void write(QJsonObject&);

    private:

        int m_type;
        int m_noteValue;
        int m_noteVelocity;

};

#endif
