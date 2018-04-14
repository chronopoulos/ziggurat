#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QContextMenuEvent>

class GroupWidget : public QWidget
{
    Q_OBJECT

    public:
        GroupWidget(void);
        void addSequence(void);

    private:
        QComboBox *typeSelector;
        QVBoxLayout *seqLayout;

    protected:
        void contextMenuEvent(QContextMenuEvent*);

};
