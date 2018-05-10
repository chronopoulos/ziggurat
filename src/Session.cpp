#include <QJsonDocument>
#include <QJsonArray>
#include "Session.h"

#include <QDebug>

Session::Session(void) {

    selectedThumbnail = nullptr;

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

}

void Session::createSequenceInGroup(int nsteps, QString name) {

    GroupContainer *gcont = qobject_cast<GroupContainer*>(sender());
    SequenceContainer *scont = new SequenceContainer(nsteps, name);

    sconts.push_back(scont);

    gcont->addScont(scont);

    makeScontConnections(scont);

    // if this is the only sequence in the session, select it
    if (sconts.size() == 1) scont->select();

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
            emit pageSelected(nullptr);
            emit rowSelected(nullptr);
            selectedThumbnail = nullptr;
        }
    }

    delete scont;

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

void Session::save(QString filename) {

    // open file
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
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

}

void Session::load(QString filename) {

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

    // main loop
    QJsonArray groupJSA = json["groups"].toArray();
    GroupContainer *gcont;
    for (int i = 0; i < groupJSA.size(); i++) {

        gcont = new GroupContainer(groupJSA[i].toObject());

        // some redundancy here
        for (scontIter = gcont->group->sconts.begin(); scontIter != gcont->group->sconts.end(); scontIter++) {
            sconts.push_back(*scontIter);
            makeScontConnections(*scontIter);
            if (sconts.size() == 1) (*scontIter)->select();
        }

        addGcont(gcont);
        
    }

    // update GUI
    for (gcontIter = gconts.begin(); gcontIter != gconts.end(); gcontIter++) {
        emit groupWidgetCreated((*gcontIter)->groupWidget);
    }

}
