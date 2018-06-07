#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include "Session.h"
#include "Dialogs.h"
#include "Delta.h"

#include <QDebug>

// globals
Thumbnail *THUMB_CLIPBOARD = nullptr;
extern Delta DELTA;

Session::Session(void) {

    selectedThumbnail = nullptr;
    selectNothing();

    sessionFile = QDir::homePath().append("/untitled.zig");
    uninitialized = true;

}

void Session::phocusEvent(QKeyEvent *e) {

    if (e->key() == Qt::Key_Right) {
        advanceGcontPhocus(1);
    } else if (e->key() == Qt::Key_Left) {
        advanceGcontPhocus(-1);
    } else if (e->key() == Qt::Key_Up) {
        advanceScontPhocus(-1);
    } else if (e->key() == Qt::Key_Down) {
        advanceScontPhocus(1);
    } else if (!e->isAutoRepeat()) {

        if (e->key() == Qt::Key_M) {
            if (selectedThumbnail) getPhocusScont()->toggleMute();
        } else if (e->key() == Qt::Key_Q) {
            if (selectedThumbnail) getPhocusScont()->toggleQueue();
        } else if (e->key() == Qt::Key_Delete) {
            if (selectedThumbnail) deleteScont(getPhocusScont());
        }

    }

}

void Session::advanceScontPhocus(int increment) {
    
    int scontPos, gcontPos;
    if (getPhocusCoordinates(&gcontPos, &scontPos) >= 0) {

        int nsconts = gconts[gcontPos]->group->sconts.size();

        scontPos += increment;
        if (scontPos >= nsconts) scontPos = 0;
        if (scontPos < 0) scontPos = nsconts - 1;

        gconts[gcontPos]->group->sconts[scontPos]->select();

    }

}

void Session::advanceGcontPhocus(int increment) {

    int scontPos, gcontPos;
    if (getPhocusCoordinates(&gcontPos, &scontPos) >= 0) {

        gcontPos += increment;
        if (gcontPos >= (int)gconts.size()) gcontPos = 0;
        if (gcontPos < 0) gcontPos = (int)(gconts.size() - 1);

        GroupContainer *gcont = gconts[gcontPos];
        int gcontSize = (int) (gcont->group->sconts.size());
        
        if (gcontSize == 0) {
            qDebug() << "TODO: empty group phocus";
            return;
        }

        if (scontPos >= gcontSize) {
            scontPos = gcontSize - 1;
        }

        gcont->group->sconts[scontPos]->select();

    } else {

        qDebug() << "couldn't find phocus";

    }

}

int Session::getPhocusCoordinates(int *gcontPosition, int *scontPosition) {

    SequenceContainer *phocusedScont = getPhocusScont();
    if (!phocusedScont) return -1;

    // find the phocused gcont
    for (gcontIter = gconts.begin(); gcontIter != gconts.end(); gcontIter++) {

        scontIter = std::find((*gcontIter)->group->sconts.begin(),
                                (*gcontIter)->group->sconts.end(), phocusedScont);

        if (scontIter != (*gcontIter)->group->sconts.end()) {
            // find the positions (indices)
            *scontPosition = scontIter - (*gcontIter)->group->sconts.begin();
            *gcontPosition = gcontIter - gconts.begin();
            break;
        }

    }

    if (scontIter == sconts.end()) return -1;

    return 0;

}

SequenceContainer* Session::getPhocusScont(void) {

    for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
        if ((*scontIter)->thumb == selectedThumbnail) {
            return *scontIter;
        }
    }

    return nullptr;

}

void Session::createGroup(void) {

    GroupContainer *gcont = new GroupContainer();
    addGcont(gcont);

}

void Session::addGcont(GroupContainer *gcont) {

    gconts.push_back(gcont);
    emit groupWidgetCreated(gcont->groupWidget);

    connect(gcont, SIGNAL(newSequenceRequested(int, QString)),
            this, SLOT(createSequenceInGroup(int, QString)));
    connect(gcont, SIGNAL(deleteRequested(GroupContainer*)),
            this, SLOT(deleteGcont(GroupContainer*)));
    connect(gcont, SIGNAL(transferRequested(Thumbnail*, GroupContainer*, int)),
            this, SLOT(handleTransfer(Thumbnail*, GroupContainer*, int)));
    connect(gcont, SIGNAL(pasteRequested(GroupContainer*)),
            this, SLOT(handlePaste(GroupContainer*)));

    DELTA.setState(true);

}

void Session::createSequenceInGroup(int nsteps, QString name) {

    GroupContainer *gcont = qobject_cast<GroupContainer*>(sender());
    SequenceContainer *scont = new SequenceContainer(nsteps, name);

    addScontToGcont(scont, gcont);

}

void Session::addScontToGcont(SequenceContainer *scont, GroupContainer *gcont) {

    sconts.push_back(scont);
    gcont->addScont(scont);
    makeScontConnections(scont);

    // if this is the only sequence in the session, select it
    if (sconts.size() == 1) scont->select();

    DELTA.setState(true);

}

void Session::makeScontConnections(SequenceContainer *scont) {

    // selection
    connect(scont, SIGNAL(thumbnailSelected(Thumbnail*)),
            this, SLOT(updateSelectedThumbnail(Thumbnail*)));

    // passthroughs
    connect(scont, SIGNAL(pageSelected(ConfigPage*)),
            this, SIGNAL(pageSelected(ConfigPage*)));
    connect(scont, SIGNAL(rowSelected(ButtonRow*)),
            this, SIGNAL(rowSelected(ButtonRow*)));

    // delete
    connect(scont, SIGNAL(deleteRequested(SequenceContainer*)),
            this, SLOT(deleteScont(SequenceContainer*)));

}

void Session::deleteScont(SequenceContainer *scont) {

    // remove scont from all groups, and groupWidgets
    for (gcontIter = gconts.begin(); gcontIter != gconts.end(); gcontIter++) {
        (*gcontIter)->group->removeScont(scont);
        (*gcontIter)->groupWidget->removeThumbnail(scont->thumb);
    }

    // remove scont from sconts
    scontIter = std::find(sconts.begin(), sconts.end(), scont);
    sconts.erase(scontIter);

    // if the sequence was selected, select another sequence
    // (or set null state)
    if (scont->selected()) {
        if (!sconts.empty()) {
            scontIter = sconts.begin();
            (*scontIter)->select();
        } else {
            selectNothing();
        }
    }

    delete scont;

    DELTA.setState(true);

}

void Session::handleTransfer(Thumbnail *thumb, GroupContainer *newGcont, int insertIndex) {

    // find the scont
    SequenceContainer *scont = nullptr;
    for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
        if ((*scontIter)->thumb == thumb) {
            scont = *scontIter;
            break;
        }
    }

    // find the old gcont
    GroupContainer *oldGcont = nullptr;
    for (gcontIter = gconts.begin(); gcontIter != gconts.end(); gcontIter++) {

        scontIter = std::find((*gcontIter)->group->sconts.begin(),
                                (*gcontIter)->group->sconts.end(), scont);

        if (scontIter != (*gcontIter)->group->sconts.end()) {
            oldGcont = *gcontIter;
            break;
        }

    }

    // do the transfer
    if (scont && oldGcont) {

        oldGcont->removeScont(scont);
        newGcont->addScontAt(scont, insertIndex);

    }

}

void Session::handlePaste(GroupContainer *gcont) {

    if (THUMB_CLIPBOARD) {

        // find the scont
        SequenceContainer *scont = nullptr;
        for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
            if ((*scontIter)->thumb == THUMB_CLIPBOARD) {
                scont = *scontIter;
                break;
            }
        }

        if (scont) {

            // copy it, and add it
            SequenceContainer *newScont = new SequenceContainer(scont);

            addScontToGcont(newScont, gcont);

        }


    }

}

void Session::selectNothing(void) {

    if (selectedThumbnail) selectedThumbnail->deselect();

    emit pageSelected(nullptr);
    emit rowSelected(nullptr);
    selectedThumbnail = nullptr;

}

void Session::deleteGcont(GroupContainer *gcont) {

    // remove groupWidget from groupManager
    emit groupWidgetDeleted(gcont->groupWidget);
    
    // remove gcont from gconts
    gcontIter = std::find(gconts.begin(), gconts.end(), gcont);
    gconts.erase(gcontIter);

    delete gcont;

    DELTA.setState(true);

}

void Session::updateSelectedThumbnail(Thumbnail *thumb) {

    if (thumb != selectedThumbnail) {
        if (selectedThumbnail) selectedThumbnail->deselect();
        selectedThumbnail = thumb;
    }

}

void Session::tick(void) {

    for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
        (*scontIter)->seq->tick();
    }

}

void Session::resetAll(void) {

    for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
        (*scontIter)->seq->reset();
    }

}

bool Session::save(void) {

    if (uninitialized) {
        return saveAs();
    } else {
        return save(sessionFile);
    }

}

bool Session::save(const QString &filename) {

    // open file
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)) {

        qWarning("Couldn't open save file.");
        return false; // TODO present warning dialog

    }

    QJsonObject sessionObject;

    // save all groups
    QJsonArray groupArray;
    QJsonObject groupObject;
    for (gcontIter = gconts.begin(); gcontIter != gconts.end(); gcontIter++) {
        (*gcontIter)->group->write(groupObject);
        groupArray.append(groupObject);
    }
    sessionObject["groups"] = groupArray;

    // write to file
    QJsonDocument saveDoc(sessionObject);
    saveFile.write(saveDoc.toJson());

    sessionFile = filename;

    // reset flags
    DELTA.setState(false);
    uninitialized = false;

    return true;

}

bool Session::saveAs(void) {

    QString filename = QFileDialog::getSaveFileName(Q_NULLPTR, "Save Session", sessionFile);
    if (filename.isNull()) {
        return false;
    }

    return save(filename);

}

void Session::load(void) {

    // if there are changes to be saved, ask to save them
    if (DELTA.state()) {

        MaybeSaveDialog dlg;
        switch (dlg.exec()) {
            case -1: // Discard
                break;
            case 0: // Cancel
                return;
            case 1: // Save
                if (save()) {
                    break;
                } else {
                    return;
                }
        }

    }

    QString filename = QFileDialog::getOpenFileName(Q_NULLPTR, "Open Session", QDir::homePath());

    if (!filename.isNull()) {
        load(filename);
    }

}

void Session::load(const QString &filename) {

    // open file
    QFile loadFile(filename);
    if (!loadFile.open(QIODevice::ReadOnly)) {

        qWarning("Couldn't open load file."); // TODO present warning dialog
        return;

    }

    // check if format is correct
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData));
    QJsonObject json(loadDoc.object());
    if (!json.contains("groups") || !json["groups"].isArray()) {

        // TODO present warning dialog
        qDebug() << "Load file " << filename << "has invalid format";
        return;

    }


    // nuke the current session
    selectNothing();
    for (scontIter = sconts.begin(); scontIter != sconts.end(); scontIter++) {
        delete *scontIter;
    }
    sconts.clear();
    for (gcontIter = gconts.begin(); gcontIter != gconts.end(); gcontIter++) {
        delete *gcontIter;
    }
    gconts.clear();

    

    // load all groups and sequences from file
    QJsonArray groupJSA = json["groups"].toArray();
    GroupContainer *gcont;
    for (int i = 0; i < groupJSA.size(); i++) {

        gcont = new GroupContainer(groupJSA[i].toObject());

        // some redundancy here: store groups in session->sconts as well as group->sconts
        for (scontIter = gcont->group->sconts.begin(); scontIter != gcont->group->sconts.end(); scontIter++) {
            sconts.push_back(*scontIter);
            makeScontConnections(*scontIter);
        }

        addGcont(gcont);
        
    }

    if (!sconts.empty()) {
        scontIter = sconts.begin();
        (*scontIter)->select();  
    }

    sessionFile = filename;

    DELTA.setState(false);
    uninitialized = false;

}
