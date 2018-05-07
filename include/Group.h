#ifndef GROUP_H
#define GROUP_H

#include "SequenceContainer.h"

class Group : public QObject
{
    Q_OBJECT

    public:
        Group(void);
        void addScont(SequenceContainer*);
        void removeScont(SequenceContainer*);

        static int Type_Free;
        static int Type_Scene;
        static int Type_Mutex;
        static int Type_Chain;

    private:
        int m_type;
        std::vector<SequenceContainer*> sconts;
        std::vector<SequenceContainer*>::iterator scontIter;

    public slots:
        void handleMuteChange(bool);
        void handleSubloopCompleted(void);
        void setType(int);

};

#endif
