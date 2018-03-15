#include "ClickLabel.h"
////

const int ClickLabel::Type_Int = 0;
const int ClickLabel::Type_QString = 1;
const int ClickLabel::Type_Double = 2;
const int ClickLabel::Type_Item = 3;

ClickLabel::ClickLabel(QString baseText, int type) {

    m_baseText = baseText;
    m_type = type;
    setText(m_baseText);

    // TODO: prevent stylesheet from propagating to children
    setStyleSheet("QLabel { background-color : black; color : white; }");
    setAlignment(Qt::AlignCenter);

    m_hardwired = false;

    m_min_int = -2147483647;
    m_max_int =  2147483647;
    m_min_double = -2147483647;
    m_max_double =  2147483647;

}

void ClickLabel::setMin(int min) {

    m_min_int = min;

}

void ClickLabel::setMax(int max) {

    m_max_int = max;

}

void ClickLabel::setMin(double min) {

    m_min_double = min;

}

void ClickLabel::setMax(double max) {

    m_max_double = max;

}

void ClickLabel::setItems(const QStringList &items) {

    m_items = items;

}

void ClickLabel::setValue(int val) {

    m_val_int = val;
    setText(m_baseText.arg(m_val_int));
    emit valueChanged(m_val_int);

}

void ClickLabel::setValue(QString val) {

    m_val_qstring = val;
    setText(m_baseText.arg(m_val_qstring));
    emit valueChanged(m_val_qstring);

}

void ClickLabel::setValue(double val) {

    m_val_double = val;
    setText(m_baseText.arg(m_val_double));
    emit valueChanged(m_val_double);

}

void ClickLabel::mousePressEvent(QMouseEvent*) {

    if (!m_hardwired) {

        bool ok = false;

        if (m_type == ClickLabel::Type_Int) {
            int result = QInputDialog::getInt(this, "Title", "Label",
                                                m_val_int, m_min_int, m_max_int, 1, &ok);
            if (ok) setValue(result);
        } else if (m_type == ClickLabel::Type_QString) {
            QString result = QInputDialog::getText(this, "Title", "Label", QLineEdit::Normal,
                                                    m_val_qstring, &ok);
            if (ok) setValue(result);
        } else if (m_type == ClickLabel::Type_Double) {
            double result = QInputDialog::getDouble(this, "Title", "Label",
                                                        m_val_double, m_min_double, m_max_double, 2, &ok);
            if (ok) setValue(result);
        } else if (m_type == ClickLabel::Type_Item) {
            QString result = QInputDialog::getItem(this, "Title", "Label", m_items, 0, false, &ok);
            if (ok) setValue(result);
        }
    }

}

void ClickLabel::setHardwired(bool hardwired) {

    m_hardwired = hardwired;

}


