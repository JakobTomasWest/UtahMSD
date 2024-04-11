#include "mainwidget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QSpacerItem>

mainWidget::mainWidget(QWidget *parent) : QWidget(parent)
{
    // Initialize widgets
    labelFirstName = new QLabel(tr("First Name:"));
    editFirstName = new QLineEdit;


    labelLastName = new QLabel(tr("Last Name:"));
    editLastName = new QLineEdit;

    labelAge = new QLabel(tr("Age:"));

    ageDial = new QSpinBox;

    labelGender = new QLabel(tr("Gender:"));
    radioMale = new QRadioButton(tr("Male"));
    radioFemale = new QRadioButton(tr("Female"));

    finishB = new QPushButton(tr("Finish"));
    refreshB = new QPushButton(tr("Refresh"));
    summaryText = new QTextEdit;

    // GroupBox for gender radios
    groupBoxGender = new QGroupBox(tr(""));
    auto *genderLayout = new QVBoxLayout;
    genderLayout->addWidget(radioMale);
    genderLayout->addWidget(radioFemale);
    groupBoxGender->setLayout(genderLayout);



    // Layout setup
    setGridLayout();
    setVerticalLayout();

    // Connect signal to slot
    connect(refreshB, &QPushButton::clicked, this, &mainWidget::fillText);
    connect(finishB, &QPushButton::clicked, [this]() {
        editFirstName->clear();
        editLastName->clear();
        ageDial->clear();
        radioMale->setChecked(false);
        radioFemale->setChecked(false);
        summaryText->clear();
    });
}

void mainWidget::setGridLayout()
{
    gridLayout = new QGridLayout;
    gridLayout->addWidget(labelFirstName, 0, 0);
    gridLayout->addWidget(editFirstName, 0, 1);
    gridLayout->addWidget(labelLastName, 1, 0);
    gridLayout->addWidget(editLastName, 1, 1);
    gridLayout->addWidget(labelAge, 2, 0);
    gridLayout->addWidget(ageDial,2,1);
    gridLayout->addItem(new QSpacerItem(10, 50, QSizePolicy::Expanding, QSizePolicy::Minimum), 0, 2, 1, 1);
    gridLayout->addWidget(labelGender, 3, 0);
    gridLayout->addWidget(groupBoxGender, 3, 1);
}

void mainWidget::setVerticalLayout()
{
    mainLayout = new QVBoxLayout;
    mainLayout->addItem(gridLayout);
    mainLayout->addWidget(refreshB);

    mainLayout->addWidget(summaryText);
    mainLayout->addWidget(finishB);
    setLayout(mainLayout);
}

void mainWidget::fillText()
{
    QString summary;
    summary += "First Name: " + editFirstName->text() + "\n";
    summary += "Last Name: " + editLastName->text() + "\n";
    QString gender = radioMale->isChecked() ? "Male" : (radioFemale->isChecked() ? "Female" : "Not specified");
    summary += "Gender: " + gender;
    summaryText->setText(summary);
}
