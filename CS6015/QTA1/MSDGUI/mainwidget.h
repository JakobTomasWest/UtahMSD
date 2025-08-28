#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "parse.hpp"
#include "expr.hpp"
#include "Val.h"
#include "env.h"
#include "pointer.hpp"


#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
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
    QLabel *labelExpression;
    QTextEdit *submitText;
    QLabel *labelResult;


    QGroupBox *radioGroup;
    QVBoxLayout *radioLayout;
    QRadioButton *radioInterp;
    QRadioButton *radioPrettyPrint;
    QPushButton *resetB;
    QPushButton *submitB;

    QTextEdit *resultText;
    QVBoxLayout *mainLayout;
    QGridLayout *gridLayout;

    // Layout setup methods
    void setGridLayout();
    void setVerticalLayout();
    // Slot
    void fillText();
};
#endif // MAINWIDGET_H
