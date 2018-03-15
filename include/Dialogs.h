#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>

class NewSequenceDialog : public QDialog
{
    public:
        NewSequenceDialog();
        QString name(void);
        int length(void);

    private:
        QLabel *nameLabel, *lengthLabel;
        QGridLayout *layout;
        QLineEdit *nameLineEdit;
        QDialogButtonBox *buttons;
        QSpinBox *lengthSpin;

};
