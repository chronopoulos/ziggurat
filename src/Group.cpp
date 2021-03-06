#include <QJsonArray>

#include "Group.h"
#include "Delta.h"

#include <QDebug>

extern Delta DELTA;

int Group::Type_Free = 0;
int Group::Type_Scene = 1;
int Group::Type_Mutex = 2;
int Group::Type_Chain = 3;

Group::Group(void) {

    m_type = Group::Type_Free;

}

void Group::setType(int type) {

    m_type = type;

    /*
        initialize the mute states for the group type. the idea here is that
        type selection brings the group into compliance with the type, in such
        a way as to never surprise the user with additional sound. so sequences
        get muted, but not un-muted here.
    */

    if (m_type == Group::Type_Scene) {
        bool anyMuted = false;
        for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
            anyMuted = ((*scontIter)->seq->isMuted() || anyMuted);
        }
        if (anyMuted) {
            for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
                (*scontIter)->setMute(true);
            }
        }
    }

    if ((m_type == Group::Type_Mutex) || (m_type == Group::Type_Chain)) {
        // keep the first unmuted sequence as the singleton, and mute the rest
        bool singletonFound = false;
        for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
            if (singletonFound) {
                (*scontIter)->setMute(true);
            } else if (!(*scontIter)->seq->isMuted()) {
                singletonFound = true;
            }
            
        }
    }

    DELTA.setState(true);

}

void Group::addScont(SequenceContainer *scont) {

    sconts.push_back(scont);

    DELTA.setState(true);

}

void Group::addScontAt(SequenceContainer *scont, int insertIndex) {

    sconts.insert(sconts.begin() + insertIndex, scont);

    DELTA.setState(true);

}

void Group::removeScont(SequenceContainer *scont) {

    scontIter = std::find(sconts.begin(), sconts.end(), scont);
    if (scontIter != sconts.end()) {
        sconts.erase(scontIter);
    }

    DELTA.setState(true);

}

SequenceContainer* Group::getNextScont(SequenceContainer *scont, int increment) {

    scontIter = std::find(sconts.begin(), sconts.end(), scont);

    if (scontIter != sconts.end()) {
        if ((scontIter == sconts.begin()) && (increment < 0)) { // better way?
            return sconts.back();
        } else {
            scontIter = std::next(scontIter, increment);
            if (scontIter == sconts.end()) scontIter = sconts.begin();
            return *scontIter;
        }
    }

    return nullptr;
}

void Group::handleMuteChange(bool mute) {

    SequenceContainer *scont = qobject_cast<SequenceContainer*>(sender());

    // if the group is a scene, and we just got a mute change, then apply
    //  that change to all other sconts
    if (m_type == Group::Type_Scene) {
        for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
            if (scont != (*scontIter)) {
                (*scontIter)->setMute(mute);
            }
        }
    }

    // if the group is a mutex/chain, and we just got an un-muting signal, then
    //  mute all other sconts
    if ((m_type == Group::Type_Mutex) || (m_type == Group::Type_Chain)) {
        if (!mute) {
            for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
                if (scont != (*scontIter)) {
                    (*scontIter)->setMute(true);
                }
            }
        }
    } 

}

void Group::handleSubloopCompleted(void) {

    if (m_type == Group::Type_Chain) {

        SequenceContainer *scont = qobject_cast<SequenceContainer*>(sender());

        scontIter = std::find(sconts.begin(), sconts.end(), scont);
        if ((++scontIter)==sconts.end()) scontIter = sconts.begin();

        scont->setMute(true);
        (*scontIter)->seq->setQueue(true);
 
    }

}

void Group::write(QJsonObject &groupJsonObject) {

    groupJsonObject["type"] = m_type;

    // save all sequences
    QJsonArray seqArray;
    QJsonObject seqObject;
    for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
        (*scontIter)->seq->write(seqObject);
        seqArray.append(seqObject);
    }

    groupJsonObject["seqs"] = seqArray;


}

