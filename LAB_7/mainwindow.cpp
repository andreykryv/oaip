#include "mainwindow.h"
#include <QApplication>
#include <QRandomGenerator>
#include <QFrame>
#include <QScrollBar>
#include <cmath>
#include <climits>
#include <sstream>

// ────────── Утилиты ──────────────────────────────────────────────
static QPushButton* mkBtn(const QString& t, bool secondary = false) {
    auto* b = new QPushButton(t);
    if (secondary) b->setObjectName("sec");
    b->setCursor(Qt::PointingHandCursor);
    return b;
}
static QLabel* mkLabel(const QString& t, int sz = 12, bool bold = false) {
    auto* l = new QLabel(t);
    QString css = QString("font-size:%1px;").arg(sz);
    if (bold) css += "font-weight:700;";
    l->setStyleSheet(css + "background:transparent;");
    return l;
}
static QGroupBox* mkCard(const QString& t) { return new QGroupBox(t); }

// ════════════════════════════════════════════════════════════════
//  MainWindow
// ════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Лабораторная работа №7  ·  Динамические структуры  ·  Вариант 7");
    resize(1280, 820);
    setMinimumSize(960, 640);

    auto* central = new QWidget(this);
    auto* vl = new QVBoxLayout(central);
    vl->setContentsMargins(0,0,0,0);
    vl->setSpacing(0);

    // ── Шапка ──────────────────────────────────────────────────
    auto* header = new QFrame(central);
    header->setFixedHeight(58);
    header->setStyleSheet("background:#0d0d24; border-bottom:1px solid #252545;");
    auto* hl = new QHBoxLayout(header);
    hl->setContentsMargins(20,0,20,0);
    auto* titleLbl = new QLabel("⟨λ⟩  ЛР №7  ·  Динамические структуры данных", header);
    titleLbl->setStyleSheet("font-size:16px; font-weight:700; color:#c0c0ff; background:transparent;");
    auto* varLbl = new QLabel("Вариант  7", header);
    varLbl->setStyleSheet(
        "font-size:11px; color:#7c4dff; background:#1a1030;"
        "border:1px solid #4a2a99; border-radius:5px; padding:3px 10px;");
    hl->addWidget(titleLbl);
    hl->addStretch();
    hl->addWidget(varLbl);

    // ── Вкладки ────────────────────────────────────────────────
    tabs_ = new QTabWidget(central);
    tabs_->setDocumentMode(true);

    task1_ = new Task1Widget(tabs_);
    task2_ = new Task2Widget(tabs_);
    task3_ = new Task3Widget(tabs_);
    task4_ = new Task4Widget(tabs_);
    task5_ = new Task5Widget(tabs_);

    tabs_->addTab(task1_, "  📦  Задание 1  ·  Очередь + Кольца  ");
    tabs_->addTab(task2_, "  🌳  Задание 2  ·  Дерево поиска  ");
    tabs_->addTab(task3_, "  🗂  Задание 3  ·  Хеш + Стеки  ");
    tabs_->addTab(task4_, "  🔑  Задание 4  ·  Открытая адресация  ");
    tabs_->addTab(task5_, "  🚀  Задание 5  ·  Deque  ");

    vl->addWidget(header);
    vl->addWidget(tabs_, 1);

    statusBar()->showMessage("  Готово  ·  Вариант 7");
    statusBar()->setStyleSheet("background:#0b0b1a; color:#5555a0; "
                               "border-top:1px solid #252545; font-size:11px;");
    setCentralWidget(central);

    connect(tabs_, &QTabWidget::currentChanged, [](int i){
        Q_UNUSED(i);
    });
}

// ════════════════════════════════════════════════════════════════
//  QueuePaintWidget
// ════════════════════════════════════════════════════════════════
void QueuePaintWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(8, 8, 20));

    if (!label_.isEmpty()) {
        p.setPen(QColor(120, 77, 255));
        p.setFont(QFont("Consolas", 10, QFont::Bold));
        p.drawText(QRect(8, 4, width()-16, 20), Qt::AlignLeft, label_);
    }

    if (vals_.empty()) {
        p.setPen(QColor(80, 80, 120));
        p.drawText(rect(), Qt::AlignCenter, "[ пусто ]");
        return;
    }

    const int N = (int)vals_.size();
    const int nodeW = std::max(44, std::min(70, (width() - 32) / N - 8));
    const int nodeH = 38;
    const int startY = (height() - nodeH) / 2 + (label_.isEmpty() ? 0 : 12);
    const int totalW = N * (nodeW + 8) - 8;
    int startX = (width() - totalW) / 2;
    if (startX < 8) startX = 8;

    for (int i = 0; i < N; ++i) {
        int x = startX + i * (nodeW + 8);
        int v = vals_[i];

        // Цвет узла
        QColor fill, border;
        if (v == hMax_)     { fill = QColor(50,30,100); border = QColor(150,100,255); }
        else if (v == hMin_){ fill = QColor(20,60,50);  border = QColor(0,200,140); }
        else                { fill = QColor(20,20,45);  border = QColor(50,50,90); }

        // Узел
        QPainterPath path;
        path.addRoundedRect(x, startY, nodeW, nodeH, 8, 8);
        p.fillPath(path, fill);
        p.setPen(QPen(border, 1.5));
        p.drawPath(path);

        // Значение
        p.setPen(QColor(220, 220, 255));
        p.setFont(QFont("Consolas", 10, QFont::Bold));
        p.drawText(QRect(x, startY, nodeW, nodeH), Qt::AlignCenter, QString::number(v));

        // Стрелки
        if (i > 0) {
            p.setPen(QPen(QColor(80, 80, 140), 1.5));
            int ax = startX + (i-1)*(nodeW+8) + nodeW;
            int ay = startY + nodeH/2;
            p.drawLine(ax, ay, x, ay);
            // Стрелка вправо
            p.drawLine(x, ay, x-5, ay-4);
            p.drawLine(x, ay, x-5, ay+4);
        }
        // Стрелка назад (двухсвязная)
        if (i < N-1) {
            p.setPen(QPen(QColor(60,60,110), 1));
            int ax2 = startX + (i+1)*(nodeW+8);
            int ay2 = startY + nodeH/2 + 5;
            p.drawLine(x+nodeW, ay2, ax2, ay2);
        }
    }

    // Кольцо: замыкающая стрелка
    if (isRing_ && N > 1) {
        p.setPen(QPen(QColor(100, 200, 180), 1.5, Qt::DashLine));
        int lx = startX + (N-1)*(nodeW+8) + nodeW;
        int ly = startY - 12;
        int rx = startX;
        p.drawLine(lx, startY + nodeH/2, lx, ly);
        p.drawLine(lx, ly, rx, ly);
        p.drawLine(rx, ly, rx, startY + nodeH/2);
        // стрелка вниз
        p.drawLine(rx, startY+nodeH/2, rx+5, startY+nodeH/2-4);
        p.drawLine(rx, startY+nodeH/2, rx-5, startY+nodeH/2-4);
    }
}

// ════════════════════════════════════════════════════════════════
//  BSTWidget
// ════════════════════════════════════════════════════════════════
void BSTWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(8,8,20));
    if (!tree_ || !tree_->root) {
        p.setPen(QColor(80,80,120));
        p.drawText(rect(), Qt::AlignCenter, "[ дерево пусто ]");
        return;
    }
    drawNode(p, tree_->root, width()/2.0, 30, width()/3.0, 0);
}

void BSTWidget::drawNode(QPainter& p, BSTNode* n, double x, double y, double dx, int level) {
    if (!n) return;
    const double nodeR = 22;
    const double dy = 52;

    // Рёбра
    if (n->left) {
        double cx = x - dx, cy = y + dy;
        p.setPen(QPen(QColor(50, 50, 100), 1.5));
        p.drawLine(QPointF(x, y+nodeR), QPointF(cx, cy-nodeR));
        drawNode(p, n->left, cx, cy, dx/1.8, level+1);
    }
    if (n->right) {
        double cx = x + dx, cy = y + dy;
        p.setPen(QPen(QColor(50, 50, 100), 1.5));
        p.drawLine(QPointF(x, y+nodeR), QPointF(cx, cy-nodeR));
        drawNode(p, n->right, cx, cy, dx/1.8, level+1);
    }

    // Узел
    bool hl = (n->key == hlKey_);
    QColor fill = hl ? QColor(80,40,160) : QColor(22,22,50);
    QColor bord = hl ? QColor(180,120,255) : QColor(60,60,110);
    p.setPen(QPen(bord, hl?2.0:1.5));
    p.setBrush(fill);
    p.drawEllipse(QPointF(x,y), nodeR, nodeR);

    // Ключ
    p.setPen(hl ? QColor(230,200,255) : QColor(200,200,240));
    p.setFont(QFont("Consolas", 9, QFont::Bold));
    p.drawText(QRectF(x-nodeR, y-nodeR, nodeR*2, nodeR*2), Qt::AlignCenter, QString::number(n->key));

    // Данные (под узлом маленьким шрифтом)
    p.setPen(QColor(100,100,160));
    p.setFont(QFont("Consolas", 7));
    QString shortData = QString::fromStdString(n->data).left(6);
    p.drawText(QRectF(x-30, y+nodeR+1, 60, 14), Qt::AlignCenter, shortData);
}

// ════════════════════════════════════════════════════════════════
//  HashTableWidget
// ════════════════════════════════════════════════════════════════
void HashTableWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(8,8,20));
    if (!ht_) return;

    int sz = ht_->tableSize;
    int cellW = std::max(30, (width()-32) / sz);
    int startX = (width() - cellW*sz) / 2;
    int startY = 12;

    for (int i = 0; i < sz; ++i) {
        int x = startX + i*cellW;
        bool hlit = (i == hl_);

        // Индекс
        p.setPen(hlit ? QColor(200,150,255) : QColor(80,80,130));
        p.setFont(QFont("Consolas", 8));
        p.drawText(QRect(x, startY, cellW, 14), Qt::AlignCenter, QString::number(i));

        // Стек
        auto stack = ht_->getStack(i);
        int sy = startY + 18;
        for (auto& [key, val] : stack) {
            QColor fill = hlit ? QColor(50,25,100) : QColor(18,18,40);
            QColor bord = hlit ? QColor(140,80,255) : QColor(45,45,80);
            p.setBrush(fill);
            p.setPen(QPen(bord, 1));
            p.drawRoundedRect(x+2, sy, cellW-4, 24, 4, 4);
            p.setPen(hlit ? QColor(220,180,255) : QColor(180,180,220));
            p.setFont(QFont("Consolas", 9, QFont::Bold));
            p.drawText(QRect(x+2, sy, cellW-4, 24), Qt::AlignCenter, QString::number(key));
            sy += 28;
        }
        if (stack.empty()) {
            p.setPen(QColor(40,40,70));
            p.drawText(QRect(x, sy, cellW, 20), Qt::AlignCenter, "—");
        }
    }

    // Граница снизу первого ряда (сетка)
    p.setPen(QColor(30,30,55));
    for (int i = 0; i <= sz; ++i) {
        int x = startX + i*cellW;
        p.drawLine(x, startY+14, x, startY+14+height());
    }
}

// ════════════════════════════════════════════════════════════════
//  OpenAddrWidget
// ════════════════════════════════════════════════════════════════
void OpenAddrWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(8,8,20));
    if (!ht_) return;

    int cap = ht_->capacity;
    int cols = std::min(cap, (width()-24) / 34);
    int rows = (cap + cols - 1) / cols;
    int cw = (width()-24) / cols;
    int ch = std::min(30, (height()-24) / std::max(1,rows));
    int sx = 12, sy = 12;

    for (int i = 0; i < cap; ++i) {
        int c = i % cols, r = i / cols;
        int x = sx + c*cw, y = sy + r*ch;
        auto& cell = ht_->table[i];
        QColor fill, bord, textC;
        if (cell.state == CellState::EMPTY) {
            fill = QColor(12,12,28); bord = QColor(28,28,50); textC = QColor(40,40,70);
        } else if (cell.state == CellState::DELETED) {
            fill = QColor(40,20,20); bord = QColor(80,30,30); textC = QColor(120,60,60);
        } else {
            fill = QColor(22,15,50); bord = QColor(80,60,150); textC = QColor(180,160,240);
        }
        p.setBrush(fill);
        p.setPen(QPen(bord, 1));
        p.drawRect(x, y, cw-2, ch-2);
        if (cell.state == CellState::OCCUPIED) {
            p.setPen(textC);
            p.setFont(QFont("Consolas", 8));
            p.drawText(QRect(x, y, cw-2, ch-2), Qt::AlignCenter, QString::number(cell.key));
        }
    }
}

// ════════════════════════════════════════════════════════════════
//  DequeWidget
// ════════════════════════════════════════════════════════════════
void DequeWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(8,8,20));
    if (!dq_ || dq_->empty()) {
        p.setPen(QColor(70,70,110));
        p.drawText(rect(), Qt::AlignCenter, "[ deque пуст ]");
        return;
    }

    auto stats = dq_->stats();
    int N = dq_->size();
    const int cellW = std::max(40, std::min(60, (width()-32)/std::max(1,N) - 4));
    const int cellH = 40;
    int totalW = N*(cellW+4)-4;
    int sx = (width()-totalW)/2; if(sx<8) sx=8;
    int sy = (height()-cellH)/2 - 8;

    // Метки FRONT / BACK
    p.setPen(QColor(100,200,180));
    p.setFont(QFont("Consolas", 9, QFont::Bold));
    p.drawText(sx-4, sy-5, "FRONT");
    p.drawText(sx+(N-1)*(cellW+4)+cellW-38, sy-5, "BACK");

    for (int i = 0; i < N; ++i) {
        int x = sx + i*(cellW+4);
        int v = (*dq_)[i];

        QColor fill = (i==0 || i==N-1) ? QColor(30,18,60) : QColor(18,18,40);
        QColor bord = (i==0 || i==N-1) ? QColor(120,80,220) : QColor(50,50,90);

        QPainterPath path;
        path.addRoundedRect(x, sy, cellW, cellH, 7, 7);
        p.fillPath(path, fill);
        p.setPen(QPen(bord, i==0||i==N-1 ? 2.0 : 1.2));
        p.drawPath(path);

        p.setPen(QColor(210,210,250));
        p.setFont(QFont("Consolas", 10, QFont::Bold));
        p.drawText(QRect(x, sy, cellW, cellH), Qt::AlignCenter, QString::number(v));

        // Индекс
        p.setPen(QColor(70,70,120));
        p.setFont(QFont("Consolas", 8));
        p.drawText(QRect(x, sy+cellH+2, cellW, 12), Qt::AlignCenter, QString("[%1]").arg(i));
    }

    // Статистика блоков
    QString stat = QString("size=%1  capacity=%2  blocks=%3  block_size=%4")
        .arg(stats.usedElements).arg(stats.capacity)
        .arg(stats.totalBlocks).arg(stats.blockSize);
    p.setPen(QColor(70,70,120));
    p.setFont(QFont("Consolas", 9));
    p.drawText(QRect(0, height()-18, width(), 16), Qt::AlignCenter, stat);
}

// ════════════════════════════════════════════════════════════════
//  BenchChart
// ════════════════════════════════════════════════════════════════
void BenchChart::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor(10,10,22));
    if (data_.empty()) return;

    const int ML=60, MR=20, MT=20, MB=40;
    int W = width()-ML-MR, H = height()-MT-MB;

    // Максимум для шкалы
    double maxT = 1.0;
    for (auto& b : data_) maxT = std::max(maxT, std::max(b.avgSearchTimeNs, b.avgFailTimeNs));

    // Сетка
    p.setPen(QPen(QColor(25,25,50), 1, Qt::DashLine));
    for (int i=0; i<=4; ++i) {
        int y = MT + H - i*H/4;
        p.drawLine(ML, y, ML+W, y);
        p.setPen(QColor(70,70,120));
        p.setFont(QFont("Consolas", 8));
        p.drawText(QRect(0, y-8, ML-4, 16), Qt::AlignRight|Qt::AlignVCenter,
                   QString::number((int)(maxT/4*i)) + "ns");
        p.setPen(QPen(QColor(25,25,50), 1, Qt::DashLine));
    }

    // Оси
    p.setPen(QPen(QColor(60,60,110), 1.5));
    p.drawLine(ML, MT, ML, MT+H);
    p.drawLine(ML, MT+H, ML+W, MT+H);

    int N = (int)data_.size();
    double barW = W / (N * 3.0 + 1);

    for (int i = 0; i < N; ++i) {
        auto& b = data_[i];
        double bx = ML + (i * 3 + 1) * barW;

        auto drawBar = [&](double bxOff, double val, QColor c) {
            int bh = (int)(H * val / maxT);
            int by = MT + H - bh;
            QPainterPath path;
            path.addRoundedRect(bxOff, by, barW*0.9, bh, 3, 3);
            QLinearGradient gr(bxOff, by, bxOff, by+bh);
            gr.setColorAt(0, c.lighter(140));
            gr.setColorAt(1, c);
            p.fillPath(path, gr);
        };
        drawBar(bx, b.avgSearchTimeNs, QColor(100,70,220));
        drawBar(bx + barW, b.avgFailTimeNs, QColor(0,180,140));

        // Подпись
        p.setPen(QColor(120,120,180));
        p.setFont(QFont("Consolas", 8));
        p.drawText(QRect((int)bx-10, MT+H+4, (int)(barW*2)+20, 16),
                   Qt::AlignCenter, QString("N=%1").arg(b.tableSize));
    }

    // Легенда
    p.fillRect(ML+W-110, MT+2, 12, 10, QColor(100,70,220));
    p.setPen(QColor(150,150,200));
    p.setFont(QFont("Consolas", 9));
    p.drawText(ML+W-95, MT+11, "Успешный поиск");
    p.fillRect(ML+W-110, MT+16, 12, 10, QColor(0,180,140));
    p.drawText(ML+W-95, MT+25, "Неуспешный поиск");
}

// ════════════════════════════════════════════════════════════════
//  TASK 1
// ════════════════════════════════════════════════════════════════
Task1Widget::Task1Widget(QWidget* parent) : QWidget(parent) {
    auto* main = new QVBoxLayout(this);
    main->setContentsMargins(16,12,16,12);
    main->setSpacing(10);

    // Описание
    auto* desc = new QLabel(
        "<b style='color:#9060ff;'>Вариант 7</b>  Создать двухсвязную очередь из случайных целых чисел. "
        "Из элементов, расположенных между максимальным и минимальным, создать <b>первое кольцо</b>. "
        "Остальные элементы — <b>второе кольцо</b>.");
    desc->setWordWrap(true);
    desc->setStyleSheet("color:#9090cc; font-size:12px; background:#0f0f22; "
                        "border:1px solid #252545; border-radius:6px; padding:8px;");
    main->addWidget(desc);

    // ── Панель управления ──────────────────────────────────────
    auto* ctrlCard = mkCard("Управление очередью");
    auto* ctrlL = new QVBoxLayout(ctrlCard);

    auto* row1 = new QHBoxLayout;
    countSpin_ = new QSpinBox; countSpin_->setRange(4,30); countSpin_->setValue(10);
    countSpin_->setPrefix("Размер: ");
    auto* genBtn = mkBtn("⚡ Сгенерировать");
    auto* buildBtn = mkBtn("🔄 Построить кольца", false);
    row1->addWidget(countSpin_);
    row1->addWidget(genBtn);
    row1->addWidget(buildBtn);
    row1->addStretch();
    ctrlL->addLayout(row1);

    auto* row2 = new QHBoxLayout;
    valSpin_ = new QSpinBox; valSpin_->setRange(-99,99); valSpin_->setValue(42);
    valSpin_->setPrefix("Значение: ");
    auto* pbBtn  = mkBtn("push_back",  true);
    auto* pfBtn  = mkBtn("push_front", true);
    auto* pfpBtn = mkBtn("pop_front",  true);
    auto* pbkBtn = mkBtn("pop_back",   true);
    row2->addWidget(valSpin_);
    row2->addWidget(pbBtn); row2->addWidget(pfBtn);
    row2->addWidget(pfpBtn); row2->addWidget(pbkBtn);
    row2->addStretch();
    ctrlL->addLayout(row2);
    main->addWidget(ctrlCard);

    // ── Главная очередь ────────────────────────────────────────
    auto* qCard = mkCard("Двухсвязная очередь  (MAX = фиолетовый, MIN = бирюзовый)");
    auto* qL = new QVBoxLayout(qCard);
    mainQView_ = new QueuePaintWidget;
    mainQView_->setFixedHeight(90);
    qL->addWidget(mainQView_);
    infoLabel_ = mkLabel("", 11);
    qL->addWidget(infoLabel_);
    main->addWidget(qCard);

    // ── Кольца ─────────────────────────────────────────────────
    auto* ringCard = mkCard("Кольца (результат варианта 7)");
    auto* ringL = new QVBoxLayout(ringCard);
    ring1View_ = new QueuePaintWidget;
    ring1View_->setFixedHeight(90);
    ring2View_ = new QueuePaintWidget;
    ring2View_->setFixedHeight(90);
    ringL->addWidget(ring1View_);
    ringL->addWidget(ring2View_);
    main->addWidget(ringCard);

    // ── Лог ────────────────────────────────────────────────────
    auto* logCard = mkCard("Лог операций");
    auto* logL = new QVBoxLayout(logCard);
    log_ = new QTextEdit; log_->setReadOnly(true); log_->setMaximumHeight(120);
    logL->addWidget(log_);
    main->addWidget(logCard);

    // Подключения
    connect(genBtn,   &QPushButton::clicked, this, &Task1Widget::onGenerate);
    connect(buildBtn, &QPushButton::clicked, this, &Task1Widget::onBuildRings);
    connect(pbBtn,    &QPushButton::clicked, this, &Task1Widget::onPushBack);
    connect(pfBtn,    &QPushButton::clicked, this, &Task1Widget::onPushFront);
    connect(pfpBtn,   &QPushButton::clicked, this, &Task1Widget::onPopFront);
    connect(pbkBtn,   &QPushButton::clicked, this, &Task1Widget::onPopBack);

    onGenerate();
}

void Task1Widget::logLine(const QString& s, const QString& col) {
    log_->append(QString("<span style='color:%1;'>%2</span>").arg(col, s));
    log_->verticalScrollBar()->setValue(log_->verticalScrollBar()->maximum());
}

void Task1Widget::refresh() {
    auto vals = queue_.toVector();
    auto* mx = queue_.findMax();
    auto* mn = queue_.findMin();
    int mv = mx ? mx->data : INT_MIN;
    int mnv = mn ? mn->data : INT_MAX;
    mainQView_->setData(vals, mv, mnv);
    infoLabel_->setText(QString("  Размер: %1   Max: %2   Min: %3")
        .arg(queue_.size()).arg(mv).arg(mnv));

    if (rings_) {
        ring1View_->setData(rings_->ring1.toVector(), INT_MIN, INT_MAX, true,
                            QString("Кольцо 1  (между max=%1 и min=%2)")
                            .arg(rings_->maxVal).arg(rings_->minVal));
        ring2View_->setData(rings_->ring2.toVector(), INT_MIN, INT_MAX, true,
                            "Кольцо 2  (остальные элементы)");
    } else {
        ring1View_->setData({}, INT_MIN, INT_MAX, false, "Кольцо 1  [не построено]");
        ring2View_->setData({}, INT_MIN, INT_MAX, false, "Кольцо 2  [не построено]");
    }
    fadeIn(mainQView_, 180);
}

void Task1Widget::onGenerate() {
    queue_.clear(); rings_.reset();
    int n = countSpin_->value();
    for (int i = 0; i < n; ++i) {
        int v = QRandomGenerator::global()->bounded(200) - 100;
        queue_.push_back(v);
    }
    logLine(QString("✦ Сгенерирована очередь из %1 элементов").arg(n), "#7c4dff");
    refresh();
}

void Task1Widget::onBuildRings() {
    if (queue_.empty()) { logLine("✗ Очередь пуста", "#ff4466"); return; }
    rings_ = buildRings(queue_);
    logLine(QString("✦ Кольца построены  |  max=%1 (pos %2)  min=%3 (pos %4)  ring1.size=%5  ring2.size=%6")
        .arg(rings_->maxVal).arg(rings_->maxPos)
        .arg(rings_->minVal).arg(rings_->minPos)
        .arg(rings_->ring1.size_).arg(rings_->ring2.size_),
        "#00c896");
    refresh();
}

void Task1Widget::onPushBack() {
    rings_.reset();
    queue_.push_back(valSpin_->value());
    logLine(QString("push_back(%1)").arg(valSpin_->value()), "#9068ff");
    refresh();
}
void Task1Widget::onPushFront() {
    rings_.reset();
    queue_.push_front(valSpin_->value());
    logLine(QString("push_front(%1)").arg(valSpin_->value()), "#9068ff");
    refresh();
}
void Task1Widget::onPopFront() {
    if (queue_.empty()) { logLine("✗ Очередь пуста", "#ff4466"); return; }
    rings_.reset();
    int v = queue_.pop_front();
    logLine(QString("pop_front() → %1").arg(v), "#ff9040");
    refresh();
}
void Task1Widget::onPopBack() {
    if (queue_.empty()) { logLine("✗ Очередь пуста", "#ff4466"); return; }
    rings_.reset();
    int v = queue_.pop_back();
    logLine(QString("pop_back() → %1").arg(v), "#ff9040");
    refresh();
}

// ════════════════════════════════════════════════════════════════
//  TASK 2
// ════════════════════════════════════════════════════════════════
Task2Widget::Task2Widget(QWidget* parent) : QWidget(parent) {
    auto* main = new QVBoxLayout(this);
    main->setContentsMargins(14,10,14,10); main->setSpacing(8);

    auto* desc = new QLabel(
        "<b style='color:#9060ff;'>Вариант 7</b>  "
        "Определить количество символов во всех строках, находящихся в узлах дерева.");
    desc->setWordWrap(true);
    desc->setStyleSheet("color:#9090cc; font-size:12px; background:#0f0f22; "
                        "border:1px solid #252545; border-radius:6px; padding:8px;");
    main->addWidget(desc);

    auto* split = new QSplitter(Qt::Horizontal);

    // Левая панель
    auto* left = new QWidget; auto* ll = new QVBoxLayout(left); ll->setContentsMargins(0,0,4,0);
    auto* ctrl = mkCard("Управление деревом");
    auto* cl = new QVBoxLayout(ctrl);

    auto* r1 = new QHBoxLayout;
    auto* genBtn = mkBtn("⚡ Генерировать");
    auto* balBtn = mkBtn("⚖ Балансировать", true);
    r1->addWidget(genBtn); r1->addWidget(balBtn);
    cl->addLayout(r1);

    auto* r2 = new QHBoxLayout;
    keySpin_ = new QSpinBox; keySpin_->setRange(-999,999); keySpin_->setValue(50);
    keySpin_->setPrefix("Ключ: ");
    dataEdit_ = new QLineEdit("данные"); dataEdit_->setPlaceholderText("Строка данных");
    r2->addWidget(keySpin_); r2->addWidget(dataEdit_);
    cl->addLayout(r2);

    auto* r3 = new QHBoxLayout;
    auto* insBtn = mkBtn("+ Вставить");
    auto* schBtn = mkBtn("🔍 Найти", true);
    auto* delBtn = mkBtn("✕ Удалить", true);
    r3->addWidget(insBtn); r3->addWidget(schBtn); r3->addWidget(delBtn);
    cl->addLayout(r3);
    ll->addWidget(ctrl);

    auto* opCard = mkCard("Операции варианта 7");
    auto* ol = new QVBoxLayout(opCard);
    auto* cntBtn = mkBtn("Σ Подсчитать символы");
    auto* ioBtn = mkBtn("Обход: inorder",  true);
    auto* poBtn = mkBtn("Обход: preorder", true);
    auto* rstBtn= mkBtn("Обход: postorder",true);
    ol->addWidget(cntBtn); ol->addWidget(ioBtn); ol->addWidget(poBtn); ol->addWidget(rstBtn);
    ll->addWidget(opCard);

    resultLabel_ = mkLabel("", 12, true);
    resultLabel_->setStyleSheet("color:#00e8a0; font-size:13px; font-weight:700; "
                                "background:#0d1a14; border:1px solid #003020; "
                                "border-radius:6px; padding:8px;");
    ll->addWidget(resultLabel_);

    log_ = new QTextEdit; log_->setReadOnly(true);
    ll->addWidget(log_, 1);
    split->addWidget(left);

    // Правая: дерево
    auto* right = new QWidget; auto* rl = new QVBoxLayout(right); rl->setContentsMargins(4,0,0,0);
    auto* tvCard = mkCard("Визуализация дерева (Ключ→Данные)");
    auto* tvl = new QVBoxLayout(tvCard);
    auto* tvScroll = new QScrollArea;
    tvScroll->setWidgetResizable(true);
    treeView_ = new BSTWidget;
    treeView_->setMinimumSize(500, 340);
    tvScroll->setWidget(treeView_);
    tvl->addWidget(tvScroll);
    rl->addWidget(tvCard, 1);
    split->addWidget(right);
    split->setSizes({320, 700});

    main->addWidget(split, 1);

    connect(genBtn, &QPushButton::clicked, this, &Task2Widget::onGenerate);
    connect(balBtn, &QPushButton::clicked, this, &Task2Widget::onBalance);
    connect(insBtn, &QPushButton::clicked, this, &Task2Widget::onInsert);
    connect(schBtn, &QPushButton::clicked, this, &Task2Widget::onSearch);
    connect(delBtn, &QPushButton::clicked, this, &Task2Widget::onRemove);
    connect(cntBtn, &QPushButton::clicked, this, &Task2Widget::onCountChars);
    connect(ioBtn,  &QPushButton::clicked, this, &Task2Widget::onInorder);
    connect(poBtn,  &QPushButton::clicked, this, &Task2Widget::onPreorder);
    connect(rstBtn, &QPushButton::clicked, this, &Task2Widget::onPostorder);

    onGenerate();
}

void Task2Widget::logLine(const QString& s, const QString& col) {
    log_->append(QString("<span style='color:%1;'>%2</span>").arg(col, s));
    log_->verticalScrollBar()->setValue(log_->verticalScrollBar()->maximum());
}

static const char* NAMES[] = {"Alice","Bob","Carol","Dave","Eve","Frank","Grace",
    "Hank","Iris","Jake","Kate","Leon","Mia","Nick","Olivia","Paul","Quinn",
    "Ruth","Sam","Tina","Uma","Victor","Wendy","Xia","Yuri","Zara"};

void Task2Widget::onGenerate() {
    // Очищаем BST
    delete tree_.root; tree_.root = nullptr;
    QRandomGenerator rng(QDateTime::currentMSecsSinceEpoch() & 0xFFFF);
    for (int i = 0; i < 12; ++i) {
        int k = rng.bounded(200) - 100;
        int ni = rng.bounded(26);
        tree_.insert(k, NAMES[ni]);
    }
    treeView_->setTree(&tree_);
    treeView_->clearHighlight();
    logLine(QString("✦ Сгенерировано дерево (%1 узлов)").arg(12), "#7c4dff");
    resultLabel_->setText("");
    fadeIn(treeView_, 200);
}

void Task2Widget::onBalance() {
    tree_.balance();
    treeView_->setTree(&tree_);
    treeView_->clearHighlight();
    logLine("✦ Дерево сбалансировано", "#00c896");
}

void Task2Widget::onInsert() {
    int k = keySpin_->value();
    std::string d = dataEdit_->text().toStdString();
    tree_.insert(k, d);
    treeView_->setTree(&tree_);
    treeView_->setHighlight(k);
    logLine(QString("insert(%1, \"%2\")").arg(k).arg(dataEdit_->text()), "#9068ff");
}

void Task2Widget::onSearch() {
    int k = keySpin_->value();
    auto* n = tree_.search(k);
    if (n) {
        treeView_->setHighlight(k);
        logLine(QString("search(%1) → \"%2\"").arg(k).arg(QString::fromStdString(n->data)), "#00c896");
    } else {
        treeView_->clearHighlight();
        logLine(QString("search(%1) → не найдено").arg(k), "#ff6080");
    }
}

void Task2Widget::onRemove() {
    int k = keySpin_->value();
    tree_.remove(k);
    treeView_->setTree(&tree_);
    treeView_->clearHighlight();
    logLine(QString("remove(%1)").arg(k), "#ff9040");
}

void Task2Widget::onCountChars() {
    int cnt = tree_.countAllChars();
    resultLabel_->setText(QString("Σ Символов во всех строках дерева: %1").arg(cnt));
    logLine(QString("countAllChars() = %1").arg(cnt), "#ffd040");
    fadeIn(resultLabel_, 300);
}

void Task2Widget::onInorder() {
    QString s = "Inorder: ";
    tree_.inorder([&](BSTNode* n){
        s += QString("(%1:%2) ").arg(n->key).arg(QString::fromStdString(n->data));
    });
    logLine(s, "#80d0ff");
}
void Task2Widget::onPreorder() {
    QString s = "Preorder: ";
    tree_.preorder([&](BSTNode* n){
        s += QString("(%1:%2) ").arg(n->key).arg(QString::fromStdString(n->data));
    });
    logLine(s, "#80d0ff");
}
void Task2Widget::onPostorder() {
    QString s = "Postorder: ";
    tree_.postorder([&](BSTNode* n){
        s += QString("(%1:%2) ").arg(n->key).arg(QString::fromStdString(n->data));
    });
    logLine(s, "#80d0ff");
}

// ════════════════════════════════════════════════════════════════
//  TASK 3
// ════════════════════════════════════════════════════════════════
Task3Widget::Task3Widget(QWidget* parent) : QWidget(parent) {
    auto* main = new QVBoxLayout(this);
    main->setContentsMargins(14,10,14,10); main->setSpacing(8);

    auto* desc = new QLabel(
        "<b style='color:#9060ff;'>Вариант 7</b>  "
        "Хеш-таблица на массиве стеков. Найти номер стека, содержащего минимальное значение ключа.");
    desc->setWordWrap(true);
    desc->setStyleSheet("color:#9090cc; font-size:12px; background:#0f0f22; "
                        "border:1px solid #252545; border-radius:6px; padding:8px;");
    main->addWidget(desc);

    // Управление
    auto* ctrl = mkCard("Управление хеш-таблицей  (размер=11, h(k) = k mod 11)");
    auto* cl = new QHBoxLayout(ctrl);
    auto* genBtn  = mkBtn("⚡ Генерировать");
    auto* findBtn = mkBtn("🔍 Найти стек min");
    keySpin_ = new QSpinBox; keySpin_->setRange(-999,999); keySpin_->setValue(37);
    keySpin_->setPrefix("Ключ: ");
    auto* insBtn  = mkBtn("+ Вставить", true);
    auto* delBtn  = mkBtn("✕ Удалить",  true);
    auto* schBtn  = mkBtn("? Найти",    true);
    cl->addWidget(genBtn); cl->addWidget(findBtn);
    cl->addStretch();
    cl->addWidget(keySpin_); cl->addWidget(insBtn);
    cl->addWidget(delBtn); cl->addWidget(schBtn);
    main->addWidget(ctrl);

    resultLabel_ = mkLabel("", 13, true);
    resultLabel_->setStyleSheet("color:#ffd040; font-size:14px; font-weight:700;"
                                "background:#1a1600; border:1px solid #3a3000;"
                                "border-radius:6px; padding:8px;");
    main->addWidget(resultLabel_);

    // Визуализация
    auto* htCard = mkCard("Визуализация стеков");
    auto* htL = new QVBoxLayout(htCard);
    htView_ = new HashTableWidget;
    htView_->setMinimumHeight(220);
    htL->addWidget(htView_);
    main->addWidget(htCard, 1);

    log_ = new QTextEdit; log_->setReadOnly(true); log_->setMaximumHeight(110);
    main->addWidget(log_);

    connect(genBtn,  &QPushButton::clicked, this, &Task3Widget::onGenerate);
    connect(findBtn, &QPushButton::clicked, this, &Task3Widget::onFindMinStack);
    connect(insBtn,  &QPushButton::clicked, this, &Task3Widget::onInsert);
    connect(delBtn,  &QPushButton::clicked, this, &Task3Widget::onRemove);
    connect(schBtn,  &QPushButton::clicked, this, &Task3Widget::onSearch);

    onGenerate();
}

void Task3Widget::logLine(const QString& s, const QString& col) {
    log_->append(QString("<span style='color:%1;'>%2</span>").arg(col, s));
    log_->verticalScrollBar()->setValue(log_->verticalScrollBar()->maximum());
}
void Task3Widget::refreshView(int hl) { htView_->setTable(&ht_, hl); fadeIn(htView_,150); }

void Task3Widget::onGenerate() {
    ht_.clear();
    for (int i = 0; i < 16; ++i) {
        int k = QRandomGenerator::global()->bounded(200) - 100;
        ht_.insert(k);
    }
    resultLabel_->setText("");
    logLine(QString("✦ Таблица заполнена (%1 элементов)").arg(ht_.totalElements()), "#7c4dff");
    refreshView();
}

void Task3Widget::onFindMinStack() {
    int idx = ht_.findStackWithMinStack_helper();  // will be called below
    idx = ht_.findStackWithMinKey();
    int minK = ht_.getMinKey();
    resultLabel_->setText(QString("  ★  Стек с мин. ключом: #%1  (min key = %2)").arg(idx).arg(minK));
    logLine(QString("findStackWithMinKey() → стек #%1  (key=%2)").arg(idx).arg(minK), "#ffd040");
    refreshView(idx);
    fadeIn(resultLabel_, 350);
}

void Task3Widget::onInsert() {
    ht_.insert(keySpin_->value());
    logLine(QString("insert(%1)").arg(keySpin_->value()), "#9068ff");
    refreshView();
}
void Task3Widget::onRemove() {
    bool ok = ht_.remove(keySpin_->value());
    logLine(QString("remove(%1) → %2").arg(keySpin_->value()).arg(ok?"ok":"не найден"),
            ok ? "#ff9040" : "#ff4466");
    refreshView();
}
void Task3Widget::onSearch() {
    auto* n = ht_.search(keySpin_->value());
    if (n) {
        int idx = ht_.hash(keySpin_->value());
        logLine(QString("search(%1) → найден в стеке #%2").arg(keySpin_->value()).arg(idx), "#00c896");
        refreshView(idx);
    } else {
        logLine(QString("search(%1) → не найден").arg(keySpin_->value()), "#ff6080");
        refreshView();
    }
}

// ════════════════════════════════════════════════════════════════
//  TASK 4
// ════════════════════════════════════════════════════════════════
Task4Widget::Task4Widget(QWidget* parent) : QWidget(parent) {
    auto* main = new QVBoxLayout(this);
    main->setContentsMargins(14,10,14,10); main->setSpacing(8);

    auto* desc = new QLabel(
        "<b style='color:#9060ff;'>Вариант 7</b>  "
        "Хеш-таблица с открытой адресацией. Универсальная хеш-функция: "
        "<code>h(k) = ((a·k + b) mod p) mod m</code> "
        "где p=10⁹+7, a и b случайные. Линейное зондирование коллизий.");
    desc->setWordWrap(true);
    desc->setStyleSheet("color:#9090cc; font-size:12px; background:#0f0f22; "
                        "border:1px solid #252545; border-radius:6px; padding:8px;");
    main->addWidget(desc);

    // Управление
    auto* ctrl = mkCard("Управление");
    auto* cl = new QHBoxLayout(ctrl);
    sizeSpin_ = new QSpinBox; sizeSpin_->setRange(8,4096); sizeSpin_->setValue(64);
    sizeSpin_->setPrefix("Размер: ");
    auto* genBtn  = mkBtn("⚡ Генерировать");
    keySpin_ = new QSpinBox; keySpin_->setRange(-9999,9999); keySpin_->setValue(42);
    keySpin_->setPrefix("Ключ: ");
    auto* insBtn = mkBtn("+ Вставить", true);
    auto* schBtn = mkBtn("🔍 Найти", true);
    auto* delBtn = mkBtn("✕ Удалить", true);
    cl->addWidget(sizeSpin_); cl->addWidget(genBtn);
    cl->addStretch();
    cl->addWidget(keySpin_); cl->addWidget(insBtn);
    cl->addWidget(schBtn); cl->addWidget(delBtn);
    main->addWidget(ctrl);

    // Таблица
    auto* htCard = mkCard("Визуализация ячеек (фиолетовый=занят, красный=удалён, тёмный=пуст)");
    auto* htL = new QVBoxLayout(htCard);
    htView_ = new OpenAddrWidget;
    htView_->setMinimumHeight(130);
    htL->addWidget(htView_);
    benchLabel_ = mkLabel("", 11);
    htL->addWidget(benchLabel_);
    main->addWidget(htCard);

    // График
    auto* benchCard = mkCard("Тест производительности  (N=16, 64, 128, 2048)");
    auto* benchL = new QVBoxLayout(benchCard);
    auto* runBtn = mkBtn("▶ Запустить бенчмарк");
    benchL->addWidget(runBtn);
    chartWidget_ = new BenchChart;
    static_cast<BenchChart*>(chartWidget_)->setMinimumHeight(200);
    benchL->addWidget(chartWidget_, 1);
    main->addWidget(benchCard, 1);

    log_ = new QTextEdit; log_->setReadOnly(true); log_->setMaximumHeight(100);
    main->addWidget(log_);

    connect(genBtn,  &QPushButton::clicked, this, &Task4Widget::onGenerate);
    connect(insBtn,  &QPushButton::clicked, this, &Task4Widget::onInsert);
    connect(schBtn,  &QPushButton::clicked, this, &Task4Widget::onSearch);
    connect(delBtn,  &QPushButton::clicked, this, &Task4Widget::onRemove);
    connect(runBtn,  &QPushButton::clicked, this, &Task4Widget::onRunBenchmark);

    rebuildHT();
    onGenerate();
}

void Task4Widget::logLine(const QString& s, const QString& col) {
    log_->append(QString("<span style='color:%1;'>%2</span>").arg(col, s));
    log_->verticalScrollBar()->setValue(log_->verticalScrollBar()->maximum());
}

void Task4Widget::rebuildHT() {
    ht_ = std::make_unique<UniversalHashTable>(sizeSpin_->value(),
              QRandomGenerator::global()->generate());
    htView_->setTable(ht_.get());
}

void Task4Widget::onGenerate() {
    rebuildHT();
    int n = (int)(ht_->capacity * 0.7);
    for (int i = 0; i < n; ++i) {
        int k = QRandomGenerator::global()->bounded(20000) - 10000;
        ht_->insert(k);
    }
    htView_->setTable(ht_.get());
    benchLabel_->setText(QString("  Размер: %1  Заполнено: %2  Коэффициент: %.2f  Коллизий: %3")
        .arg(ht_->capacity).arg(ht_->size_)
        .arg(ht_->loadFactor()).arg(ht_->totalCollisions));
    logLine(QString("✦ Таблица заполнена  size=%1  collisions=%2")
        .arg(ht_->size_).arg(ht_->totalCollisions), "#7c4dff");
    fadeIn(htView_,150);
}

void Task4Widget::onInsert() {
    if (!ht_) rebuildHT();
    bool ok = ht_->insert(keySpin_->value());
    htView_->setTable(ht_.get());
    logLine(QString("insert(%1) → %2").arg(keySpin_->value()).arg(ok?"ok":"переполнено"),
            ok?"#9068ff":"#ff4466");
}
void Task4Widget::onSearch() {
    if (!ht_) return;
    int idx = ht_->search(keySpin_->value());
    logLine(QString("search(%1) → %2").arg(keySpin_->value())
        .arg(idx>=0 ? QString("ячейка [%1]").arg(idx) : "не найден"),
        idx>=0 ? "#00c896" : "#ff6080");
}
void Task4Widget::onRemove() {
    if (!ht_) return;
    bool ok = ht_->remove(keySpin_->value());
    htView_->setTable(ht_.get());
    logLine(QString("remove(%1) → %2").arg(keySpin_->value()).arg(ok?"ok":"не найден"),
            ok?"#ff9040":"#ff4466");
}

void Task4Widget::onRunBenchmark() {
    logLine("▶ Запуск бенчмарка...", "#ffd040");
    benchResults_ = runAllBenchmarks();
    static_cast<BenchChart*>(chartWidget_)->setData(benchResults_);
    for (auto& b : benchResults_) {
        logLine(QString("  N=%1  LF=%.2f  succ=%.1fns  fail=%.1fns  coll=%2")
            .arg(b.tableSize).arg(b.loadFactor)
            .arg(b.avgSearchTimeNs).arg(b.avgFailTimeNs).arg(b.collisions), "#a0d8ff");
    }
    fadeIn(chartWidget_, 300);
}

// ════════════════════════════════════════════════════════════════
//  TASK 5
// ════════════════════════════════════════════════════════════════
Task5Widget::Task5Widget(QWidget* parent) : QWidget(parent) {
    auto* main = new QVBoxLayout(this);
    main->setContentsMargins(14,10,14,10); main->setSpacing(8);

    auto* desc = new QLabel(
        "<b style='color:#9060ff;'>Задание 5 — Deque</b>  "
        "Блочная структура deque. Каждый блок = 8 элементов. "
        "Итераторы не инвалидируются при resize. "
        "push_back / push_front / pop_back / pop_front — O(1), random access — O(1).");
    desc->setWordWrap(true);
    desc->setStyleSheet("color:#9090cc; font-size:12px; background:#0f0f22; "
                        "border:1px solid #252545; border-radius:6px; padding:8px;");
    main->addWidget(desc);

    auto* ctrl = mkCard("Управление Deque");
    auto* cl = new QHBoxLayout(ctrl);
    valSpin_ = new QSpinBox; valSpin_->setRange(-999,999); valSpin_->setValue(42);
    valSpin_->setPrefix("Значение: ");
    auto* pbBtn  = mkBtn("push_back");
    auto* pfBtn  = mkBtn("push_front");
    auto* ppbBtn = mkBtn("pop_back", true);
    auto* ppfBtn = mkBtn("pop_front", true);
    auto* clrBtn = mkBtn("clear", true);
    auto* rndBtn = mkBtn("⚡ Заполнить", true);
    cl->addWidget(valSpin_); cl->addWidget(pbBtn); cl->addWidget(pfBtn);
    cl->addWidget(ppbBtn); cl->addWidget(ppfBtn);
    cl->addStretch();
    cl->addWidget(rndBtn); cl->addWidget(clrBtn);
    main->addWidget(ctrl);

    auto* accCard = mkCard("Random Access");
    auto* accL = new QHBoxLayout(accCard);
    idxSpin_ = new QSpinBox; idxSpin_->setRange(0,9999); idxSpin_->setValue(0);
    idxSpin_->setPrefix("Индекс: ");
    auto* accBtn = mkBtn("Получить [i]", true);
    statsLabel_ = mkLabel("", 12, true);
    statsLabel_->setStyleSheet("color:#00e8a0; font-weight:700; background:#0d1a14;"
                               "border:1px solid #003020; border-radius:6px; padding:6px;");
    accL->addWidget(idxSpin_); accL->addWidget(accBtn); accL->addStretch(); accL->addWidget(statsLabel_);
    main->addWidget(accCard);

    auto* dqCard = mkCard("Визуализация Deque (оранжевые = front/back)");
    auto* dqL = new QVBoxLayout(dqCard);
    dqView_ = new DequeWidget;
    dqView_->setMinimumHeight(150);
    dqL->addWidget(dqView_);
    main->addWidget(dqCard);

    log_ = new QTextEdit; log_->setReadOnly(true); log_->setMaximumHeight(120);
    main->addWidget(log_);

    connect(pbBtn,  &QPushButton::clicked, this, &Task5Widget::onPushBack);
    connect(pfBtn,  &QPushButton::clicked, this, &Task5Widget::onPushFront);
    connect(ppbBtn, &QPushButton::clicked, this, &Task5Widget::onPopBack);
    connect(ppfBtn, &QPushButton::clicked, this, &Task5Widget::onPopFront);
    connect(clrBtn, &QPushButton::clicked, this, &Task5Widget::onClear);
    connect(rndBtn, &QPushButton::clicked, this, &Task5Widget::onRandomFill);
    connect(accBtn, &QPushButton::clicked, this, &Task5Widget::onRandomAccess);

    onRandomFill();
}

void Task5Widget::logLine(const QString& s, const QString& col) {
    log_->append(QString("<span style='color:%1;'>%2</span>").arg(col, s));
    log_->verticalScrollBar()->setValue(log_->verticalScrollBar()->maximum());
}

void Task5Widget::refresh() {
    dqView_->setDeque(&dq_);
    auto st = dq_.stats();
    statsLabel_->setText(QString("size=%1  cap=%2  blocks=%3")
        .arg(st.usedElements).arg(st.capacity).arg(st.totalBlocks));
    fadeIn(dqView_, 150);
}

void Task5Widget::onPushBack() {
    dq_.push_back(valSpin_->value());
    logLine(QString("push_back(%1)  size→%2").arg(valSpin_->value()).arg(dq_.size()), "#7c4dff");
    refresh();
}
void Task5Widget::onPushFront() {
    dq_.push_front(valSpin_->value());
    logLine(QString("push_front(%1)  size→%2").arg(valSpin_->value()).arg(dq_.size()), "#7c4dff");
    refresh();
}
void Task5Widget::onPopBack() {
    if (dq_.empty()) { logLine("✗ deque пуст", "#ff4466"); return; }
    int v = dq_.back(); dq_.pop_back();
    logLine(QString("pop_back() → %1  size→%2").arg(v).arg(dq_.size()), "#ff9040");
    refresh();
}
void Task5Widget::onPopFront() {
    if (dq_.empty()) { logLine("✗ deque пуст", "#ff4466"); return; }
    int v = dq_.front(); dq_.pop_front();
    logLine(QString("pop_front() → %1  size→%2").arg(v).arg(dq_.size()), "#ff9040");
    refresh();
}
void Task5Widget::onClear() {
    dq_.clear();
    logLine("clear()", "#ff6080");
    refresh();
}
void Task5Widget::onRandomFill() {
    dq_.clear();
    for (int i = 0; i < 12; ++i) {
        int v = QRandomGenerator::global()->bounded(200) - 100;
        if (QRandomGenerator::global()->bounded(2)) dq_.push_back(v);
        else dq_.push_front(v);
    }
    logLine(QString("✦ Заполнено случайно (%1 эл.)").arg(dq_.size()), "#00c896");
    refresh();
}
void Task5Widget::onRandomAccess() {
    int i = idxSpin_->value();
    if (i < 0 || i >= dq_.size()) {
        logLine(QString("✗ Индекс %1 вне диапазона [0, %2)").arg(i).arg(dq_.size()), "#ff4466");
        return;
    }
    int v = dq_[i];
    logLine(QString("operator[%1] = %2").arg(i).arg(v), "#a0d8ff");
}
