#ifndef BITSETDEMO_H
#define BITSETDEMO_H

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include "bitset.h"

class BitSetDemo : public QWidget
{
    Q_OBJECT
public:
    explicit BitSetDemo(QWidget *parent = nullptr);

private slots:
    void createBitSets();
    void toggleBit(int setNum, int index);
    void performAll();
    void performAny();
    void performNone();
    void performCount();
    void performFlipAll();
    void performFlipBit();
    void performSet();
    void performReset();
    void performAND();
    void performOR();
    void performNOT();
    void showString();
    void showULLong();

private:
    void updateDisplay();
    std::unique_ptr<BitSet> bitset1;
    std::unique_ptr<BitSet> bitset2;
    QSpinBox *sizeSpin;
    QWidget *bitsWidget1;
    QWidget *bitsWidget2;
    QTextEdit *output;
    QGridLayout *grid1;
    QGridLayout *grid2;
    QSpinBox *bitIndexSpin;
};

#endif