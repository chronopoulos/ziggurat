#include "Dialogs.h"

NewSequenceDialog::NewSequenceDialog(void) {

    layout = new QGridLayout();

    nameLabel = new QLabel("Name");
    lengthLabel = new QLabel("Length");

    nameLineEdit = new QLineEdit();
    nameLineEdit->setText("My Sequence");
    nameLineEdit->selectAll();

    lengthSpin = new QSpinBox();
    lengthSpin->setMinimum(1);
    lengthSpin->setValue(16);

    buttons = new QDialogButtonBox();
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->addButton(QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted(void)), this, SLOT(accept(void)));
    connect(buttons, SIGNAL(rejected(void)), this, SLOT(reject(void)));

    layout->addWidget(nameLabel, 0,0, 1,1);
    layout->addWidget(nameLineEdit, 0,1, 1,1);
    layout->addWidget(lengthLabel, 1,0, 1,1);
    layout->addWidget(lengthSpin, 1,1, 1,1);
    layout->addWidget(buttons, 2,0, 1,2);
    setLayout(layout);

    setModal(true);
    setWindowTitle("Add Sequence");
    setWindowFlags(Qt::WindowStaysOnTopHint);

}

QString NewSequenceDialog::name(void) {

    return nameLineEdit->text();

}

int NewSequenceDialog::length(void) {

    return lengthSpin->value();

}
