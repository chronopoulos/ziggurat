#include <QObject>

class Delta : public QObject
{
    Q_OBJECT

    public:
        Delta(void);
        void setState(bool);
        bool state(void);

    private:
        bool m_state;

    signals:
        void stateChanged(bool);

};
