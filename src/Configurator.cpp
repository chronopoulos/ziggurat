#include <QDebug>

#include "Configurator.h"

Configurator::Configurator(void) {

    layout = new QVBoxLayout();
    setLayout(layout);

    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(1);

    currentPage = nullptr;

    defaultPage = new ConfigPage(0);

}

void Configurator::setPage(ConfigPage *newPage) {

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
