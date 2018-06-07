#ifndef SESSION_H
#define SESSION_H

#include "SequenceContainer.h"
#include "GroupContainer.h"

class Session : public QObject
{
    Q_OBJECT

    public:
        Session(void);
        bool save(void);
        bool save(const QString&);
        bool saveAs(void);
        void load(void);
        void load(const QString&);
        void selectNothing(void);
        QString sessionFile;

        void phocusEvent(QKeyEvent*);

    private:
        void makeScontConnections(SequenceContainer*);
        void addScontToGcont(SequenceContainer*, GroupContainer*);
        void addGcont(GroupContainer*);

        std::vector<SequenceContainer*> sconts;
        std::vector<SequenceContainer*>::iterator scontIter;
        std::vector<GroupContainer*> gconts;
        std::vector<GroupContainer*>::iterator gcontIter;
        Thumbnail *selectedThumbnail;
        bool uninitialized;

        void advanceScontPhocus(int);
        void advanceGcontPhocus(int);
        int getPhocusCoordinates(int*, int*);
        SequenceContainer* getPhocusScont(void);

    public slots:
        void createGroup(void);
        void createSequenceInGroup(int, QString);
        void updateSelectedThumbnail(Thumbnail*);
        void tick(void);
        void resetAll(void);
        void deleteScont(SequenceContainer*);
        void deleteGcont(GroupContainer*);
        void handleTransfer(Thumbnail*, GroupContainer*, int);
        void handlePaste(GroupContainer*);

    signals:
        void groupWidgetCreated(GroupWidget*);
        void groupWidgetDeleted(GroupWidget*);
        void pageSelected(ConfigPage*);
        void rowSelected(ButtonRow*);
       
};

#endif
