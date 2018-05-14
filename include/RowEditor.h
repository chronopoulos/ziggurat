#include <QFrame>
#include <QVBoxLayout>

#include "ButtonRow.h"

class RowEditor : public QFrame
{
    Q_OBJECT

    public:
        RowEditor(void);

    private:
        QVBoxLayout *layout;
        ButtonRow *currentRow, *defaultRow;

    public slots:
        void setRow(ButtonRow*);

};
