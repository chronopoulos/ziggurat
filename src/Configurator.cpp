#include <QDebug>

#include "Configurator.h"

Configurator::Configurator(void) {

    layout = new QVBoxLayout();
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    defaultPage = new ConfigPage(0);

    currentPage = defaultPage;
    layout->addWidget(currentPage);

    setFrameStyle(QFrame::Box | QFrame::Plain);

    setScoped(false);

}

void Configurator::paintEvent(QPaintEvent *e) {

    if (m_scoped) {
        setLineWidth(2);
    } else {
        setLineWidth(1);
    }

    QFrame::paintEvent(e);

}

void Configurator::setScoped(bool scoped) {

    m_scoped = scoped;
    update();

}

void Configurator::setPage(ConfigPage *newPage) {

    // case of newPage == nullptr
    if (!newPage) newPage = defaultPage;

    if (newPage != currentPage) {

        layout->replaceWidget(currentPage, newPage);

        currentPage->hide();
        currentPage = newPage;
        currentPage->show();

    }

}

