#include "mainwindow.h"
#include "vector.h"
#include "pair.h"
#include "sorting.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QSplitter>
#include <QRandomGenerator>
#include <QElapsedTimer>
#include <QScrollBar>
#include <QMessageBox>
#include <QFont>
#include <QPalette>
#include <QStatusBar>
#include <QFrame>

#include <vector>
#include <cstring>
#include <cmath>
#include <sstream>

// ═══════════════════════════════════════════════════════════
//  SortVisualizer Implementation
// ═══════════════════════════════════════════════════════════

SortVisualizer::SortVisualizer(QWidget* parent)
    : QWidget(parent), m_highlight(-1), m_minVal(0), m_maxVal(100) {
    setMinimumHeight(200);
    setStyleSheet("background:#0d1117; border:1px solid #30363d; border-radius:8px;");
}

void SortVisualizer::setData(const std::vector<int>& data) {
    m_data = data;
    if (!m_data.empty()) {
        m_minVal = *std::min_element(m_data.begin(), m_data.end());
        m_maxVal = *std::max_element(m_data.begin(), m_data.end());
        if (m_minVal == m_maxVal) { m_minVal--; m_maxVal++; }
    }
    update();
}

void SortVisualizer::setHighlight(int index) {
    m_highlight = index;
    update();
}

void SortVisualizer::setRange(int minVal, int maxVal) {
    m_minVal = minVal;
    m_maxVal = maxVal;
    if (m_minVal == m_maxVal) { m_minVal--; m_maxVal++; }
    update();
}

void SortVisualizer::setBarColor(const QColor& color) {
    m_barColor = color;
    update();
}

void SortVisualizer::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width() - 20;
    int h = height() - 20;
    int n = static_cast<int>(m_data.size());

    if (n == 0 || w <= 0 || h <= 0) return;

    double barWidth = static_cast<double>(w) / n;
    if (barWidth > 50) barWidth = 50;
    int totalWidth = static_cast<int>(barWidth * n);
    int startX = (width() - totalWidth) / 2;

    double range = m_maxVal - m_minVal;
    if (range <= 0) range = 1;

    for (int i = 0; i < n; ++i) {
        double normalized = (m_data[i] - m_minVal) / range;
        int barHeight = static_cast<int>(normalized * h * 0.9) + 10;
        int x = startX + static_cast<int>(i * barWidth);
        int y = height() - 10 - barHeight;

        QRect barRect(x, y, static_cast<int>(barWidth) - 2, barHeight);

        if (i == m_highlight) {
            painter.setBrush(QColor("#f0883e"));
            painter.setPen(QPen(QColor("#f0883e"), 2));
        } else {
            // Gradient-like coloring based on value
            float hue = 220 - normalized * 100; // Blue to green
            painter.setBrush(QColor::fromHslF(hue/360.0f, 0.7f, 0.5f));
            painter.setPen(QPen(m_barColor, 1));
        }

        painter.drawRoundedRect(barRect, 4, 4);

        // Draw value label for small arrays
        if (n <= 20 && barHeight > 20) {
            painter.setPen(QColor("#e6edf3"));
            painter.setFont(QFont("Segoe UI", 8));
            painter.drawText(barRect, Qt::AlignBottom | Qt::AlignHCenter, QString::number(m_data[i]));
        }
    }

    // Draw baseline
    painter.setPen(QPen(QColor("#30363d"), 2));
    painter.drawLine(10, height()-10, width()-10, height()-10);
}

// ─────────────────────────────────────────────────────────
//  Static helpers
// ─────────────────────────────────────────────────────────

QPushButton* MainWindow::makeBtn(const QString& text, const QString& color) {
    auto* b = new QPushButton(text);
    b->setFixedHeight(36);
    b->setFont(QFont("Segoe UI", 10, QFont::Medium));
    b->setCursor(Qt::PointingHandCursor);
    b->setStyleSheet(QString(
        "QPushButton{"
        "  background:%1; color:#fff; border:none;"
        "  border-radius:6px; padding:0 18px; font-weight:600;"
        "}"
        "QPushButton:hover{"
        "  background:%1; border:2px solid rgba(255,255,255,0.5);"
        "}"
        "QPushButton:pressed{"
        "  background:%1; border:2px solid rgba(255,255,255,0.9);"
        "}"
    ).arg(color));
    return b;
}

QGroupBox* MainWindow::makeGroup(const QString& title) {
    auto* g = new QGroupBox(title);
    g->setFont(QFont("Segoe UI", 10, QFont::Bold));
    g->setStyleSheet(
        "QGroupBox{"
        "  border:1px solid #3a3f4b; border-radius:8px;"
        "  margin-top:10px; padding:8px;"
        "  color:#c9d1d9; font-weight:700;"
        "}"
        "QGroupBox::title{subcontrol-origin:margin; left:10px; padding:0 4px;}"
    );
    return g;
}

QTextEdit* MainWindow::makeConsole() {
    auto* t = new QTextEdit;
    t->setReadOnly(true);
    t->setFont(QFont("Cascadia Code,Consolas,monospace", 10));
    t->setStyleSheet(
        "QTextEdit{"
        "  background:#0d1117; color:#c9d1d9;"
        "  border:1px solid #30363d; border-radius:8px; padding:8px;"
        "}"
        "QScrollBar:vertical{background:#161b22; width:8px; border-radius:4px;}"
        "QScrollBar::handle:vertical{background:#3a3f4b; border-radius:4px;}"
    );
    t->setMinimumHeight(250);
    return t;
}

QString MainWindow::badge(const QString& text, const QString& bg,
                          const QString& fg) {
    return QString(
        "<span style='background:%1;color:%2;"
        "border-radius:4px;padding:1px 6px;font-weight:bold;'>%3</span>"
    ).arg(bg, fg, text);
}

QString MainWindow::htmlArr(const std::vector<int>& v, int highlight,
                            const QString& label) {
    QString s;
    if (!label.isEmpty())
        s += QString("<span style='color:#8b949e;'>%1 </span>").arg(label);
    s += "<span style='color:#58a6ff;'>[</span>";
    for (int i = 0; i < (int)v.size(); ++i) {
        if (i > 0) s += "<span style='color:#8b949e;'>, </span>";
        if (i == highlight)
            s += badge(QString::number(v[i]), "#f0883e", "#0d1117");
        else
            s += QString("<span style='color:#e6edf3;'>%1</span>")
                     .arg(v[i]);
    }
    s += "<span style='color:#58a6ff;'>]</span>";
    return s;
}

QString MainWindow::htmlArrD(const std::vector<double>& v) {
    QString s = "<span style='color:#3fb950;'>[</span>";
    for (int i = 0; i < (int)v.size(); ++i) {
        if (i > 0) s += "<span style='color:#8b949e;'>, </span>";
        double val = v[i];
        // integer-looking doubles shown without decimal
        if (val == std::floor(val))
            s += badge(QString::number((int)val), "#238636", "#e6edf3");
        else
            s += badge(QString::number(val,'f',1), "#1f6feb", "#e6edf3");
    }
    s += "<span style='color:#3fb950;'>]</span>";
    return s;
}

// ─────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Лабораторная работа №4  —  Алгоритмы сортировки и поиска");
    resize(1100, 750);

    applyStyle();

    m_tabs = new QTabWidget;
    m_tabs->setDocumentMode(true);
    m_tabs->setTabPosition(QTabWidget::North);
    m_tabs->setFont(QFont("Segoe UI", 10, QFont::Bold));
    m_tabs->setStyleSheet(
        "QTabWidget::pane{"
        "  border:1px solid #30363d; border-radius:8px;"
        "  background:#161b22;"
        "}"
        "QTabBar::tab{"
        "  background:#0d1117; color:#8b949e;"
        "  padding:8px 20px; border:1px solid #30363d;"
        "  border-bottom:none; border-radius:6px 6px 0 0; margin-right:2px;"
        "}"
        "QTabBar::tab:selected{"
        "  background:#161b22; color:#58a6ff; border-bottom:2px solid #58a6ff;"
        "}"
        "QTabBar::tab:hover{color:#e6edf3;}"
    );

    m_tabs->addTab(buildTask1(), "⚡  Задание 1");
    m_tabs->addTab(buildTask2(), "🔢  Задание 2");
    m_tabs->addTab(buildTask3(), "📊  Задание 3");
    m_tabs->addTab(buildTask4(), "📦  Задание 4");
    m_tabs->addTab(buildTask5(), "🔗  Задание 5");

    setCentralWidget(m_tabs);
    statusBar()->setStyleSheet("color:#8b949e; background:#0d1117; border-top:1px solid #30363d;");
    statusBar()->showMessage("Готово к работе  •  Qt 6.4  •  C++20");
}

// ─────────────────────────────────────────────────────────
//  Global dark theme
// ─────────────────────────────────────────────────────────

void MainWindow::applyStyle() {
    qApp->setStyle("Fusion");
    QPalette p;
    p.setColor(QPalette::Window,          QColor("#0d1117"));
    p.setColor(QPalette::WindowText,      QColor("#c9d1d9"));
    p.setColor(QPalette::Base,            QColor("#161b22"));
    p.setColor(QPalette::AlternateBase,   QColor("#21262d"));
    p.setColor(QPalette::ToolTipBase,     QColor("#161b22"));
    p.setColor(QPalette::ToolTipText,     QColor("#c9d1d9"));
    p.setColor(QPalette::Text,            QColor("#c9d1d9"));
    p.setColor(QPalette::Button,          QColor("#21262d"));
    p.setColor(QPalette::ButtonText,      QColor("#c9d1d9"));
    p.setColor(QPalette::BrightText,      Qt::red);
    p.setColor(QPalette::Link,            QColor("#58a6ff"));
    p.setColor(QPalette::Highlight,       QColor("#1f6feb"));
    p.setColor(QPalette::HighlightedText, QColor("#e6edf3"));
    qApp->setPalette(p);

    qApp->setStyleSheet(
        "QLineEdit{"
        "  background:#21262d; color:#c9d1d9;"
        "  border:1px solid #30363d; border-radius:6px; padding:4px 8px;"
        "}"
        "QLineEdit:focus{border-color:#58a6ff;}"
        "QSpinBox{"
        "  background:#21262d; color:#c9d1d9;"
        "  border:1px solid #30363d; border-radius:6px; padding:4px 8px;"
        "}"
        "QLabel{color:#c9d1d9;}"
        "QSplitter::handle{background:#30363d;}"
        "QTableWidget{"
        "  background:#161b22; color:#c9d1d9;"
        "  gridline-color:#30363d; border:1px solid #30363d; border-radius:8px;"
        "}"
        "QHeaderView::section{"
        "  background:#21262d; color:#58a6ff;"
        "  border:1px solid #30363d; padding:4px 8px; font-weight:bold;"
        "}"
        "QTableWidget::item:selected{"
        "  background:#1f6feb; color:#e6edf3;"
        "}"
        "QScrollBar:vertical{background:#161b22;width:8px;border-radius:4px;}"
        "QScrollBar::handle:vertical{background:#3a3f4b;border-radius:4px;}"
        "QScrollBar:horizontal{background:#161b22;height:8px;border-radius:4px;}"
        "QScrollBar::handle:horizontal{background:#3a3f4b;border-radius:4px;}"
    );
}

// ─────────────────────────────────────────────────────────
//  Tab 1  — Сортировки + бинарный поиск
// ─────────────────────────────────────────────────────────

QWidget* MainWindow::buildTask1() {
    auto* w = new QWidget;
    auto* main = new QVBoxLayout(w);
    main->setContentsMargins(12,12,12,12);
    main->setSpacing(10);

    // ── Input panel ──
    auto* grpIn = makeGroup("Входные данные");
    auto* gl = new QGridLayout(grpIn);
    gl->setSpacing(8);

    gl->addWidget(new QLabel("Массив (через запятую):"), 0, 0);
    t1_input = new QLineEdit("3,1,4,1,5,9,2,6,5,3,5");
    gl->addWidget(t1_input, 0, 1);

    gl->addWidget(new QLabel("Размер случайного:"), 0, 2);
    t1_size = new QSpinBox; t1_size->setRange(5,200); t1_size->setValue(15);
    gl->addWidget(t1_size, 0, 3);

    auto* btnGen  = makeBtn("🎲 Случайный", "#1f6feb");
    auto* btnSort = makeBtn("▶ Сортировать", "#238636");
    gl->addWidget(btnGen,  0, 4);
    gl->addWidget(btnSort, 0, 5);

    main->addWidget(grpIn);

    // ── Search panel ──
    auto* grpS = makeGroup("Бинарный поиск (после сортировки)");
    auto* hl = new QHBoxLayout(grpS);
    hl->addWidget(new QLabel("Искать число:"));
    t1_search = new QLineEdit("5");
    t1_search->setMaximumWidth(100);
    hl->addWidget(t1_search);
    auto* btnSrch = makeBtn("🔍 Найти", "#6e40c9");
    hl->addWidget(btnSrch);
    hl->addStretch();
    main->addWidget(grpS);

    // ── Visualization ──
    t1_viz = new SortVisualizer;
    t1_viz->setMinimumHeight(250);
    main->addWidget(t1_viz);

    // ── Output ──
    t1_out = makeConsole();
    main->addWidget(t1_out, 1);

    connect(btnGen,  &QPushButton::clicked, this, &MainWindow::onTask1Generate);
    connect(btnSort, &QPushButton::clicked, this, &MainWindow::onTask1Sort);
    connect(btnSrch, &QPushButton::clicked, this, &MainWindow::onTask1Search);
    return w;
}

void MainWindow::onTask1Generate() {
    int n = t1_size->value();
    QStringList lst;
    for (int i = 0; i < n; ++i)
        lst << QString::number(QRandomGenerator::global()->bounded(1, 100));
    t1_input->setText(lst.join(","));

    // Update visualization with random data
    std::vector<int> randData;
    for (int i = 0; i < n; ++i)
        randData.push_back(lst[i].toInt());
    t1_viz->setData(randData);
}

void MainWindow::onTask1Sort() {
    QString raw = t1_input->text().trimmed();
    QStringList parts = raw.split(',', Qt::SkipEmptyParts);
    if (parts.isEmpty()) {
        t1_out->setHtml("<p style='color:#f85149;'>Введите массив!</p>");
        return;
    }
    std::vector<int> orig;
    for (auto& s : parts) {
        bool ok; int v = s.trimmed().toInt(&ok);
        if (!ok) { t1_out->setHtml("<p style='color:#f85149;'>Ошибка: не число!</p>"); return; }
        orig.push_back(v);
    }
    int n = (int)orig.size();

    delete[] t1_sorted;
    t1_sorted = new int[n];
    t1_n = n;

    QString html;
    html += "<p style='color:#58a6ff; font-size:14px; font-weight:bold;'>📋 Исходный массив:</p>";
    html += "<p style='margin:4px 0;'>" + htmlArr(orig) + "</p><br>";

    auto runSort = [&](const QString& name, const QString& color,
                        std::function<void(int*,int)> fn) {
        int* arr = new int[n];
        std::copy(orig.begin(), orig.end(), arr);
        QElapsedTimer t; t.start();
        fn(arr, n);
        qint64 ns = t.nsecsElapsed();

        html += QString("<p style='color:%1; font-weight:bold;'>🔷 %2</p>").arg(color, name);
        html += "<p style='margin:2px 0;'>";
        std::vector<int> sv(arr, arr+n);
        html += htmlArr(sv) + "</p>";
        double ms = ns / 1e6;
        QString timeStr;
        if (ns < 1000)
            timeStr = QString("%1 нс").arg(ns);
        else if (ns < 1000000)
            timeStr = QString("%1 мкс").arg(ns/1000.0, 0, 'f', 2);
        else
            timeStr = QString("%1 мс").arg(ms, 0, 'f', 3);

        html += QString("<p style='margin:2px 0;'>⏱ <span style='color:#e3b341;'>%1</span></p><br>").arg(timeStr);

        // Save last sort result for binary search
        std::copy(arr, arr+n, t1_sorted);
        delete[] arr;
    };

    runSort("Heap Sort",  "#ff7b72", heapSort);
    runSort("Quick Sort", "#3fb950", quickSort);
    runSort("Merge Sort", "#d2a8ff", mergeSort);

    // Update visualization with sorted data (show Quick Sort result)
    std::vector<int> sortedViz(t1_sorted, t1_sorted + t1_n);
    t1_viz->setData(sortedViz);

    t1_out->setHtml(html);
    t1_out->verticalScrollBar()->setValue(0);
}

void MainWindow::onTask1Search() {
    if (!t1_sorted || t1_n == 0) {
        t1_out->append("<p style='color:#f85149;'>Сначала выполните сортировку!</p>");
        return;
    }
    bool ok;
    int digit = t1_search->text().trimmed().toInt(&ok);
    if (!ok) {
        t1_out->append("<p style='color:#f85149;'>Введите корректное число!</p>");
        return;
    }
    int idx = binsearch(t1_sorted, t1_n, digit);
    QString html = t1_out->toHtml();

    QString result;
    if (idx == -1)
        result = QString("<p>🔍 Поиск <b style='color:#f0883e;'>%1</b>  →  "
                         "<span style='color:#f85149; font-weight:bold;'>-1 (не найдено)</span></p>")
                     .arg(digit);
    else {
        std::vector<int> sv(t1_sorted, t1_sorted + t1_n);
        result = QString("<p>🔍 Поиск <b style='color:#f0883e;'>%1</b>  →  "
                         "<span style='color:#3fb950; font-weight:bold;'>индекс %2</span></p>")
                     .arg(digit).arg(idx);
        result += "<p style='margin:2px 0;'>Массив: " + htmlArr(sv, idx) + "</p>";

        // Highlight found element in visualization
        t1_viz->setData(sv);
        t1_viz->setHighlight(idx);
    }
    t1_out->append(result);
    t1_out->verticalScrollBar()->setValue(t1_out->verticalScrollBar()->maximum());
}

// ─────────────────────────────────────────────────────────
//  Tab 2  — Интерполяционная сортировка + binpow
// ─────────────────────────────────────────────────────────

QWidget* MainWindow::buildTask2() {
    auto* w = new QWidget;
    auto* main = new QVBoxLayout(w);
    main->setContentsMargins(12,12,12,12);
    main->setSpacing(10);

    auto* grpIn = makeGroup("Входные данные");
    auto* gl = new QGridLayout(grpIn);
    gl->setSpacing(8);

    gl->addWidget(new QLabel("Массив:"), 0, 0);
    t2_input = new QLineEdit("8,3,5,1,9,2,7,4,6");
    gl->addWidget(t2_input, 0, 1, 1, 3);

    gl->addWidget(new QLabel("Искать:"), 1, 0);
    t2_search = new QLineEdit("7");
    t2_search->setMaximumWidth(80);
    gl->addWidget(t2_search, 1, 1);

    gl->addWidget(new QLabel("Modulo:"), 1, 2);
    t2_mod = new QLineEdit("1000000007");
    gl->addWidget(t2_mod, 1, 3);

    auto* btnSort2 = makeBtn("▶ Сортировать по шагам", "#238636");
    gl->addWidget(btnSort2, 0, 4);

    main->addWidget(grpIn);

    // Binpow section
    auto* grpPow = makeGroup("Быстрое возведение в степень: binpow(base, exp, mod)");
    auto* hl = new QHBoxLayout(grpPow);
    hl->addWidget(new QLabel("base:"));
    t2_base = new QLineEdit("2"); t2_base->setMaximumWidth(80);
    hl->addWidget(t2_base);
    hl->addWidget(new QLabel("exp:"));
    t2_exp = new QLineEdit("10"); t2_exp->setMaximumWidth(80);
    hl->addWidget(t2_exp);
    hl->addWidget(new QLabel("mod:"));
    t2_pmod = new QLineEdit("1000000007"); t2_pmod->setMaximumWidth(120);
    hl->addWidget(t2_pmod);
    auto* btnPow = makeBtn("= Вычислить", "#6e40c9");
    hl->addWidget(btnPow);
    hl->addStretch();
    main->addWidget(grpPow);

    t2_out = makeConsole();
    main->addWidget(t2_out, 1);

    connect(btnSort2, &QPushButton::clicked, this, &MainWindow::onTask2Sort);
    connect(btnPow,   &QPushButton::clicked, this, &MainWindow::onTask2Binpow);
    return w;
}

void MainWindow::onTask2Sort() {
    QString raw = t2_input->text().trimmed();
    QStringList parts = raw.split(',', Qt::SkipEmptyParts);
    if (parts.isEmpty()) return;

    std::vector<int> orig;
    for (auto& s : parts) {
        bool ok; int v = s.trimmed().toInt(&ok);
        if (!ok) { t2_out->setHtml("<p style='color:#f85149;'>Ошибка!</p>"); return; }
        orig.push_back(v);
    }
    int n = (int)orig.size();
    int* arr = new int[n];
    std::copy(orig.begin(), orig.end(), arr);

    std::vector<std::vector<int>> steps;
    interpolationSort(arr, n, steps);

    QString html;
    html += "<p style='color:#58a6ff; font-size:14px; font-weight:bold;'>"
            "🔢 Интерполяционная сортировка — пошаговый вывод</p>";
    html += "<p style='color:#8b949e; font-size:11px;'>"
            "На каждом шаге элемент вставляется в отсортированный префикс "
            "по позиции, найденной интерполяцией.</p><br>";

    for (int i = 0; i < (int)steps.size(); ++i) {
        QString stepLabel = (i == 0)
            ? badge("Исходный", "#21262d", "#8b949e")
            : badge(QString("Шаг %1").arg(i), "#1f6feb", "#e6edf3");
        html += "<p style='margin:3px 0;'>" + stepLabel + " &nbsp; ";
        html += htmlArr(steps[i], (i > 0) ? i-1 : -1);
        html += "</p>";
    }

    // Binary search result
    bool ok; int digit = t2_search->text().trimmed().toInt(&ok);
    if (ok) {
        bool ok2; long long mod = t2_mod->text().trimmed().toLongLong(&ok2);
        if (!ok2 || mod <= 0) mod = 1000000007LL;

        int idx = binsearch(arr, n, digit);
        html += "<br><hr style='border-color:#30363d;'>";
        html += QString("<p style='color:#e6edf3; font-weight:bold;'>Бинарный поиск: %1</p>")
                    .arg(badge(QString::number(digit), "#f0883e","#0d1117"));

        if (idx == -1) {
            html += "<p><span style='color:#f85149;'>Не найдено → -1</span></p>";
        } else {
            long long pw = binpow((long long)idx, (long long)n, mod);
            html += QString("<p>Индекс: %1</p>")
                        .arg(badge(QString::number(idx),"#238636","#e6edf3"));
            html += QString("<p>binpow(%1, n=%2, mod=%3) = %4</p>")
                        .arg(idx).arg(n).arg(mod)
                        .arg(badge(QString::number(pw), "#6e40c9", "#e6edf3"));
        }
    }

    delete[] arr;
    t2_out->setHtml(html);
    t2_out->verticalScrollBar()->setValue(0);
}

void MainWindow::onTask2Binpow() {
    bool ok1, ok2, ok3;
    long long base = t2_base->text().trimmed().toLongLong(&ok1);
    long long exp  = t2_exp->text().trimmed().toLongLong(&ok2);
    long long mod  = t2_pmod->text().trimmed().toLongLong(&ok3);
    if (!ok1||!ok2||!ok3||mod<=0) {
        t2_out->append("<p style='color:#f85149;'>Некорректные параметры!</p>");
        return;
    }
    long long res = binpow(base, exp, mod);
    QString html = t2_out->toHtml();
    t2_out->append(
        QString("<p>⚡ binpow(%1, %2, %3) = %4</p>")
            .arg(base).arg(exp).arg(mod)
            .arg(badge(QString::number(res), "#6e40c9", "#e6edf3"))
    );
    t2_out->verticalScrollBar()->setValue(t2_out->verticalScrollBar()->maximum());
}

// ─────────────────────────────────────────────────────────
//  Tab 3  — Медианы троек
// ─────────────────────────────────────────────────────────

QWidget* MainWindow::buildTask3() {
    auto* w = new QWidget;
    auto* main = new QVBoxLayout(w);
    main->setContentsMargins(12,12,12,12);
    main->setSpacing(10);

    auto* grpIn = makeGroup("Входной вектор a");
    auto* hl = new QHBoxLayout(grpIn);
    hl->addWidget(new QLabel("a:"));
    t3_input = new QLineEdit("1,5,1,4,5,6,2,1,3,4,4,4,5,7");
    hl->addWidget(t3_input);
    auto* btn = makeBtn("📊 Вычислить медианы", "#b08800");
    hl->addWidget(btn);
    main->addWidget(grpIn);

    t3_out = makeConsole();
    main->addWidget(t3_out, 1);

    connect(btn, &QPushButton::clicked, this, &MainWindow::onTask3Run);
    return w;
}

void MainWindow::onTask3Run() {
    QStringList parts = t3_input->text().trimmed().split(',', Qt::SkipEmptyParts);
    if (parts.isEmpty()) return;
    std::vector<int> a;
    for (auto& s : parts) {
        bool ok; int v = s.trimmed().toInt(&ok);
        if (!ok) { t3_out->setHtml("<p style='color:#f85149;'>Ошибка!</p>"); return; }
        a.push_back(v);
    }

    std::vector<double> b = medianTriplets(a);

    QString html;
    html += "<p style='color:#58a6ff; font-size:14px; font-weight:bold;'>📊 Медианы подряд идущих троек</p><br>";
    html += "<p><b style='color:#e6edf3;'>Вектор a:</b> " + htmlArr(a) + "</p><br>";

    // Show triplets
    html += "<p style='color:#8b949e;'>Разбивка на тройки:</p>";
    int n = (int)a.size();
    for (int i = 0, ti = 0; i < n; i += 3, ++ti) {
        int rem = std::min(3, n-i);
        std::vector<int> grp(a.begin()+i, a.begin()+i+rem);
        QString type;
        double med;
        if (rem == 3) {
            int tmp[3] = {grp[0],grp[1],grp[2]};
            std::sort(tmp,tmp+3);
            med = tmp[1];
            type = badge("тройка", "#21262d", "#8b949e");
        } else if (rem == 2) {
            med = (grp[0]+grp[1]) / 2.0;
            type = badge("пара → среднее", "#21262d", "#e3b341");
        } else {
            med = grp[0];
            type = badge("один элемент", "#21262d", "#8b949e");
        }
        html += "<p style='margin:2px 0;'>"
                + badge(QString("Тройка %1").arg(ti+1), "#1f6feb", "#e6edf3")
                + " " + type + "  "
                + htmlArr(grp)
                + "  →  медиана: "
                + badge(med == std::floor(med)
                        ? QString::number((int)med)
                        : QString::number(med,'f',1),
                        "#238636", "#e6edf3")
                + "</p>";
    }

    html += "<br><p><b style='color:#e6edf3;'>Вектор b:</b> " + htmlArrD(b) + "</p>";
    t3_out->setHtml(html);
    t3_out->verticalScrollBar()->setValue(0);
}

// ─────────────────────────────────────────────────────────
//  Tab 4  — Демо Vector
// ─────────────────────────────────────────────────────────

QWidget* MainWindow::buildTask4() {
    auto* w = new QWidget;
    auto* main = new QVBoxLayout(w);
    main->setContentsMargins(12,12,12,12);
    main->setSpacing(10);

    auto* grpTop = makeGroup("Демонстрация шаблонного класса Vector<T>");
    auto* hl = new QHBoxLayout(grpTop);
    auto* btn = makeBtn("▶ Запустить демонстрацию всех методов", "#1f6feb");
    hl->addWidget(btn);
    hl->addStretch();
    main->addWidget(grpTop);

    // Visualization for Vector operations
    t4_viz = new SortVisualizer;
    t4_viz->setMinimumHeight(200);
    main->addWidget(t4_viz);

    t4_out = makeConsole();
    main->addWidget(t4_out, 1);

    connect(btn, &QPushButton::clicked, this, &MainWindow::onTask4Demo);
    return w;
}

void MainWindow::onTask4Demo() {
    QString html;
    auto section = [&](const QString& name) {
        html += QString("<p style='color:#58a6ff; font-weight:bold; margin-top:10px;'>── %1</p>").arg(name);
    };
    auto show = [&](const QString& op, const QString& result) {
        html += QString("<p style='margin:1px 0;'><span style='color:#8b949e;'>%1</span>  →  %2</p>").arg(op, result);
    };
    auto arrStr = [&](const Vector<int>& v) -> QString {
        std::vector<int> sv;
        for (std::size_t i = 0; i < v.size(); ++i) sv.push_back(v[i]);
        return htmlArr(sv);
    };
    auto updateViz = [&](const Vector<int>& vec, int highlight = -1) {
        std::vector<int> sv;
        for (std::size_t i = 0; i < vec.size(); ++i) sv.push_back(vec[i]);
        t4_viz->setData(sv);
        if (highlight >= 0 && highlight < (int)sv.size())
            t4_viz->setHighlight(highlight);
        qApp->processEvents();
    };

    html += "<p style='color:#3fb950; font-size:14px; font-weight:bold;'>📦 Vector&lt;T&gt; — демонстрация методов</p><br>";

    // Constructor + push_back
    section("Конструктор по умолчанию + push_back");
    Vector<int> v;
    for (int i = 1; i <= 6; ++i) v.push_back(i * 10);
    show("push_back(10..60)", arrStr(v));
    show("size()", badge(QString::number(v.size()), "#238636","#e6edf3"));
    show("capacity()", badge(QString::number(v.capacity()), "#1f6feb","#e6edf3"));
    updateViz(v);

    // Initializer list constructor
    section("Конструктор из initializer_list");
    Vector<int> v2 = {5, 3, 8, 1, 9, 2};
    show("Vector{5,3,8,1,9,2}", arrStr(v2));
    updateViz(v2);

    // at / front / back
    section("at, front, back");
    show("at(2)",  badge(QString::number(v.at(2)), "#f0883e","#0d1117"));
    show("front()",badge(QString::number(v.front()), "#f0883e","#0d1117"));
    show("back()", badge(QString::number(v.back()), "#f0883e","#0d1117"));
    updateViz(v, 2);  // highlight element at index 2

    // insert / emplace
    section("insert / emplace / emplace_back");
    v.insert(v.begin() + 2, 99);
    show("insert(pos=2, 99)", arrStr(v));
    updateViz(v, 2);  // highlight inserted element
    v.emplace(v.begin() + 4, 77);
    show("emplace(pos=4, 77)", arrStr(v));
    updateViz(v, 4);  // highlight emplaced element
    v.emplace_back(88);
    show("emplace_back(88)", arrStr(v));
    updateViz(v, v.size()-1);  // highlight last element

    // erase
    section("erase");
    v.erase(v.begin() + 3);
    show("erase(pos=3)", arrStr(v));
    updateViz(v);
    v.erase(v.begin()+1, v.begin()+3);
    show("erase(1..3)", arrStr(v));
    updateViz(v);

    // pop_back
    section("pop_back");
    v.pop_back();
    show("pop_back()", arrStr(v));
    updateViz(v, v.size()-1);  // highlight new last element

    // assign
    section("assign");
    v.assign(5, 42);
    show("assign(5, 42)", arrStr(v));
    updateViz(v);
    v.assign({7,14,21,28});
    show("assign({7,14,21,28})", arrStr(v));
    updateViz(v);

    // resize / reserve
    section("resize / reserve");
    v.resize(7, 0);
    show("resize(7, 0)", arrStr(v));
    updateViz(v);
    std::size_t old_cap = v.capacity();
    v.reserve(50);
    show(QString("reserve(50)  cap: %1 → %2").arg(old_cap).arg(v.capacity()),
         badge("OK", "#238636","#e6edf3"));

    // iterators
    section("Итераторы: begin/end/cbegin/rbegin");
    {
        Vector<int> vi = {1,2,3,4,5};
        QString fwd, rev;
        for (auto it = vi.begin(); it != vi.end(); ++it)
            fwd += QString::number(*it) + " ";
        for (auto it = vi.rbegin(); it != vi.rend(); ++it)
            rev += QString::number(*it) + " ";
        show("forward:  " + fwd.trimmed(), badge("✓","#238636","#e6edf3"));
        show("reverse:  " + rev.trimmed(), badge("✓","#238636","#e6edf3"));
        updateViz(vi);
    }

    // swap
    section("swap");
    Vector<int> va = {1,2,3}, vb = {4,5,6};
    show("До swap: va=" + arrStr(va) + "  vb=" + arrStr(vb), "");
    updateViz(va);
    va.swap(vb);
    show("После swap: va=" + arrStr(va) + "  vb=" + arrStr(vb), "");
    updateViz(va);  // show swapped va

    // copy / move
    section("Копирование / перемещение");
    Vector<int> vc(va);
    show("copy ctor: vc=" + arrStr(vc), badge("deep copy","#6e40c9","#e6edf3"));
    updateViz(vc);
    Vector<int> vd(std::move(vc));
    show("move ctor: vd=" + arrStr(vd) + "  vc.size()=" + QString::number(vc.size()),
         badge("moved","#e3b341","#0d1117"));
    updateViz(vd);

    // empty / clear
    section("empty / clear");
    show("vd.empty()", badge(vd.empty()?"true":"false","#f85149","#e6edf3"));
    vd.clear();
    show("vd.clear() → empty()", badge(vd.empty()?"true":"false","#238636","#e6edf3"));
    updateViz(vd);  // show empty vector

    // data / max_size
    section("data / max_size");
    Vector<int> vx = {10,20,30};
    show("data()[1]", badge(QString::number(vx.data()[1]),"#1f6feb","#e6edf3"));
    show("max_size()", badge(QString::number(vx.max_size()),"#8b949e","#e6edf3"));
    updateViz(vx, 1);  // highlight element at index 1 (data()[1])

    t4_out->setHtml(html);
    t4_out->verticalScrollBar()->setValue(0);
}

// ─────────────────────────────────────────────────────────
//  Tab 5  — Pair + Матрицы
// ─────────────────────────────────────────────────────────

QWidget* MainWindow::buildTask5() {
    auto* w = new QWidget;
    auto* main = new QVBoxLayout(w);
    main->setContentsMargins(12,12,12,12);
    main->setSpacing(10);

    auto* grpTop = makeGroup("Pair<T1,T2>  +  матричное представление");
    auto* hl = new QHBoxLayout(grpTop);
    auto* btn = makeBtn("▶ Создать и показать", "#1f6feb");
    hl->addWidget(btn);
    hl->addStretch();
    main->addWidget(grpTop);

    // Tables
    auto* grpTbl = makeGroup("Матрицы из Vector<pair<T1,T2>>");
    auto* gl = new QGridLayout(grpTbl);
    gl->addWidget(new QLabel("<b style='color:#58a6ff;'>Матрица 1: first (Vector&lt;int&gt;)</b>"), 0, 0);
    gl->addWidget(new QLabel("<b style='color:#3fb950;'>Матрица 2: second (Vector&lt;Pair&lt;int,double&gt;&gt;)</b>"), 0, 1);
    t5_tbl1 = new QTableWidget;
    t5_tbl2 = new QTableWidget;
    t5_tbl1->setMinimumHeight(160);
    t5_tbl2->setMinimumHeight(160);
    t5_tbl1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    t5_tbl2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    gl->addWidget(t5_tbl1, 1, 0);
    gl->addWidget(t5_tbl2, 1, 1);
    main->addWidget(grpTbl);

    t5_out = makeConsole();
    t5_out->setMinimumHeight(180);
    main->addWidget(t5_out, 1);

    connect(btn, &QPushButton::clicked, this, &MainWindow::onTask5Demo);
    return w;
}

void MainWindow::onTask5Demo() {
    // ── Build the data structure from Task 5 ──
    // Vector where each element is Pair< Vector<int>, Vector<Pair<int,double>> >

    using InnerPair = Pair<int, double>;
    using VecInt    = Vector<int>;
    using VecPairID = Vector<InnerPair>;
    using Row       = Pair<VecInt, VecPairID>;

    Vector<Row> matrix;

    // Fill 4 rows
    const int ROWS = 4;
    const int COLS = 5;
    for (int r = 0; r < ROWS; ++r) {
        VecInt   col1;
        VecPairID col2;
        for (int c = 0; c < COLS; ++c) {
            col1.push_back((r+1)*(c+1));
            col2.emplace_back(InnerPair{(r+1)*(c+1), (r+1)*0.1*(c+1)});
        }
        matrix.emplace_back(Row{col1, col2});
    }

    // Nested pair demo: Pair<Pair<T,T>, Pair<T,T>>
    Pair<Pair<int,int>, Pair<double,double>> nested{
        Pair<int,int>{42, 73},
        Pair<double,double>{3.14, 2.71}
    };

    // ── Fill table 1 ──
    t5_tbl1->setRowCount(ROWS);
    t5_tbl1->setColumnCount(COLS);
    QStringList hdr1, hdr2;
    for (int c = 0; c < COLS; ++c) { hdr1 << QString("col %1").arg(c); }
    t5_tbl1->setHorizontalHeaderLabels(hdr1);
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            auto* item = new QTableWidgetItem(QString::number(matrix[r].first[c]));
            item->setTextAlignment(Qt::AlignCenter);
            item->setBackground(QColor(r%2==0 ? "#161b22" : "#21262d"));
            item->setForeground(QColor("#58a6ff"));
            t5_tbl1->setItem(r, c, item);
        }
    }

    // ── Fill table 2 ──
    t5_tbl2->setRowCount(ROWS);
    t5_tbl2->setColumnCount(COLS);
    t5_tbl2->setHorizontalHeaderLabels(hdr1);
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            auto& p = matrix[r].second[c];
            auto* item = new QTableWidgetItem(
                QString("(%1, %2)").arg(p.first).arg(QString::number(p.second,'f',2)));
            item->setTextAlignment(Qt::AlignCenter);
            item->setBackground(QColor(r%2==0 ? "#161b22" : "#21262d"));
            item->setForeground(QColor("#3fb950"));
            t5_tbl2->setItem(r, c, item);
        }
    }

    // ── Demo output ──
    QString html;
    html += "<p style='color:#58a6ff; font-size:14px; font-weight:bold;'>🔗 Pair&lt;T1,T2&gt; — демонстрация</p><br>";

    // Pair basics
    Pair<int,double> p1{10, 3.14};
    Pair<int,double> p2{20, 2.71};
    html += "<p><b>p1:</b> (" + badge(QString::number(p1.first),"#1f6feb","#e6edf3") +
            ", " + badge(QString::number(p1.second,'f',2),"#1f6feb","#e6edf3") + ")</p>";
    html += "<p><b>p2:</b> (" + badge(QString::number(p2.first),"#238636","#e6edf3") +
            ", " + badge(QString::number(p2.second,'f',2),"#238636","#e6edf3") + ")</p>";
    html += "<p>p1 &lt; p2: " + badge(p1<p2?"true":"false","#6e40c9","#e6edf3") + "</p>";
    html += "<p>p1 == p2: " + badge(p1==p2?"true":"false","#f85149","#e6edf3") + "</p>";
    p1.swap(p2);
    html += "<p>После swap(p1,p2):<br>&nbsp;&nbsp;p1=(" +
            QString::number(p1.first) + ", " + QString::number(p1.second,'f',2) + ")  p2=(" +
            QString::number(p2.first) + ", " + QString::number(p2.second,'f',2) + ")</p>";

    // Nested pair
    html += "<br><p style='color:#e3b341; font-weight:bold;'>Вложенный Pair&lt;Pair&lt;int,int&gt;, Pair&lt;double,double&gt;&gt;:</p>";
    html += "<p>&nbsp;&nbsp;first = (" +
            badge(QString::number(nested.first.first),"#f0883e","#0d1117") + ", " +
            badge(QString::number(nested.first.second),"#f0883e","#0d1117") + ")</p>";
    html += "<p>&nbsp;&nbsp;second = (" +
            badge(QString::number(nested.second.first,'f',2),"#6e40c9","#e6edf3") + ", " +
            badge(QString::number(nested.second.second,'f',2),"#6e40c9","#e6edf3") + ")</p>";

    // Matrix description
    html += "<br><p style='color:#3fb950; font-weight:bold;'>Структура данных:</p>";
    html += "<p style='color:#8b949e; font-size:11px;'>"
            "Vector&lt;Pair&lt;Vector&lt;int&gt;, Vector&lt;Pair&lt;int,double&gt;&gt;&gt;&gt;<br>"
            "Каждая строка — Pair: первая часть = строка матрицы 1, "
            "вторая = строка матрицы 2 (пары int+double)</p>";
    html += "<p>Строк: " + badge(QString::number(matrix.size()),"#238636","#e6edf3") +
            "  Столбцов: " + badge(QString::number(COLS),"#238636","#e6edf3") + "</p>";

    // Demonstrate remaining Vector methods on inner vector
    auto& row0 = matrix[0].first;
    html += "<br><p style='color:#58a6ff; font-weight:bold;'>Дополнительные методы Vector на row[0].first:</p>";
    html += "<p>size=" + badge(QString::number(row0.size()),"#1f6feb","#e6edf3") +
            "  capacity=" + badge(QString::number(row0.capacity()),"#1f6feb","#e6edf3") +
            "  empty=" + badge(row0.empty()?"true":"false","#f85149","#e6edf3") + "</p>";
    row0.push_back(999);
    html += "<p>push_back(999) → size=" + badge(QString::number(row0.size()),"#238636","#e6edf3") + "</p>";
    row0.pop_back();
    html += "<p>pop_back() → back()=" + badge(QString::number(row0.back()),"#3fb950","#0d1117") + "</p>";

    t5_out->setHtml(html);
    t5_out->verticalScrollBar()->setValue(0);
}