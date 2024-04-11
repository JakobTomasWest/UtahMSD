#include "mainwidget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QSpacerItem>




mainWidget::mainWidget(QWidget *parent) : QWidget(parent)
{
    // Initialize widgets
    labelExpression = new QLabel(tr("Expression: "));
    submitText = new QTextEdit;
    labelResult = new QLabel(tr("Result: "));


    radioInterp = new QRadioButton(tr("Interp"));
    radioPrettyPrint = new QRadioButton(tr("PrettyPrint"));

    submitB = new QPushButton(tr("Submit"));
    resetB = new QPushButton(tr("Reset"));
    resultText = new QTextEdit;
    // auto *optionLayout = new QVBoxLayout;
    // optionLayout->addWidget(radioInterp);
    // optionLayout->addWidget(radioPrettyPrint);
    radioGroup = new QGroupBox();
    radioLayout = new QVBoxLayout;


    // Layout setup
    setGridLayout();
    setVerticalLayout();

    // Connect signal to slot
    connect(submitB, &QPushButton::clicked, this, &mainWidget::fillText);
    connect(resetB, &QPushButton::clicked, [this]() {
        submitText->clear();
        resultText->clear();
        radioInterp->setChecked(false);
        radioPrettyPrint->setChecked(false);
    });
}

void mainWidget::setGridLayout()
{
    gridLayout = new QGridLayout;

    radioLayout->addWidget(radioInterp);
    radioLayout->addWidget(radioPrettyPrint);
    radioGroup->setLayout(radioLayout);

    gridLayout->addWidget(labelExpression, 0, 0);
    gridLayout->addWidget(submitText, 0, 1,1,8);

    gridLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum), 2,1,1,8);
    gridLayout->addWidget(radioGroup, 3, 1, 1, 8);
    gridLayout->addWidget(submitB, 5, 1,1,2);
    gridLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum), 6,1,1,8);

    gridLayout->addWidget(labelResult, 7, 0);
    gridLayout->addWidget(resultText, 7, 1, 1, 8);

    gridLayout->addWidget(resetB, 9, 1,1,2);
}

void mainWidget::setVerticalLayout()
{
    mainLayout = new QVBoxLayout;
    mainLayout->addItem(gridLayout);
    // mainLayout->addWidget(refreshB);

    // mainLayout->addWidget(resultText);
    // mainLayout->addWidget(resultB);
    setLayout(mainLayout);
}

void mainWidget::fillText()
{
//     QString summary;
//     summary +=  submitText->toPlainText() + "\n";
//     resultText->setText(summary);

    //Convert input from QTextEdit to std::string
    std::string inputString = submitText->toPlainText().toUtf8().constData();
try {
    // Parse the input string into an expression
    PTR(Expr) parsedExpr = parseString(inputString);

    QString result;
    //Check if interpret or pretty print
    if (radioInterp->isChecked()) {
        //Interp the expression and convert the result to a string
        PTR(Val) val = parsedExpr->interp(NEW(EmptyEnv)());
        result = QString::fromStdString(val->to_string());
    } else if (radioPrettyPrint->isChecked()) {
        //PrettyPrint
        std::stringstream ss;
        parsedExpr->print(ss);
        result = QString::fromStdString(parsedExpr->to_pretty_string());
    } else {
        std::stringstream ss;
        parsedExpr->print(ss);
        result = QString::fromStdString(ss.str());
    }

    // Display the result in the QTextEdit
    resultText->setText(result);
} catch (const std::exception& e) {
    // If an error occurs during parsing or evaluation, display the error message
    resultText->setText(QString("Error: ") + QString::fromStdString(e.what()));
}
}
