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

