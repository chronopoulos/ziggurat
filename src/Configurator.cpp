#include <QDebug>

#include "Configurator.h"

Configurator::Configurator(void) {

    layout = new QVBoxLayout();
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    currentPage = nullptr;

    defaultPage = new ConfigPage(0);

    setDefaultPage();

}

void Configurator::setPage(ConfigPage *newPage) {

    if (!newPage) newPage = defaultPage;

    if (currentPage != newPage) {
        if (currentPage) {
            layout->replaceWidget(currentPage, newPage);
            currentPage->setParent(NULL);
        } else {
            layout->addWidget(newPage);
        }
        currentPage = newPage;
    }


}

void Configurator::setDefaultPage(void) {

    setPage(defaultPage);

}
