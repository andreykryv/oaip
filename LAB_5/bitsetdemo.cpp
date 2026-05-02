#include "bitsetdemo.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QGroupBox>
#include <QDateTime>
#include <QScrollBar>          // добавлено

// ═══════════════════════════════════════════════════════════
//  BitGridWidget — кликабельная сетка битов
// ═══════════════════════════════════════════════════════════
BitGridWidget::BitGridWidget(QWidget *parent) : QWidget(parent)
{
    m_grid = new QGridLayout(this);
    m_grid->setSpacing(3);
    m_grid->setContentsMargins(4, 4, 4, 4);
}

void BitGridWidget::setBitSet(BitSet *bs)
{
    m_bs = bs;
    rebuild();
}

void BitGridWidget::clear()
{
    m_bs = nullptr;
    QLayoutItem *item;
    while ((item = m_grid->takeAt(0))) {
        delete item->widget();
        delete item;
    }
}

void BitGridWidget::rebuild()
{
    // Удаляем старые кнопки
    QLayoutItem *item;
    while ((item = m_grid->takeAt(0))) {
        delete item->widget();
        delete item;
    }
    if (!m_bs) return;

    size_t n = m_bs->size();
    int cols = (n <= 16) ? 8 : (n <= 32) ? 16 : (n <= 64) ? 16 : 32;

    for (size_t i = 0; i < n; ++i) {
        bool v = m_bs->test(i);
        auto *btn = new QPushButton(v ? "1" : "0");
        btn->setFixedSize(32, 32);
        btn->setCheckable(true);
        btn->setChecked(v);
        btn->setProperty("bitIndex", (int)i);

        // Цвет: установлен = neon green, сброшен = тёмный
        btn->setStyleSheet(v
            ? QString("QPushButton{background:%1;color:#000;border:1px solid %1;"
                      "border-radius:4px;font-family:'Cascadia Code';font-weight:700;font-size:12px;}"
                      "QPushButton:hover{background:#00ff99;}")
              .arg(Theme::NEON_GREEN)
            : QString("QPushButton{background:%1;color:%2;border:1px solid %3;"
                      "border-radius:4px;font-family:'Cascadia Code';font-weight:700;font-size:12px;}"
                      "QPushButton:hover{background:%4;border-color:%5;}")
              .arg(Theme::BG_SURFACE, Theme::TEXT_DIM, Theme::BORDER, Theme::BG_CARD, Theme::BORDER_LIT));

        connect(btn, &QPushButton::clicked, this, &BitGridWidget::onBitClicked);

        int row = i / cols;
        int col = i % cols;
        m_grid->addWidget(btn, row, col);
    }
}

void BitGridWidget::onBitClicked()
{
    auto *btn = qobject_cast<QPushButton*>(sender());
    if (!btn || !m_bs) return;
    int idx = btn->property("bitIndex").toInt();
    emit bitToggled(idx);
    rebuild();
}

// ═══════════════════════════════════════════════════════════
//  BitInfoWidget — блок bin / dec / hex / count
// ═══════════════════════════════════════════════════════════
BitInfoWidget::BitInfoWidget(const QString &title, QWidget *parent) : QFrame(parent)
{
    setStyleSheet(QString("QFrame{background:%1;border:1px solid %2;border-radius:8px;}")
        .arg(Theme::BG_DEEP, Theme::BORDER));

    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(10, 8, 10, 8);
    lay->setSpacing(3);

    QLabel *ttl = new QLabel(title);
    ttl->setStyleSheet(QString("color:%1;font-size:11px;font-weight:700;").arg(Theme::NEON_CYAN));
    lay->addWidget(ttl);

    auto makeRow = [&](const QString &prefix, QLabel *&lbl, const QString &color) {
        auto *row = new QHBoxLayout;
        auto *pfx = new QLabel(prefix);
        pfx->setStyleSheet(QString("color:%1;font-size:11px;font-family:'Cascadia Code';")
                               .arg(Theme::TEXT_SEC));
        pfx->setFixedWidth(40);
        lbl = new QLabel("—");
        lbl->setStyleSheet(QString("color:%1;font-size:12px;font-weight:600;"
                                    "font-family:'Cascadia Code','Consolas',monospace;")
                               .arg(color));
        lbl->setWordWrap(true);
        row->addWidget(pfx);
        row->addWidget(lbl, 1);
        lay->addLayout(row);
    };

    makeRow("BIN:",  m_binLabel, Theme::NEON_GREEN);
    makeRow("DEC:",  m_decLabel, Theme::NEON_YELLOW);
    makeRow("HEX:",  m_hexLabel, Theme::NEON_CYAN);
    makeRow("CNT:",  m_cntLabel, Theme::NEON_PURPLE);
}

void BitInfoWidget::update(BitSet *bs)
{
    if (!bs) {
        m_binLabel->setText("—");
        m_decLabel->setText("—");
        m_hexLabel->setText("—");
        m_cntLabel->setText("—");
        return;
    }

    // BIN (ограничиваем длину)
    std::string bin = bs->to_string();
    QString binQ = QString::fromStdString(bin);
    if (binQ.length() > 32) binQ = "..." + binQ.right(32);
    m_binLabel->setText(binQ);

    // DEC
    try {
        m_decLabel->setText(QString::number(bs->to_ullong()));
    } catch (...) {
        m_decLabel->setText("(слишком большой)");
    }

    // HEX
    m_hexLabel->setText(QString::fromStdString(bs->to_hex()));

    // COUNT
    m_cntLabel->setText(QString("%1 / %2 бит установлено")
        .arg(bs->count()).arg(bs->size()));
}

// ═══════════════════════════════════════════════════════════
//  BitSetDemo
// ═══════════════════════════════════════════════════════════
BitSetDemo::BitSetDemo(QWidget *parent) : QWidget(parent)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(10, 8, 10, 8);
    root->setSpacing(8);

    // ── Панель создания ───────────────────────────────────
    auto *createCard = new QGroupBox("Создание битовых наборов");
    auto *cLay = new QHBoxLayout(createCard);

    cLay->addWidget(new QLabel("Размер:"));
    sizeSpin = new QSpinBox;
    sizeSpin->setRange(2, 64);
    sizeSpin->setValue(16);
    sizeSpin->setFixedWidth(80);
    cLay->addWidget(sizeSpin);

    auto *createBtn = new QPushButton("⚡  Создать два BitSet");
    createBtn->setStyleSheet(
        QString("QPushButton{background:%1;color:#000;border:none;"
                "border-radius:7px;padding:7px 18px;font-weight:700;font-size:13px;}"
                "QPushButton:hover{background:#00ff99;}")
        .arg(Theme::NEON_GREEN));
    connect(createBtn, &QPushButton::clicked, this, &BitSetDemo::createBitSets);
    cLay->addWidget(createBtn);
    cLay->addStretch();

    cLay->addWidget(new QLabel("Индекс бита:"));
    bitIndexSpin = new QSpinBox;
    bitIndexSpin->setRange(0, 63);
    bitIndexSpin->setValue(0);
    bitIndexSpin->setFixedWidth(70);
    cLay->addWidget(bitIndexSpin);

    cLay->addWidget(new QLabel("Сдвиг:"));
    shiftSpin = new QSpinBox;
    shiftSpin->setRange(1, 32);
    shiftSpin->setValue(1);
    shiftSpin->setFixedWidth(60);
    cLay->addWidget(shiftSpin);

    root->addWidget(createCard);

    // ── Биты + информация ─────────────────────────────────
    auto *bitsLayout = new QHBoxLayout;
    bitsLayout->setSpacing(10);

    // BitSet 1
    auto *col1 = new QVBoxLayout;
    QLabel *lbl1 = new QLabel("BitSet A");
    lbl1->setStyleSheet(QString("color:%1;font-weight:700;font-size:13px;").arg(Theme::NEON_GREEN));
    col1->addWidget(lbl1);
    auto *scrollA = new QScrollArea;
    scrollA->setWidgetResizable(true);
    scrollA->setMaximumHeight(150);
    scrollA->setStyleSheet(QString("background:%1;border:1px solid %2;border-radius:8px;")
        .arg(Theme::BG_CARD, Theme::BORDER));
    grid1 = new BitGridWidget;
    scrollA->setWidget(grid1);
    col1->addWidget(scrollA);
    info1 = new BitInfoWidget("A — информация");
    col1->addWidget(info1);
    bitsLayout->addLayout(col1, 1);

    // BitSet 2
    auto *col2 = new QVBoxLayout;
    QLabel *lbl2 = new QLabel("BitSet B");
    lbl2->setStyleSheet(QString("color:%1;font-weight:700;font-size:13px;").arg(Theme::NEON_CYAN));
    col2->addWidget(lbl2);
    auto *scrollB = new QScrollArea;
    scrollB->setWidgetResizable(true);
    scrollB->setMaximumHeight(150);
    scrollB->setStyleSheet(QString("background:%1;border:1px solid %2;border-radius:8px;")
        .arg(Theme::BG_CARD, Theme::BORDER));
    grid2 = new BitGridWidget;
    scrollB->setWidget(grid2);
    col2->addWidget(scrollB);
    info2 = new BitInfoWidget("B — информация");
    col2->addWidget(info2);
    bitsLayout->addLayout(col2, 1);

    root->addLayout(bitsLayout);

    // ── Операции ──────────────────────────────────────────
    auto *opCard = new QGroupBox("Операции");
    auto *opLay  = new QVBoxLayout(opCard);

    // Ряд 1: побитовые над A
    auto *row1 = new QHBoxLayout;
    auto addBtn = [&](const QString &lbl, auto slot, const QString &col = Theme::BG_SURFACE) {
        auto *b = new QPushButton(lbl);
        b->setStyleSheet(QString("QPushButton{background:%1;color:%2;border:1px solid %3;"
                                  "border-radius:6px;padding:6px 10px;font-size:12px;font-weight:600;}"
                                  "QPushButton:hover{border-color:%4;color:%4;}")
                          .arg(col, Theme::TEXT_PRI, Theme::BORDER, Theme::NEON_GREEN));
        connect(b, &QPushButton::clicked, this, slot);
        return b;
    };

    row1->addWidget(addBtn("~A  (NOT)",       &BitSetDemo::performNOT));
    row1->addWidget(addBtn("flip all A",      &BitSetDemo::performFlipAll));
    row1->addWidget(addBtn("flip бит A",      &BitSetDemo::performFlipBit));
    row1->addWidget(addBtn("set бит A",       &BitSetDemo::performSet));
    row1->addWidget(addBtn("reset бит A",     &BitSetDemo::performReset));
    row1->addWidget(addBtn("set ALL A",       &BitSetDemo::performSetAll));
    row1->addWidget(addBtn("reset ALL A",     &BitSetDemo::performResetAll));
    opLay->addLayout(row1);

    // Ряд 2: бинарные A op B → A
    auto *row2 = new QHBoxLayout;
    row2->addWidget(addBtn("A & B  (AND)", &BitSetDemo::performAND));
    row2->addWidget(addBtn("A | B  (OR)",  &BitSetDemo::performOR));
    row2->addWidget(addBtn("A ^ B  (XOR)", &BitSetDemo::performXOR));
    row2->addWidget(addBtn("A << n",       &BitSetDemo::performSHL));
    row2->addWidget(addBtn("A >> n",       &BitSetDemo::performSHR));
    opLay->addLayout(row2);

    // Ряд 3: предикаты
    auto *row3 = new QHBoxLayout;
    row3->addWidget(addBtn("all(A)",    &BitSetDemo::performAll));
    row3->addWidget(addBtn("any(A)",    &BitSetDemo::performAny));
    row3->addWidget(addBtn("none(A)",   &BitSetDemo::performNone));
    row3->addWidget(addBtn("count(A)",  &BitSetDemo::performCount));
    row3->addWidget(addBtn("to_string", &BitSetDemo::showString));
    row3->addWidget(addBtn("to_ullong", &BitSetDemo::showULLong));
    opLay->addLayout(row3);

    root->addWidget(opCard);

    // ── Лог операций ─────────────────────────────────────
    auto *logCard = new QGroupBox("Журнал операций");
    auto *logLay  = new QHBoxLayout(logCard);

    logOutput = new QTextEdit;
    logOutput->setReadOnly(true);
    logOutput->setMinimumHeight(120);
    logOutput->setMaximumHeight(160);
    logOutput->setStyleSheet(
        QString("QTextEdit{background:%1;color:%2;border:none;border-radius:6px;"
                "font-family:'Cascadia Code','Consolas',monospace;font-size:12px;}")
        .arg(Theme::BG_DEEP, Theme::TEXT_PRI));
    logLay->addWidget(logOutput);

    auto *clearBtn = new QPushButton("🗑");
    clearBtn->setFixedWidth(30);
    clearBtn->setToolTip("Очистить лог");
    connect(clearBtn, &QPushButton::clicked, logOutput, &QTextEdit::clear);
    logLay->addWidget(clearBtn, 0, Qt::AlignTop);

    root->addWidget(logCard);

    // ── Начальное приветствие ─────────────────────────────
    log(QString("<span style='color:%1;'>BitSet Demo готов. "
                "Нажмите «Создать два BitSet» для начала.</span>").arg(Theme::NEON_CYAN));

    // Связи кликов по битам
    connect(grid1, &BitGridWidget::bitToggled, this, &BitSetDemo::onBit1Toggled);
    connect(grid2, &BitGridWidget::bitToggled, this, &BitSetDemo::onBit2Toggled);
}

// ─── Создание ─────────────────────────────────────────────
void BitSetDemo::createBitSets()
{
    int sz = sizeSpin->value();
    bitIndexSpin->setMaximum(sz - 1);
    bitset1 = std::make_unique<BitSet>(sz);
    bitset2 = std::make_unique<BitSet>(sz);
    grid1->setBitSet(bitset1.get());
    grid2->setBitSet(bitset2.get());
    refreshInfoWidgets();
    log(QString("<span style='color:%1;'>Создано: A[%2] и B[%2] — все биты = 0</span>")
        .arg(Theme::NEON_GREEN).arg(sz));
}

// ─── Клики по битам ───────────────────────────────────────
void BitSetDemo::onBit1Toggled(int idx)
{
    if (!bitset1) return;
    bitset1->flip(idx);
    refreshInfoWidgets();
    logOp(QString("flip A[%1]").arg(idx),
          QString("A[%1] = %2").arg(idx).arg(bitset1->test(idx) ? "1" : "0"));
}

void BitSetDemo::onBit2Toggled(int idx)
{
    if (!bitset2) return;
    bitset2->flip(idx);
    refreshInfoWidgets();
    logOp(QString("flip B[%1]").arg(idx),
          QString("B[%1] = %2").arg(idx).arg(bitset2->test(idx) ? "1" : "0"));
}

// ─── Операции ─────────────────────────────────────────────
void BitSetDemo::performNOT()
{
    if (!bitset1) return;
    ~(*bitset1);
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp("~A (NOT)", "A инвертирован");
}

void BitSetDemo::performFlipAll()
{
    if (!bitset1) return;
    bitset1->flip();
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp("A.flip()", "все биты A инвертированы");
}

void BitSetDemo::performFlipBit()
{
    if (!bitset1) return;
    int idx = bitIndexSpin->value();
    bitset1->flip(idx);
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp(QString("A.flip(%1)").arg(idx),
          QString("A[%1] = %2").arg(idx).arg(bitset1->test(idx) ? "1" : "0"));
}

void BitSetDemo::performSet()
{
    if (!bitset1) return;
    int idx = bitIndexSpin->value();
    bitset1->set(idx, true);
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp(QString("A.set(%1)").arg(idx), QString("A[%1] = 1").arg(idx));
}

void BitSetDemo::performReset()
{
    if (!bitset1) return;
    int idx = bitIndexSpin->value();
    bitset1->reset(idx);
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp(QString("A.reset(%1)").arg(idx), QString("A[%1] = 0").arg(idx));
}

void BitSetDemo::performSetAll()
{
    if (!bitset1) return;
    bitset1->setAll();
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp("A.setAll()", QString("все %1 бит установлены в 1").arg(bitset1->size()));
}

void BitSetDemo::performResetAll()
{
    if (!bitset1) return;
    bitset1->resetAll();
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp("A.resetAll()", QString("все %1 бит сброшены в 0").arg(bitset1->size()));
}

void BitSetDemo::performAND()
{
    if (!bitset1 || !bitset2) return;
    *bitset1 &= *bitset2;
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp("A &= B  (AND)", QString("count(A) = %1").arg(bitset1->count()));
}

void BitSetDemo::performOR()
{
    if (!bitset1 || !bitset2) return;
    *bitset1 |= *bitset2;
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp("A |= B  (OR)", QString("count(A) = %1").arg(bitset1->count()));
}

void BitSetDemo::performXOR()
{
    if (!bitset1 || !bitset2) return;
    *bitset1 ^= *bitset2;
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp("A ^= B  (XOR)", QString("count(A) = %1").arg(bitset1->count()));
}

void BitSetDemo::performSHL()
{
    if (!bitset1) return;
    int n = shiftSpin->value();
    *bitset1 = *bitset1 << n;
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp(QString("A = A << %1").arg(n), QString("count(A) = %1").arg(bitset1->count()));
}

void BitSetDemo::performSHR()
{
    if (!bitset1) return;
    int n = shiftSpin->value();
    *bitset1 = *bitset1 >> n;
    grid1->setBitSet(bitset1.get());
    refreshInfoWidgets();
    logOp(QString("A = A >> %1").arg(n), QString("count(A) = %1").arg(bitset1->count()));
}

void BitSetDemo::performAll()
{
    if (!bitset1) return;
    bool r = bitset1->all();
    log(QString("<span style='color:%1;'>A.all() = <b>%2</b></span>")
        .arg(Theme::NEON_YELLOW).arg(r ? "true (все биты = 1)" : "false"));
}

void BitSetDemo::performAny()
{
    if (!bitset1) return;
    bool r = bitset1->any();
    log(QString("<span style='color:%1;'>A.any() = <b>%2</b></span>")
        .arg(Theme::NEON_YELLOW).arg(r ? "true (есть хотя бы один 1)" : "false (все = 0)"));
}

void BitSetDemo::performNone()
{
    if (!bitset1) return;
    bool r = bitset1->none();
    log(QString("<span style='color:%1;'>A.none() = <b>%2</b></span>")
        .arg(Theme::NEON_YELLOW).arg(r ? "true (все биты = 0)" : "false"));
}

void BitSetDemo::performCount()
{
    if (!bitset1) return;
    log(QString("<span style='color:%1;'>A.count() = <b>%2</b> бит установлено из %3</span>")
        .arg(Theme::NEON_YELLOW).arg(bitset1->count()).arg(bitset1->size()));
}

void BitSetDemo::showString()
{
    if (!bitset1) return;
    log(QString("<span style='color:%1;'>to_string(A) = </span>"
                "<span style='color:%2;font-family:monospace;'><b>%3</b></span>")
        .arg(Theme::TEXT_SEC, Theme::NEON_GREEN)
        .arg(QString::fromStdString(bitset1->to_string())));
}

void BitSetDemo::showULLong()
{
    if (!bitset1) return;
    try {
        unsigned long long v = bitset1->to_ullong();
        log(QString("<span style='color:%1;'>to_ullong(A) = </span>"
                    "<span style='color:%2;'><b>%3</b></span>"
                    "<span style='color:%4;'> (%5)</span>")
            .arg(Theme::TEXT_SEC, Theme::NEON_YELLOW)
            .arg(v)
            .arg(Theme::TEXT_DIM, QString::fromStdString(bitset1->to_hex())));
    } catch (const std::exception &e) {
        log(QString("<span style='color:%1;'>Ошибка: %2</span>")
            .arg(Theme::ERROR_CLR).arg(e.what()));
    }
}

// ─── Лог и обновление ─────────────────────────────────────
void BitSetDemo::log(const QString &msg, const QString &)
{
    QString ts = QDateTime::currentDateTime().toString("hh:mm:ss");
    logOutput->append(
        QString("<span style='color:%1;'>[%2]</span>  %3")
        .arg(Theme::TEXT_DIM, ts, msg));
    logOutput->verticalScrollBar()->setValue(
        logOutput->verticalScrollBar()->maximum());
}

void BitSetDemo::logOp(const QString &op, const QString &result)
{
    log(QString("<span style='color:%1;font-weight:700;'>%2</span>"
                "<span style='color:%3;'>  →  %4</span>")
        .arg(Theme::NEON_CYAN, op, Theme::TEXT_SEC, result));
}

void BitSetDemo::refreshInfoWidgets()
{
    info1->update(bitset1.get());
    info2->update(bitset2.get());
}