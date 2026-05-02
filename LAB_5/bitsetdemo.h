#ifndef BITSETDEMO_H
#define BITSETDEMO_H

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QFrame>
#include <QHBoxLayout>
#include "bitset.h"

// ─── Виджет отображения одного битового набора ─────────────
class BitGridWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BitGridWidget(QWidget *parent = nullptr);
    void setBitSet(BitSet *bs);
    void clear();

signals:
    void bitToggled(int index);

private slots:
    void onBitClicked();

private:
    void rebuild();
    BitSet *m_bs = nullptr;
    QGridLayout *m_grid;
};

// ─── Виджет информации (bin/dec/hex) ──────────────────────
class BitInfoWidget : public QFrame
{
    Q_OBJECT
public:
    explicit BitInfoWidget(const QString &title, QWidget *parent = nullptr);
    void update(BitSet *bs);

private:
    QLabel *m_binLabel;
    QLabel *m_decLabel;
    QLabel *m_hexLabel;
    QLabel *m_cntLabel;
};

// ─── Главный демо-виджет ──────────────────────────────────
class BitSetDemo : public QWidget
{
    Q_OBJECT
public:
    explicit BitSetDemo(QWidget *parent = nullptr);

private slots:
    void createBitSets();
    void onBit1Toggled(int idx);
    void onBit2Toggled(int idx);
    void performAll();
    void performAny();
    void performNone();
    void performCount();
    void performFlipAll();
    void performFlipBit();
    void performSet();
    void performReset();
    void performSetAll();
    void performResetAll();
    void performAND();
    void performOR();
    void performXOR();
    void performNOT();
    void performSHL();
    void performSHR();
    void showString();
    void showULLong();

private:
    void log(const QString &msg, const QString &color = "#e8eaf6");
    void logOp(const QString &op, const QString &result);
    void refreshInfoWidgets();

    std::unique_ptr<BitSet> bitset1;
    std::unique_ptr<BitSet> bitset2;

    QSpinBox       *sizeSpin;
    QSpinBox       *bitIndexSpin;
    QSpinBox       *shiftSpin;
    BitGridWidget  *grid1;
    BitGridWidget  *grid2;
    BitInfoWidget  *info1;
    BitInfoWidget  *info2;
    QTextEdit      *logOutput;
};

#endif // BITSETDEMO_H