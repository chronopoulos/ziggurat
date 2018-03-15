#include <QDebug>

#include "Thumbnail.h"

Thumbnail::Thumbnail(int nsteps) : QFrame() {

    this->nsteps = nsteps;

    QGridLayout *layout = new QGridLayout();
    label = new QLabel();
    layout->addWidget(label, 0,0, 1,nsteps);
    Led *led;
    for (int i=0; i<nsteps; i++) {
        led = new Led();
        layout->addWidget(led, 1,i, 1,1);
        leds.push_back(led);
    }
    setLayout(layout);

    playheadLed = nullptr;

    palette_light = QPalette();
    palette_light.setColor(QPalette::Background, QColor(200,200,200));
    palette_dark = QPalette();
    palette_dark.setColor(QPalette::Background, QColor(100, 100, 100));

    setAutoFillBackground(true);

    m_enabled = true;
    setPalette(palette_dark);

    interest = false;
    setFrameStyle(QFrame::Box | QFrame::Plain);


    setMinimumWidth(300);
    setMinimumHeight(70);
    setMaximumHeight(120);

}

void Thumbnail::setName(QString name) {

    m_name = name;
    label->setText(m_name);

}

void Thumbnail::setInterest(bool interest) {

    this->interest = interest;
    if (interest) {
        setLineWidth(4);
    } else {
        setLineWidth(1);
    }

}

bool Thumbnail::hasInterest(void) {

    return interest;

}

void Thumbnail::mousePressEvent(QMouseEvent *e) {

    if (e->buttons()==Qt::LeftButton) {
        emit interestRequested();
    } 

}

void Thumbnail::contextMenuEvent(QContextMenuEvent*) {

    QMenu managerMenu;
    managerMenu.addAction("Delete");
    QAction *action = managerMenu.exec(QCursor::pos());
    if (action && (action->text().contains("Delete"))) {
        emit deleteRequested();
    }

}

void Thumbnail::toggleEnabling(void) {

    m_enabled = !m_enabled;
    if (m_enabled) {
        setPalette(palette_dark);
    } else {
        setPalette(palette_light);
    }
    emit enablingChanged(m_enabled);

}

void Thumbnail::updatePlayhead(int step) {

    if (playheadLed) playheadLed->setPlayhead(false);
    playheadLed = leds[step];
    playheadLed->setPlayhead(true);


}

void Thumbnail::setActivation(int step, bool activation) {

    leds[step]->setActive(activation);

}
