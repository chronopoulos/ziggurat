#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include "Session.h"
#include "Dialogs.h"
#include "Delta.h"

#include <QDebug>

extern Delta DELTA;

Session::Session(void) {

    selectedThumbnail = nullptr;
    selectNothing();

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

    DELTA.setState(true);

}

void Session::createSequenceInGroup(int nsteps, QString name) {

    GroupContainer *gcont = qobject_cast<GroupContainer*>(sender());
    SequenceContainer *scont = new SequenceContainer(nsteps, name);

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
        //newGcont->addScont(scont);
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

    QString filename;

    if (sessionFile.isNull()) {

        filename = QFileDialog::getSaveFileName(Q_NULLPTR, "Save Session", QDir::homePath());
        if (filename.isNull()) {
            return false;
        }

    } else {

        filename = sessionFile;

    }

    // open file
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    } else {
        qDebug() << "saving to: " << filename;
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
    emit sessionFileChanged(sessionFile);

    // reset flag
    DELTA.setState(false);

    return true;

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
        qWarning("Couldn't open load file.");
        return;
    } else {
        qDebug() << "loading: " << filename;
    }

    // check if format is correct
    QByteArray loadData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData));
    QJsonObject json(loadDoc.object());
    if (!json.contains("groups") || !json["groups"].isArray()) {
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
    emit sessionFileChanged(sessionFile);

    DELTA.setState(false);

}
