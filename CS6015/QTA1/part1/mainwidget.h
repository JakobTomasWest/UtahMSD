#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QRadioButton>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QSpacerItem>

class mainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit mainWidget(QWidget *parent = nullptr);
    QLabel *labelFirstName;
    QLabel *labelLastName;
    QLabel *labelAge;
    QLabel *labelGender;
    QLineEdit *editFirstName;
    QLineEdit *editLastName;
    QSpinBox *ageDial;
    QRadioButton *radioMale;
    QRadioButton *radioFemale;
    QPushButton *finishB;
    QPushButton *refreshB;
    QTextEdit *summaryText;
    QVBoxLayout *mainLayout;
    QGridLayout *gridLayout;
    QGroupBox *groupBoxGender;\

    // Layout setup methods
    void setGridLayout();
    void setVerticalLayout();
    // Slot
    void fillText();
};
#endif // MAINWIDGET_H
