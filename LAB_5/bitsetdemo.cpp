#include "bitsetdemo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

BitSetDemo::BitSetDemo(QWidget *parent) : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);

    auto *controlLayout = new QHBoxLayout;
    sizeSpin = new QSpinBox;
    sizeSpin->setRange(1, 256);
    sizeSpin->setValue(16);
    controlLayout->addWidget(new QLabel("Размер:"));
    controlLayout->addWidget(sizeSpin);
    QPushButton *createBtn = new QPushButton("Создать два BitSet");
    connect(createBtn, &QPushButton::clicked, this, &BitSetDemo::createBitSets);
    controlLayout->addWidget(createBtn);
    mainLayout->addLayout(controlLayout);

    auto *bitsLayout = new QHBoxLayout;
    bitsWidget1 = new QWidget;
    grid1 = new QGridLayout(bitsWidget1);
    bitsWidget2 = new QWidget;
    grid2 = new QGridLayout(bitsWidget2);
    bitsLayout->addWidget(new QLabel("Первый набор:"));
    bitsLayout->addWidget(bitsWidget1);
    bitsLayout->addWidget(new QLabel("Второй набор:"));
    bitsLayout->addWidget(bitsWidget2);
    mainLayout->addLayout(bitsLayout);

    bitIndexSpin = new QSpinBox;
    bitIndexSpin->setPrefix("Индекс бита: ");
    bitIndexSpin->setRange(0, 255);
    mainLayout->addWidget(bitIndexSpin);

    auto *opLayout = new QHBoxLayout;
    QPushButton *btnFlipAll = new QPushButton("Flip All (1)");
    QPushButton *btnFlipBit = new QPushButton("Flip Bit (1)");
    QPushButton *btnSet = new QPushButton("Set Bit (1)");
    QPushButton *btnReset = new QPushButton("Reset Bit (1)");
    QPushButton *btnAND = new QPushButton("1 & 2");
    QPushButton *btnOR = new QPushButton("1 | 2");
    QPushButton *btnNOT = new QPushButton("~1");
    QPushButton *btnAll = new QPushButton("all 1");
    QPushButton *btnAny = new QPushButton("any 1");
    QPushButton *btnNone = new QPushButton("none 1");
    QPushButton *btnCount = new QPushButton("count 1");
    QPushButton *btnString = new QPushButton("to_string 1");
    QPushButton *btnULLong = new QPushButton("to_ullong 1");

    connect(btnFlipAll, &QPushButton::clicked, this, &BitSetDemo::performFlipAll);
    connect(btnFlipBit, &QPushButton::clicked, this, &BitSetDemo::performFlipBit);
    connect(btnSet, &QPushButton::clicked, this, &BitSetDemo::performSet);
    connect(btnReset, &QPushButton::clicked, this, &BitSetDemo::performReset);
    connect(btnAND, &QPushButton::clicked, this, &BitSetDemo::performAND);
    connect(btnOR, &QPushButton::clicked, this, &BitSetDemo::performOR);
    connect(btnNOT, &QPushButton::clicked, this, &BitSetDemo::performNOT);
    connect(btnAll, &QPushButton::clicked, this, &BitSetDemo::performAll);
    connect(btnAny, &QPushButton::clicked, this, &BitSetDemo::performAny);
    connect(btnNone, &QPushButton::clicked, this, &BitSetDemo::performNone);
    connect(btnCount, &QPushButton::clicked, this, &BitSetDemo::performCount);
    connect(btnString, &QPushButton::clicked, this, &BitSetDemo::showString);
    connect(btnULLong, &QPushButton::clicked, this, &BitSetDemo::showULLong);

    opLayout->addWidget(btnFlipAll);
    opLayout->addWidget(btnFlipBit);
    opLayout->addWidget(btnSet);
    opLayout->addWidget(btnReset);
    opLayout->addWidget(btnAND);
    opLayout->addWidget(btnOR);
    opLayout->addWidget(btnNOT);
    opLayout->addWidget(btnAll);
    opLayout->addWidget(btnAny);
    opLayout->addWidget(btnNone);
    opLayout->addWidget(btnCount);
    opLayout->addWidget(btnString);
    opLayout->addWidget(btnULLong);
    mainLayout->addLayout(opLayout);

    output = new QTextEdit;
    output->setReadOnly(true);
    mainLayout->addWidget(output);

    setWindowTitle("BitSet Demo");
    setStyleSheet("background-color: #1e1e1e; color: #ddd;");
}

void BitSetDemo::createBitSets()
{
    int size = sizeSpin->value();
    bitset1 = std::make_unique<BitSet>(size);
    bitset2 = std::make_unique<BitSet>(size);
    // Инициализируем все биты в 0, UI обновится
    updateDisplay();
}

void BitSetDemo::toggleBit(int setNum, int index)
{
    BitSet* bs = (setNum == 1) ? bitset1.get() : bitset2.get();
    if (!bs) return;
    bs->flip(index);
    updateDisplay();
}

void BitSetDemo::updateDisplay()
{
    if (!bitset1 || !bitset2) return;

    // Очистка старых кнопок
    QLayoutItem *item;
    while ((item = grid1->takeAt(0))) {
        delete item->widget();
        delete item;
    }
    while ((item = grid2->takeAt(0))) {
        delete item->widget();
        delete item;
    }

    size_t size = bitset1->size();
    int cols = 8;
    for (size_t i = 0; i < size; ++i) {
        auto createBtn = [&](int setNum, size_t bitIndex) -> QPushButton* {
            QPushButton *btn = new QPushButton;
            btn->setFixedSize(30,30);
            btn->setCheckable(true);
            btn->setChecked(bitset1->test(bitIndex) ? true : false);
            btn->setStyleSheet(
                bitset1->test(bitIndex) ?
                "background-color: #00aa00; border-radius: 3px;" :
                "background-color: #555; border-radius: 3px;"
            );
            connect(btn, &QPushButton::clicked, [=]() {
                toggleBit(setNum, bitIndex);
            });
            return btn;
        };
        grid1->addWidget(createBtn(1, i), i/cols, i%cols);
        grid2->addWidget(createBtn(2, i), i/cols, i%cols);
    }
}

// Реализации операций - вызов методов BitSet и вывод результата
void BitSetDemo::performAll() { if(bitset1) output->setText(QString("all: %1").arg(bitset1->all())); }
void BitSetDemo::performAny() { if(bitset1) output->setText(QString("any: %1").arg(bitset1->any())); }
void BitSetDemo::performNone() { if(bitset1) output->setText(QString("none: %1").arg(bitset1->none())); }
void BitSetDemo::performCount() { if(bitset1) output->setText(QString("count: %1").arg(bitset1->count())); }
void BitSetDemo::performFlipAll() { if(bitset1) { bitset1->flip(); updateDisplay(); } }
void BitSetDemo::performFlipBit() { if(bitset1) { bitset1->flip(bitIndexSpin->value()); updateDisplay(); } }
void BitSetDemo::performSet() { if(bitset1) { bitset1->set(bitIndexSpin->value()); updateDisplay(); } }
void BitSetDemo::performReset() { if(bitset1) { bitset1->reset(bitIndexSpin->value()); updateDisplay(); } }
void BitSetDemo::performAND() {
    if(!bitset1 || !bitset2) return;
    auto result = *bitset1 & *bitset2;
    bitset1 = std::make_unique<BitSet>(result);
    output->setText("Выполнено &, результат присвоен первому набору");
    updateDisplay();
}
void BitSetDemo::performOR() {
    if(!bitset1 || !bitset2) return;
    auto result = *bitset1 | *bitset2;
    bitset1 = std::make_unique<BitSet>(result);
    output->setText("Выполнено |, результат присвоен первому набору");
    updateDisplay();
}
void BitSetDemo::performNOT() {
    if(!bitset1) return;
    ~(*bitset1);
    updateDisplay();
}
void BitSetDemo::showString() { if(bitset1) output->setText(QString("to_string: %1").arg(bitset1->to_string().c_str())); }
void BitSetDemo::showULLong() {
    if(!bitset1) return;
    try {
        output->setText(QString("to_ullong: %1").arg(bitset1->to_ullong()));
    } catch(const std::exception& e) {
        output->setText(QString("Ошибка: %1").arg(e.what()));
    }
}