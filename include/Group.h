#ifndef GROUP_H
#define GROUP_H

#include "SequenceContainer.h"

class Group
{
    public:
        Group(void);

    private:
        std::vector<Sequence*> seqs;
        std::vector<Sequence*>::iterator seqIter;

};

#endif
