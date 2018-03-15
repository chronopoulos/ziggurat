#include <QFrame>
#include <QVBoxLayout>

#include "ConfigPage.h"

class Configurator : public QFrame
{
    Q_OBJECT

    public:
        Configurator(void);
        void setDefaultPage(void);

    private:
        QVBoxLayout *layout;
        ConfigPage *currentPage;
        ConfigPage *defaultPage;

    public slots:
        void setPage(ConfigPage*);

};
