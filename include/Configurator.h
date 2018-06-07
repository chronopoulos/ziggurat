#include <QFrame>
#include <QVBoxLayout>

#include "ConfigPage.h"

class Configurator : public QFrame
{
    Q_OBJECT

    public:
        Configurator(void);
        void setScoped(bool);
        void phocusEvent(QKeyEvent*);

    private:
        QVBoxLayout *layout;
        ConfigPage *currentPage;
        ConfigPage *defaultPage;
        bool m_scoped;

    public slots:
        void setPage(ConfigPage*);

    protected:
        void paintEvent(QPaintEvent*);

};
