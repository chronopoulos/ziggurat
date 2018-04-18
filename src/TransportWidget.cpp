#include "TransportWidget.h"

#include <QDebug>

int TransportWidget::Stopped = 0;
int TransportWidget::Paused = 1;
int TransportWidget::Playing = 2;

TransportWidget::TransportWidget() {

    // qobjects

    tempoLabel = new ClickLabel("Tempo: %1 BPM", ClickLabel::Type_Double, "Ziggurat", "Tempo");
    connect(tempoLabel, SIGNAL(valueChanged(double)), this, SLOT(setTempo(double)));
    

    stopButton = new QPushButton();
    stopButton->setIcon(QIcon(":/img/stop.png"));
    stopButton->setCheckable(true);
    stopButton->setFocusPolicy(Qt::NoFocus);
    connect(stopButton, SIGNAL(pressed(void)), this, SLOT(stop(void)));

    pauseButton = new QPushButton();
    pauseButton->setIcon(QIcon(":/img/pause.png"));
    pauseButton->setCheckable(true);
    pauseButton->setFocusPolicy(Qt::NoFocus);
    connect(pauseButton, SIGNAL(pressed(void)), this, SLOT(pause(void)));

    playButton = new QPushButton();
    playButton->setIcon(QIcon(":/img/play.png"));
    playButton->setCheckable(true);
    playButton->setFocusPolicy(Qt::NoFocus);
    connect(playButton, SIGNAL(pressed(void)), this, SLOT(play(void)));

    buttonGroup = new QButtonGroup();
    buttonGroup->addButton(stopButton);
    buttonGroup->addButton(pauseButton);
    buttonGroup->addButton(playButton);

    masterClock = new QTimer(this);
    masterClock->setTimerType(Qt::PreciseTimer);

    connect(masterClock, SIGNAL(timeout(void)), this, SIGNAL(ticked(void)));

    // ui

    layout = new QGridLayout();
    layout->addWidget(tempoLabel, 0,0, 1,3);
    layout->addWidget(stopButton, 1,0, 1,1);
    layout->addWidget(pauseButton, 1,1, 1,1);
    layout->addWidget(playButton, 1,2, 1,1);

    setLayout(layout);
    
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    // init

    tempoLabel->setValue(double(120));
    stopButton->setChecked(true);
    state = TransportWidget::Stopped;

}

void TransportWidget::setTempo(double bpm) {

    m_period_ms = 60000. / (4*bpm);
    masterClock->setInterval(m_period_ms);

}

void TransportWidget::stop(void) {

    masterClock->stop();
    emit stopped();
    state = TransportWidget::Stopped;

}

void TransportWidget::pause(void) {

    masterClock->stop();
    state = TransportWidget::Paused;

}

void TransportWidget::play(void) {

    masterClock->start(m_period_ms);
    state = TransportWidget::Playing;

}

