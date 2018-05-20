#include <QDebug>
#include <QJsonArray>
#include <QDrag>
#include <QMimeData>

#include "Thumbnail.h"
#include "Trigger.h"

Thumbnail::Thumbnail(int nsteps) : QFrame() {

    this->nsteps = nsteps;

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    label = new QLabel();

    muteButton = new QPushButton("M");
    muteButton->setMaximumWidth(30);
    muteButton->setFocusPolicy(Qt::NoFocus);
    muteButton->setCheckable(true);
    muteButton->setStyleSheet("QPushButton {background-color:rgba(255,0,0,255)} "
                                "QPushButton:checked {background-color:rgba(127,0,0,255)}");
    connect(muteButton, SIGNAL(clicked(bool)), this, SLOT(handleMuteClick(bool)));

    queueButton = new QPushButton("Q");
    queueButton->setMaximumWidth(30);
    queueButton->setFocusPolicy(Qt::NoFocus);
    queueButton->setCheckable(true);
    queueButton->setStyleSheet("QPushButton {background-color:rgba(0,0,255,255)} "
                                "QPushButton:checked {background-color:rgb(0,0,127,255)}");
    connect(queueButton, SIGNAL(clicked(bool)), this, SLOT(handleQueueClick(bool)));

    topLayout->addWidget(label);
    topLayout->addWidget(muteButton);
    topLayout->addWidget(queueButton);

    Led *led;
    for (int i=0; i<nsteps; i++) {
        led = new Led();
        bottomLayout->addWidget(led);
        leds.push_back(led);
    }

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);

    playheadLed = nullptr;

    palette_light = QPalette();
    palette_light.setColor(QPalette::Background, QColor(200,200,200));
    palette_dark = QPalette();
    palette_dark.setColor(QPalette::Background, QColor(100, 100, 100));

    setAutoFillBackground(true);

    m_enabled = true;
    setPalette(palette_dark);

    isSelected = false;
    setFrameStyle(QFrame::Box | QFrame::Plain);


    setMinimumWidth(300);
    setMinimumHeight(70);
    setMaximumHeight(120);

}

Thumbnail::Thumbnail(const QJsonObject &seqJSO) : Thumbnail(seqJSO["nsteps"].toInt()) {

    setName(seqJSO["name"].toString());
    setMute(seqJSO["mute"].toBool());

    QJsonArray trigJSA = seqJSO["trigs"].toArray();
    Trigger trig;
    for (int i = 0; i < trigJSA.size(); i++) {

        trig = Trigger(trigJSA[i].toObject());
        if (trig.type() == Trigger::Type_Null) {
            setActivation(i, false);
        } else {
            setActivation(i, true);
        }

    }

}

void Thumbnail::setName(QString name) {

    m_name = name;
    label->setText(m_name);

}

void Thumbnail::select(void) {

    isSelected = true;
    setLineWidth(4);

}

void Thumbnail::deselect(void) {

    isSelected = false;
    setLineWidth(1);

}

void Thumbnail::mousePressEvent(QMouseEvent *e) {

    if (e->buttons()==Qt::LeftButton) {
        emit selected();
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

void Thumbnail::handleMuteClick(bool mute) {

    emit muteChanged(mute);

}

void Thumbnail::handleQueueClick(bool queue) {

    emit queueChanged(queue);

}

void Thumbnail::setMute(bool mute) {

    muteButton->setChecked(mute);

}

void Thumbnail::setQueue(bool queue) {

    queueButton->setChecked(queue);

}

void Thumbnail::updatePlayhead(int step) {

    if (playheadLed) playheadLed->setPlayhead(false);
    playheadLed = leds[step];
    playheadLed->setPlayhead(true);


}

void Thumbnail::setActivation(int step, bool activation) {

    leds[step]->setActive(activation);

}

void Thumbnail::mouseMoveEvent(QMouseEvent *e) {

    if (e->buttons() == Qt::LeftButton) {

        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;
        mimeData->setText("thumb");
        drag->setMimeData(mimeData);

        drag->exec(Qt::MoveAction);

    }

}
