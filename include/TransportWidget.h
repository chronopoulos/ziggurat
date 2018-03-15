#include <QFrame>
#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QIcon>
#include <QDoubleSpinBox>
#include <QTimer>

#include "ClickLabel.h"

class TransportWidget : public QFrame
{
    Q_OBJECT

    public:
        TransportWidget();
        int state;
        static int Stopped, Playing, Paused;
        QPushButton *stopButton, *pauseButton, *playButton;

    private:
        QGridLayout *layout;
        ClickLabel *tempoLabel;
        QButtonGroup *buttonGroup;
        QTimer *masterClock;
        int m_period_ms;

    public slots:
        void setTempo(double);
        void stop(void);
        void pause(void);
        void play(void);

    signals:
        void ticked(void);
        void stopped(void);

};
