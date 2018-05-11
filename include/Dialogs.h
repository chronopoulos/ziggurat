#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QPushButton>

class NewSequenceDialog : public QDialog
{
    Q_OBJECT

    public:
        NewSequenceDialog(void);
        QString name(void);
        int length(void);

    private:
        QLabel *nameLabel, *lengthLabel;
        QGridLayout *layout;
        QLineEdit *nameLineEdit;
        QDialogButtonBox *buttons;
        QSpinBox *lengthSpin;

};


class MaybeSaveDialog : public QDialog
{
    Q_OBJECT

    public:
        MaybeSaveDialog(void);

    private:
        QLabel *description;
        QVBoxLayout *layout;
        QDialogButtonBox *buttons;
        QPushButton *discardButton;

    public slots:
        void discard(void);
};
