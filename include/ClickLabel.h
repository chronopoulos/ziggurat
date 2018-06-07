#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QLabel>
#include <QString>
#include <QStringList>
#include <QInputDialog>
#include <QMouseEvent>

class ClickLabel: public QLabel
{
    Q_OBJECT

    public:
        ClickLabel(QString, int, QString, QString);

        void setMin(int);
        void setMax(int);
        void setMin(double);
        void setMax(double);

        void setValue(int);
        void setValue(QString);
        void setValue(double);
        void setHardwired(bool);
        void setItems(const QStringList&);
        void setPhocus(bool);

        int m_type;
        QString m_title;
        QString m_label;

        static const int Type_Int;
        static const int Type_QString;
        static const int Type_Double;
        static const int Type_Item;

    private:
        QString m_baseText;
        QStringList m_items;
        int m_min_int;
        int m_max_int;
        double m_min_double;
        double m_max_double;

        int m_val_int;
        QString m_val_qstring;
        double m_val_double;

        bool m_hardwired;

        bool m_phocus;

    protected:
        void mousePressEvent(QMouseEvent*);
        void paintEvent(QPaintEvent*);

    signals:
        void valueChanged(int);
        void valueChanged(QString);
        void valueChanged(double);

    public slots:
        void setTitle(QString);
        void setLabel(QString);


};

#endif
