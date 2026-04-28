#ifndef STRINGDEMO_H
#define STRINGDEMO_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include "stringlib.h"

class StringDemo : public QWidget
{
    Q_OBJECT
public:
    explicit StringDemo(QWidget *parent = nullptr);

private slots:
    void performOperation();
    void iterateString();

private:
    QLineEdit *input1;
    QLineEdit *input2;
    QLineEdit *countEdit;
    QComboBox *opCombo;
    QTextEdit *output;
    QPushButton *iterateBtn;
};

#endif