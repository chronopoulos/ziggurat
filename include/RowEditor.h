#include <QFrame>
#include <QVBoxLayout>

#include "ButtonRow.h"

class RowEditor : public QFrame
{
    Q_OBJECT

    public:
        RowEditor(void);
        void setScoped(bool);

    private:
        QVBoxLayout *mainLayout;
        ButtonRow *currentRow, *defaultRow;
        bool m_scoped;

    public slots:
        void setRow(ButtonRow*);

    protected:
        void paintEvent(QPaintEvent*);

};
