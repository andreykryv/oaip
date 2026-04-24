#include "mainwindow.h"
#include <QSplitter>
#include <QDesktopServices>
#include <QUrl>
#include <cmath>
#include <climits>
#include <stdexcept>

// ═══════════════════════════════════════════════════════════════════════
//  КОНСТРУКТОР
// ═══════════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Лабораторная работа №3  ·  Рекурсия  ·  C++ / Qt");
    resize(1100, 760);
    setMinimumSize(900, 600);

    applyAppStyle();

    // ── Центральный виджет ──────────────────────────────
    QWidget *central = new QWidget(this);
    QVBoxLayout *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ── Шапка ──────────────────────────────────────────
    QFrame *header = new QFrame(central);
    header->setFixedHeight(64);
    header->setObjectName("appHeader");
    QHBoxLayout *hLay = new QHBoxLayout(header);
    hLay->setContentsMargins(24, 0, 24, 0);

    QLabel *icon = new QLabel("⟳", header);
    icon->setStyleSheet(QString("font-size:28px; color:%1; margin-right:8px;")
                        .arg(Theme::ACCENT));

    QLabel *title = new QLabel("Рекурсия", header);
    title->setStyleSheet(QString(
        "font-size:22px; font-weight:700; color:%1; font-family:'Segoe UI',sans-serif;")
        .arg(Theme::TEXT_PRI));

    QLabel *sub = new QLabel("Лабораторная работа №3", header);
    sub->setStyleSheet(QString(
        "font-size:13px; color:%1; margin-left:12px; margin-top:6px;")
        .arg(Theme::TEXT_SEC));

    QLabel *ver = new QLabel("Qt + C++17", header);
    ver->setStyleSheet(QString(
        "font-size:11px; color:%1; background:%2;"
        "border:1px solid %3; border-radius:4px; padding:2px 8px;")
        .arg(Theme::ACCENT2).arg(Theme::BG_CARD).arg(Theme::BORDER));

    hLay->addWidget(icon);
    hLay->addWidget(title);
    hLay->addWidget(sub);
    hLay->addStretch();
    hLay->addWidget(ver);

    // ── Вкладки ────────────────────────────────────────
    QTabWidget *tabs = new QTabWidget(central);
    tabs->setObjectName("mainTabs");
    tabs->addTab(createTask1Widget(), "  1  Dec→Bin  ");
    tabs->addTab(createTask2Widget(), "  2  Аккерман  ");
    tabs->addTab(createTask3Widget(), "  3  Реверс числа  ");
    tabs->addTab(createTask4Widget(), "  4  Ханойская башня  ");
    tabs->addTab(createTask5Widget(), "  5  Файловая система  ");

    rootLayout->addWidget(header);
    rootLayout->addWidget(tabs);

    // ── Статус-бар ─────────────────────────────────────
    statusBar()->showMessage(
        "  Выберите задачу, введите данные и нажмите «Запустить»");
    statusBar()->setStyleSheet(
        QString("background:%1; color:%2; border-top:1px solid %3; font-size:12px;")
        .arg(Theme::BG_CARD).arg(Theme::TEXT_DIM).arg(Theme::BORDER));

    setCentralWidget(central);
}

// ═══════════════════════════════════════════════════════════════════════
//  СТИЛИ
// ═══════════════════════════════════════════════════════════════════════
void MainWindow::applyAppStyle()
{
    qApp->setStyleSheet(QString(R"(
        QMainWindow, QWidget {
            background: %1;
            color: %2;
            font-family: 'Segoe UI', 'Ubuntu', sans-serif;
        }
        QFrame#appHeader {
            background: %3;
            border-bottom: 1px solid %4;
        }
        QTabWidget#mainTabs::pane {
            border: none;
            background: %1;
        }
        QTabWidget#mainTabs QTabBar::tab {
            background: %3;
            color: %5;
            border: none;
            border-right: 1px solid %4;
            padding: 12px 18px;
            font-size: 13px;
            font-weight: 600;
            min-width: 120px;
        }
        QTabWidget#mainTabs QTabBar::tab:selected {
            background: %1;
            color: %6;
            border-bottom: 3px solid %7;
        }
        QTabWidget#mainTabs QTabBar::tab:hover:!selected {
            background: %8;
            color: %2;
        }
        QGroupBox {
            background: %3;
            border: 1px solid %4;
            border-radius: 8px;
            margin-top: 16px;
            padding: 14px 16px 16px 16px;
            font-size: 13px;
            font-weight: 700;
            color: %6;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 14px;
            top: -8px;
            padding: 0 6px;
            color: %6;
            background: %3;
        }
        QLineEdit, QSpinBox {
            background: %8;
            color: %2;
            border: 1px solid %4;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 15px;
            font-family: 'Cascadia Code','Consolas','Courier New',monospace;
            selection-background-color: %7;
        }
        QLineEdit:focus, QSpinBox:focus {
            border: 1px solid %7;
        }
        QSpinBox::up-button, QSpinBox::down-button {
            background: %4;
            border-radius: 3px;
            width: 18px;
        }
        QPushButton#runBtn {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 %7, stop:1 %9);
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 10px 28px;
            font-size: 14px;
            font-weight: 700;
            letter-spacing: 0.5px;
        }
        QPushButton#runBtn:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 %10, stop:1 %11);
        }
        QPushButton#runBtn:pressed {
            background: %4;
        }
        QPushButton#browseBtn {
            background: %8;
            color: %6;
            border: 1px solid %7;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#browseBtn:hover {
            background: %3;
            color: %2;
        }
        QTextEdit {
            background: %12;
            color: %2;
            border: 1px solid %4;
            border-radius: 8px;
            font-family: 'Cascadia Code','Consolas','Courier New',monospace;
            font-size: 13px;
            padding: 12px;
            selection-background-color: %7;
        }
        QScrollBar:vertical {
            background: %3;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: %4;
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
        QLabel#taskDesc {
            color: %5;
            font-size: 13px;
            background: transparent;
            line-height: 160%%;
        }
        QLabel#inputLabel {
            color: %6;
            font-size: 13px;
            font-weight: 600;
            background: transparent;
        }
        QTreeWidget {
            background: %12;
            color: %2;
            border: 1px solid %4;
            border-radius: 8px;
            font-family: 'Cascadia Code','Consolas','Courier New',monospace;
            font-size: 12px;
        }
        QTreeWidget::item:hover { background: %8; }
        QTreeWidget::item:selected { background: %7; color: #fff; }
        QStatusBar { font-size: 12px; }
    )").arg(
        Theme::BG_MAIN,       // %1
        Theme::TEXT_PRI,      // %2
        Theme::BG_CARD,       // %3
        Theme::BORDER,        // %4
        Theme::TEXT_SEC,      // %5
        Theme::HIGHLIGHT,     // %6
        Theme::ACCENT,        // %7
        Theme::BG_INPUT,      // %8
        Theme::ACCENT2,       // %9
        "#8b5cf6",            // %10 hover start
        "#0891b2",            // %11 hover end
        Theme::BG_OUTPUT      // %12
    ));
}

// ═══════════════════════════════════════════════════════════════════════
//  UI-ХЕЛПЕРЫ
// ═══════════════════════════════════════════════════════════════════════
QGroupBox* MainWindow::makeCard(const QString &title) {
    auto *gb = new QGroupBox(title);
    return gb;
}

QPushButton* MainWindow::makeRunButton(const QString &text) {
    auto *btn = new QPushButton(text);
    btn->setObjectName("runBtn");
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}

QTextEdit* MainWindow::makeOutput() {
    auto *te = new QTextEdit;
    te->setReadOnly(true);
    te->setMinimumHeight(160);
    return te;
}

void MainWindow::printHeader(QTextEdit *out, const QString &title,
                              const QString &task)
{
    out->clear();
    out->append(QString(
        "<div style='margin-bottom:4px;'>"
        "<span style='color:%1;font-weight:700;font-size:14px;'>%2</span>"
        "&nbsp;&nbsp;"
        "<span style='color:%3;font-size:12px;'>%4</span>"
        "</div>"
        "<div style='color:%5;font-size:11px;margin-bottom:8px;'>"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━</div>")
        .arg(Theme::ACCENT2, title, Theme::TEXT_DIM, task, Theme::BORDER));
}

void MainWindow::printLine(QTextEdit *out, const QString &text,
                            const QString &color)
{
    out->append(QString("<div style='color:%1;'>%2</div>").arg(color, text));
}

void MainWindow::printKV(QTextEdit *out, const QString &key,
                          const QString &val, const QString &valColor)
{
    out->append(QString(
        "<div>"
        "<span style='color:%1;'>%2</span>"
        "<span style='color:%3;font-weight:700;font-size:15px;'>%4</span>"
        "</div>")
        .arg(Theme::TEXT_SEC, key, valColor, val));
}

void MainWindow::printSep(QTextEdit *out) {
    out->append(QString("<div style='color:%1;font-size:11px;'>"
                        "──────────────────────────────────────"
                        "</div>").arg(Theme::BORDER));
}

void MainWindow::scrollBottom(QTextEdit *out) {
    out->verticalScrollBar()->setValue(
        out->verticalScrollBar()->maximum());
}

QString MainWindow::badge(const QString &text, const QString &color) {
    return QString("<span style='background:%1;color:#fff;"
                   "border-radius:4px;padding:1px 6px;"
                   "font-size:11px;font-weight:700;'>%2</span>")
        .arg(color, text);
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 1 — ПЕРЕВОД В ДВОИЧНУЮ СИСТЕМУ
// ═══════════════════════════════════════════════════════════════════════
QString MainWindow::decToBinRec(long long n)
{
    if (n == 0) return "0";
    if (n == 1) return "1";
    return decToBinRec(n / 2) + QString::number(n % 2);
}

QWidget* MainWindow::createTask1Widget()
{
    QWidget *w = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(20, 16, 20, 16);
    lay->setSpacing(16);

    // Описание
    auto *desc = makeCard("Задание 1 · Десятичное → Двоичное");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "Рекурсивно переводит целое десятичное число в двоичную запись.<br>"
        "<span style='color:#f59e0b;'>Принцип:</span> "
        "<code>bin(n) = bin(n÷2) + (n%2)</code> — "
        "делим на 2 до тех пор, пока не достигнем базового случая (0 или 1).");
    info->setObjectName("taskDesc");
    info->setWordWrap(true);
    dLay->addWidget(info);
    lay->addWidget(desc);

    // Ввод
    auto *inputCard = makeCard("Ввод");
    auto *iLay = new QHBoxLayout(inputCard);
    QLabel *lbl = new QLabel("Число:");
    lbl->setObjectName("inputLabel");
    lbl->setFixedWidth(80);
    t1Input = new QLineEdit("66");
    t1Input->setPlaceholderText("Введите целое число ≥ 0");
    auto *btn = makeRunButton();
    connect(btn, &QPushButton::clicked, this, &MainWindow::runTask1);
    iLay->addWidget(lbl);
    iLay->addWidget(t1Input);
    iLay->addWidget(btn);
    lay->addWidget(inputCard);

    // Вывод
    auto *outCard = makeCard("Результат");
    auto *oLay = new QVBoxLayout(outCard);
    t1Out = makeOutput();
    t1Out->setMaximumHeight(280);
    printHeader(t1Out, "Dec → Bin", "Задание 1");
    oLay->addWidget(t1Out);
    lay->addWidget(outCard);

    lay->addStretch();
    return w;
}

void MainWindow::runTask1()
{
    bool ok;
    long long n = t1Input->text().toLongLong(&ok);
    printHeader(t1Out, "Dec → Bin", "Задание 1");

    if (!ok || n < 0) {
        printLine(t1Out,
            "✗ Ошибка: введите целое неотрицательное число.",
            Theme::ERROR_CLR);
        return;
    }

    printKV(t1Out, "Входное число:  ", QString::number(n), Theme::ACCENT2);
    printSep(t1Out);

    // Пошаговое разложение
    printLine(t1Out, "Шаги рекурсии:", Theme::TEXT_SEC);
    long long tmp = n;
    QStringList steps;
    while (tmp > 1) {
        steps << QString("  %1 ÷ 2 = %2  остаток <b style='color:%3;'>%4</b>")
                   .arg(tmp).arg(tmp/2).arg(Theme::WARNING).arg(tmp%2);
        tmp /= 2;
    }
    steps << QString("  базовый случай: <b style='color:%1;'>%2</b>")
               .arg(Theme::SUCCESS).arg(tmp);
    for (auto &s : steps) printLine(t1Out, s, Theme::TEXT_PRI);

    printSep(t1Out);

    QString bin = (n == 0) ? "0" : decToBinRec(n);
    printKV(t1Out, "Результат:      ",
            bin + QString("  <span style='color:%1;font-size:11px;'>"
                          "[%2 бит]</span>").arg(Theme::TEXT_DIM)
                              .arg(bin.length()),
            Theme::SUCCESS);

    // Визуальное представление битов
    QString bits;
    for (QChar c : bin) {
        if (c == '1')
            bits += QString("<span style='color:%1;background:%2;"
                            "border-radius:3px;padding:1px 5px;margin:1px;'>"
                            "1</span> ").arg(Theme::TEXT_PRI).arg(Theme::ACCENT);
        else
            bits += QString("<span style='color:%1;background:%2;"
                            "border-radius:3px;padding:1px 5px;margin:1px;'>"
                            "0</span> ").arg(Theme::TEXT_DIM).arg(Theme::BG_CARD);
    }
    t1Out->append("<div style='margin-top:6px;'>" + bits + "</div>");

    scrollBottom(t1Out);
    statusBar()->showMessage(
        QString("  Задание 1 выполнено: %1₁₀ = %2₂").arg(n).arg(bin));
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 2 — ФУНКЦИЯ АККЕРМАНА
// ═══════════════════════════════════════════════════════════════════════
long long MainWindow::ackermann(int m, int n, int &calls)
{
    ++calls;
    if (calls > 10'000'000)
        throw std::overflow_error("Превышен лимит вызовов (10M)");
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1, calls);
    return ackermann(m - 1, ackermann(m, n - 1, calls), calls);
}

QWidget* MainWindow::createTask2Widget()
{
    QWidget *w = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(20, 16, 20, 16);
    lay->setSpacing(16);

    auto *desc = makeCard("Задание 2 · Функция Аккермана");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "Функция Аккермана A(m,n) — классический пример <b>взаимной рекурсии</b>:<br>"
        "<code>A(0,n) = n+1 &nbsp;|&nbsp; "
        "A(m,0) = A(m-1,1) &nbsp;|&nbsp; "
        "A(m,n) = A(m-1, A(m, n-1))</code><br>"
        "<span style='color:#ef4444;'>⚠</span> "
        "При m≥4 рост катастрофически быстрый, вычисление ограничено 10M вызовов.");
    info->setObjectName("taskDesc");
    info->setWordWrap(true);
    dLay->addWidget(info);
    lay->addWidget(desc);

    auto *inputCard = makeCard("Ввод");
    auto *iLay = new QHBoxLayout(inputCard);

    QLabel *lm = new QLabel("m:");
    lm->setObjectName("inputLabel");
    lm->setFixedWidth(28);
    t2M = new QSpinBox;
    t2M->setRange(0, 4);
    t2M->setValue(2);
    t2M->setFixedWidth(80);

    QLabel *ln = new QLabel("n:");
    ln->setObjectName("inputLabel");
    ln->setFixedWidth(28);
    t2N = new QSpinBox;
    t2N->setRange(0, 10);
    t2N->setValue(2);
    t2N->setFixedWidth(80);

    auto *btn = makeRunButton();
    connect(btn, &QPushButton::clicked, this, &MainWindow::runTask2);

    iLay->addWidget(lm);
    iLay->addWidget(t2M);
    iLay->addSpacing(16);
    iLay->addWidget(ln);
    iLay->addWidget(t2N);
    iLay->addStretch();
    iLay->addWidget(btn);
    lay->addWidget(inputCard);

    // Таблица малых значений
    auto *tableCard = makeCard("Справочная таблица A(m,n)");
    auto *tLay = new QVBoxLayout(tableCard);
    QTextEdit *table = makeOutput();
    table->setMaximumHeight(120);

    QString hdr = "<table style='border-collapse:collapse;font-size:12px;'>";
    hdr += "<tr><td style='padding:2px 8px;color:" + Theme::TEXT_DIM + ";'>m\\n</td>";
    for (int n2 = 0; n2 <= 5; n2++)
        hdr += QString("<td style='padding:2px 10px;color:%1;font-weight:700;'>%2</td>")
                   .arg(Theme::ACCENT2).arg(n2);
    hdr += "</tr>";
    int dummy;
    const int mmax = 3, nmax = 5;
    for (int m2 = 0; m2 <= mmax; m2++) {
        hdr += QString("<tr><td style='padding:2px 8px;color:%1;font-weight:700;'>%2</td>")
                   .arg(Theme::ACCENT).arg(m2);
        for (int n2 = 0; n2 <= nmax; n2++) {
            dummy = 0;
            try {
                long long v = ackermann(m2, n2, dummy);
                hdr += QString("<td style='padding:2px 10px;color:%1;'>%2</td>")
                           .arg(Theme::SUCCESS).arg(v);
            } catch (...) {
                hdr += QString("<td style='color:%1;'>…</td>")
                           .arg(Theme::TEXT_DIM);
            }
        }
        hdr += "</tr>";
    }
    hdr += "</table>";
    table->setHtml(hdr);
    tLay->addWidget(table);
    lay->addWidget(tableCard);

    auto *outCard = makeCard("Результат");
    auto *oLay = new QVBoxLayout(outCard);
    t2Out = makeOutput();
    t2Out->setMaximumHeight(180);
    printHeader(t2Out, "Функция Аккермана", "Задание 2");
    oLay->addWidget(t2Out);
    lay->addWidget(outCard);

    lay->addStretch();
    return w;
}

void MainWindow::runTask2()
{
    int m = t2M->value(), n = t2N->value();
    printHeader(t2Out, "Функция Аккермана", "Задание 2");
    printKV(t2Out, "Входные данные:  ",
            QString("A(%1, %2)").arg(m).arg(n), Theme::ACCENT2);
    printSep(t2Out);

    int calls = 0;
    try {
        long long result = ackermann(m, n, calls);
        printKV(t2Out, "Результат:       ", QString::number(result), Theme::SUCCESS);
        printKV(t2Out, "Вызовов рекурсии:", QString::number(calls), Theme::WARNING);

        // Предупреждение о взрывном росте
        if (calls > 100000) {
            printLine(t2Out,
                "⚡ Внимание: функция Аккермана растёт быстрее любой "
                "примитивно-рекурсивной функции!",
                Theme::WARNING);
        }
    } catch (const std::overflow_error &e) {
        printLine(t2Out,
            QString("✗ Переполнение: %1").arg(e.what()),
            Theme::ERROR_CLR);
        printLine(t2Out,
            "  Попробуйте меньшие значения m и n.",
            Theme::TEXT_SEC);
    }

    scrollBottom(t2Out);
    statusBar()->showMessage(
        QString("  Задание 2 выполнено: A(%1,%2)").arg(m).arg(n));
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 3 — РЕВЕРС ЧИСЛА
// ═══════════════════════════════════════════════════════════════════════
int MainWindow::countDigits(long long n)
{
    if (n < 10) return 1;
    return 1 + countDigits(n / 10);
}

long long MainWindow::reverseNum(long long n)
{
    if (n < 10) return n;
    int digits = countDigits(n / 10);
    return (n % 10) * (long long)std::pow(10, digits) + reverseNum(n / 10);
}

QWidget* MainWindow::createTask3Widget()
{
    QWidget *w = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(20, 16, 20, 16);
    lay->setSpacing(16);

    auto *desc = makeCard("Задание 3 · Реверс числа рекурсией");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "Разворачивает цифры числа <b>без циклов, строк и массивов</b> — "
        "только рекурсия и целочисленная арифметика.<br>"
        "<span style='color:#f59e0b;'>Идея:</span> "
        "<code>rev(n) = (n%10) × 10^len(n÷10)  +  rev(n÷10)</code>");
    info->setObjectName("taskDesc");
    info->setWordWrap(true);
    dLay->addWidget(info);
    lay->addWidget(desc);

    auto *inputCard = makeCard("Ввод");
    auto *iLay = new QHBoxLayout(inputCard);
    QLabel *lbl = new QLabel("Число:");
    lbl->setObjectName("inputLabel");
    lbl->setFixedWidth(80);
    t3Input = new QLineEdit("972");
    t3Input->setPlaceholderText("Число без нулей в записи");
    auto *btn = makeRunButton();
    connect(btn, &QPushButton::clicked, this, &MainWindow::runTask3);
    iLay->addWidget(lbl);
    iLay->addWidget(t3Input);
    iLay->addWidget(btn);
    lay->addWidget(inputCard);

    auto *outCard = makeCard("Результат");
    auto *oLay = new QVBoxLayout(outCard);
    t3Out = makeOutput();
    t3Out->setMaximumHeight(260);
    printHeader(t3Out, "Реверс числа", "Задание 3");
    oLay->addWidget(t3Out);
    lay->addWidget(outCard);

    lay->addStretch();
    return w;
}

void MainWindow::runTask3()
{
    bool ok;
    long long n = t3Input->text().toLongLong(&ok);
    printHeader(t3Out, "Реверс числа", "Задание 3");

    if (!ok || n <= 0) {
        printLine(t3Out,
            "✗ Ошибка: введите положительное число без нулей.",
            Theme::ERROR_CLR);
        return;
    }
    QString s = QString::number(n);
    if (s.contains('0')) {
        printLine(t3Out,
            "✗ Ошибка: число не должно содержать нулей в записи.",
            Theme::ERROR_CLR);
        return;
    }

    printKV(t3Out, "Входное число:  ", QString::number(n), Theme::ACCENT2);

    // Визуализация цифр
    QString digits;
    for (int i = 0; i < s.length(); i++) {
        digits += QString("<span style='background:%1;color:#fff;"
                          "border-radius:4px;padding:2px 8px;margin:2px;"
                          "font-size:16px;font-weight:700;'>%2</span> ")
                      .arg(Theme::ACCENT).arg(s[i]);
    }
    t3Out->append("<div style='margin:6px 0;'>"
                  "<span style='color:" + Theme::TEXT_SEC + ";'>Цифры:  </span>"
                  + digits + "</div>");

    printSep(t3Out);

    // Шаги
    printLine(t3Out, "Шаги рекурсии:", Theme::TEXT_SEC);
    long long tmp = n;
    int depth = 0;
    while (tmp > 9) {
        QString indent(depth * 2, QChar(' '));
        t3Out->append(QString(
            "<div style='color:%1;font-size:12px;'>"
            "  <span style='color:%2;'>%3rev(%4)</span>"
            " → (%5 % 10) × 10^%6 + rev(%7)"
            "</div>")
            .arg(Theme::TEXT_PRI, Theme::ACCENT)
            .arg(indent)
            .arg(tmp)
            .arg(tmp)
            .arg(countDigits(tmp / 10))
            .arg(tmp / 10));
        tmp /= 10;
        depth++;
    }
    printLine(t3Out,
        QString("  базовый случай: rev(%1) = %1").arg(tmp),
        Theme::SUCCESS);

    printSep(t3Out);
    long long result = reverseNum(n);
    printKV(t3Out, "Результат:      ", QString::number(result), Theme::SUCCESS);

    // Стрелка разворота
    QString sres = QString::number(result);
    QString revDigits;
    for (int i = 0; i < sres.length(); i++) {
        revDigits += QString("<span style='background:%1;color:#fff;"
                             "border-radius:4px;padding:2px 8px;margin:2px;"
                             "font-size:16px;font-weight:700;'>%2</span> ")
                         .arg(Theme::SUCCESS).arg(sres[i]);
    }
    t3Out->append("<div style='margin:6px 0;'>"
                  "<span style='color:" + Theme::TEXT_SEC + ";'>Итог:   </span>"
                  + revDigits + "</div>");

    scrollBottom(t3Out);
    statusBar()->showMessage(
        QString("  Задание 3 выполнено: rev(%1) = %2").arg(n).arg(result));
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 4 — ХАНОЙСКАЯ БАШНЯ
// ═══════════════════════════════════════════════════════════════════════
void MainWindow::hanoi(int n, char from, char to, char via,
                        QStringList &steps, int &stepNo)
{
    if (n == 1) {
        steps << QString("<span style='color:%1;'>Шаг %2:</span>"
                         " Кольцо 1:  <b style='color:%3;'>%4</b>"
                         " → <b style='color:%5;'>%6</b>")
                     .arg(Theme::TEXT_DIM).arg(stepNo++)
                     .arg(Theme::WARNING).arg(from)
                     .arg(Theme::SUCCESS).arg(to);
        return;
    }
    hanoi(n - 1, from, via, to, steps, stepNo);
    steps << QString("<span style='color:%1;'>Шаг %2:</span>"
                     " Кольцо %3:  <b style='color:%4;'>%5</b>"
                     " → <b style='color:%6;'>%7</b>")
                 .arg(Theme::TEXT_DIM).arg(stepNo++)
                 .arg(n)
                 .arg(Theme::WARNING).arg(from)
                 .arg(Theme::SUCCESS).arg(to);
    hanoi(n - 1, via, to, from, steps, stepNo);
}

QWidget* MainWindow::createTask4Widget()
{
    QWidget *w = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(20, 16, 20, 16);
    lay->setSpacing(16);

    auto *desc = makeCard("Задание 4 · Ханойская башня");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "Классическая рекурсивная задача: переместить n колец с колышка <b>A</b> "
        "на колышек <b>C</b>, используя <b>B</b> как вспомогательный.<br>"
        "<span style='color:#f59e0b;'>Минимальное число ходов:</span> "
        "<code>2ⁿ − 1</code>  (для 10 колец = 1023 хода)");
    info->setObjectName("taskDesc");
    info->setWordWrap(true);
    dLay->addWidget(info);
    lay->addWidget(desc);

    auto *inputCard = makeCard("Ввод");
    auto *iLay = new QHBoxLayout(inputCard);
    QLabel *lbl = new QLabel("Колец:");
    lbl->setObjectName("inputLabel");
    lbl->setFixedWidth(80);
    t4Rings = new QSpinBox;
    t4Rings->setRange(1, 12);
    t4Rings->setValue(3);
    t4Rings->setFixedWidth(90);
    auto *btn = makeRunButton();
    connect(btn, &QPushButton::clicked, this, &MainWindow::runTask4);
    iLay->addWidget(lbl);
    iLay->addWidget(t4Rings);
    iLay->addStretch();
    iLay->addWidget(btn);
    lay->addWidget(inputCard);

    auto *outCard = makeCard("Шаги решения");
    auto *oLay = new QVBoxLayout(outCard);
    t4Out = makeOutput();
    printHeader(t4Out, "Ханойская башня", "Задание 4");
    oLay->addWidget(t4Out);
    lay->addWidget(outCard, 1);

    return w;
}

void MainWindow::runTask4()
{
    int n = t4Rings->value();
    printHeader(t4Out, "Ханойская башня", "Задание 4");
    printKV(t4Out, "Колец:  ", QString::number(n), Theme::ACCENT2);
    long long minMoves = (1LL << n) - 1;
    printKV(t4Out, "Мин. ходов:  ",
            QString::number(minMoves) +
            QString("  <span style='color:%1;font-size:11px;'>(2^%2−1)</span>")
                .arg(Theme::TEXT_DIM).arg(n),
            Theme::WARNING);
    printSep(t4Out);

    QStringList steps;
    int stepNo = 1;
    hanoi(n, 'A', 'C', 'B', steps, stepNo);

    // Показываем максимум 100 шагов, остальные свернуть
    const int maxShow = 100;
    int shown = qMin((int)steps.size(), maxShow);
    for (int i = 0; i < shown; i++)
        t4Out->append("<div>" + steps[i] + "</div>");

    if (steps.size() > maxShow) {
        t4Out->append(QString(
            "<div style='color:%1;margin-top:6px;'>"
            "  … и ещё %2 шагов (итого %3)</div>")
            .arg(Theme::TEXT_DIM)
            .arg(steps.size() - maxShow)
            .arg(steps.size()));
    }

    printSep(t4Out);
    printLine(t4Out,
        QString("✓ Решение завершено за <b style='color:%1;'>%2</b> ходов")
            .arg(Theme::SUCCESS).arg(steps.size()),
        Theme::TEXT_PRI);

    // ASCII-схема стержней
    t4Out->append("<div style='margin-top:8px;'>");
    t4Out->append(QString(
        "<span style='color:%1;font-size:12px;'>"
        "  Начало:  [A: 1…%2]  [B: пусто]  [C: пусто]<br>"
        "  Конец:   [A: пусто]  [B: пусто]  [C: 1…%2]"
        "</span>").arg(Theme::TEXT_DIM).arg(n));
    t4Out->append("</div>");

    scrollBottom(t4Out);
    statusBar()->showMessage(
        QString("  Задание 4: %1 колец → %2 ходов").arg(n).arg(steps.size()));
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 5 — ФАЙЛОВАЯ СИСТЕМА
// ═══════════════════════════════════════════════════════════════════════
void MainWindow::countItems(const QString &path, int &files, int &folders)
{
    QDir dir(path);
    if (!dir.exists()) return;
    for (const QFileInfo &fi :
         dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        if (fi.isDir()) {
            folders++;
            countItems(fi.absoluteFilePath(), files, folders); // рекурсия
        } else {
            files++;
        }
    }
}

QTreeWidgetItem* MainWindow::buildTree(const QString &path)
{
    QFileInfo fi(path);
    auto *item = new QTreeWidgetItem;
    item->setText(0, fi.fileName().isEmpty() ? path : fi.fileName());

    if (fi.isDir()) {
        item->setText(1, "папка");
        item->setForeground(0, QColor(Theme::ACCENT2));
        item->setText(0, "📁 " + item->text(0));

        QDir dir(path);
        for (const QFileInfo &child :
             dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries,
                               QDir::DirsFirst)) {
            item->addChild(buildTree(child.absoluteFilePath())); // рекурсия
        }
    } else {
        item->setText(1, QString("%1 KB")
                            .arg(fi.size() / 1024.0, 0, 'f', 1));
        item->setForeground(0, QColor(Theme::TEXT_SEC));

        QString ext = fi.suffix().toLower();
        if (ext == "cpp" || ext == "h" || ext == "py" || ext == "js")
            item->setText(0, "📄 " + fi.fileName());
        else if (ext == "png" || ext == "jpg" || ext == "gif")
            item->setText(0, "🖼 " + fi.fileName());
        else if (ext == "pdf" || ext == "doc" || ext == "docx")
            item->setText(0, "📑 " + fi.fileName());
        else
            item->setText(0, "📄 " + fi.fileName());

        // Открыть файл двойным кликом
        item->setData(0, Qt::UserRole, fi.absoluteFilePath());
    }
    return item;
}

QWidget* MainWindow::createTask5Widget()
{
    QWidget *w = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(20, 16, 20, 16);
    lay->setSpacing(16);

    auto *desc = makeCard("Задание 5 · Рекурсивный обход файловой системы");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "Рекурсивно обходит директорию, подсчитывает подпапки и файлы. "
        "Двойной клик по файлу в дереве открывает его в системном приложении.");
    info->setObjectName("taskDesc");
    info->setWordWrap(true);
    dLay->addWidget(info);
    lay->addWidget(desc);

    auto *inputCard = makeCard("Путь к папке");
    auto *iLay = new QHBoxLayout(inputCard);
    t5Path = new QLineEdit(QDir::homePath());
    auto *browse = new QPushButton("📂  Обзор");
    browse->setObjectName("browseBtn");
    browse->setCursor(Qt::PointingHandCursor);
    browse->setFixedWidth(110);
    auto *run = makeRunButton("▶  Анализировать");
    connect(browse, &QPushButton::clicked, this, &MainWindow::browseFolder);
    connect(run,    &QPushButton::clicked, this, &MainWindow::runTask5);
    iLay->addWidget(t5Path);
    iLay->addWidget(browse);
    iLay->addWidget(run);
    lay->addWidget(inputCard);

    // Разделитель: дерево + лог
    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    // Дерево
    auto *treeCard = makeCard("Дерево каталогов");
    auto *treeLay = new QVBoxLayout(treeCard);
    t5Tree = new QTreeWidget;
    t5Tree->setHeaderLabels({"Имя", "Размер"});
    t5Tree->header()->setStretchLastSection(false);
    t5Tree->header()->resizeSection(0, 280);
    t5Tree->setStyleSheet(
        QString("QHeaderView::section { background:%1; color:%2; "
                "border:none; border-bottom:1px solid %3; "
                "padding:6px 8px; font-weight:700; }")
            .arg(Theme::BG_CARD, Theme::HIGHLIGHT, Theme::BORDER));
    connect(t5Tree, &QTreeWidget::itemDoubleClicked,
        [](QTreeWidgetItem *item, int) {
            QString path = item->data(0, Qt::UserRole).toString();
            if (!path.isEmpty())
                QDesktopServices::openUrl(QUrl::fromLocalFile(path));
        });
    treeLay->addWidget(t5Tree);
    splitter->addWidget(treeCard);

    // Лог
    auto *outCard = makeCard("Статистика");
    auto *oLay = new QVBoxLayout(outCard);
    t5Out = makeOutput();
    t5Out->setMinimumWidth(260);
    printHeader(t5Out, "Файловая система", "Задание 5");
    oLay->addWidget(t5Out);
    splitter->addWidget(outCard);

    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);
    lay->addWidget(splitter, 1);

    return w;
}

void MainWindow::browseFolder()
{
    QString dir = QFileDialog::getExistingDirectory(
        this, "Выберите папку", t5Path->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
        t5Path->setText(dir);
}

void MainWindow::runTask5()
{
    QString path = t5Path->text().trimmed();
    printHeader(t5Out, "Файловая система", "Задание 5");

    QDir dir(path);
    if (!dir.exists()) {
        printLine(t5Out, "✗ Путь не существует.", Theme::ERROR_CLR);
        return;
    }

    printKV(t5Out, "Путь:  ", path, Theme::ACCENT2);
    printSep(t5Out);
    printLine(t5Out, "⏳ Анализирую...", Theme::TEXT_DIM);
    qApp->processEvents();

    int files = 0, folders = 0;
    countItems(path, files, folders);

    // Строим дерево (ограничение глубины для производительности)
    t5Tree->clear();
    auto *root = buildTree(path);
    if (root) {
        t5Tree->addTopLevelItem(root);
        root->setExpanded(true);
    }

    // Вывод статистики
    printLine(t5Out, " ", Theme::TEXT_PRI);
    printKV(t5Out, "📁 Подпапок:  ",
            QString::number(folders), Theme::ACCENT2);
    printKV(t5Out, "📄 Файлов:    ",
            QString::number(files),   Theme::SUCCESS);
    printKV(t5Out, "📊 Итого:     ",
            QString::number(files + folders), Theme::WARNING);
    printSep(t5Out);
    printLine(t5Out,
        "✓ Готово. Двойной клик по файлу — открыть.",
        Theme::TEXT_SEC);

    scrollBottom(t5Out);
    statusBar()->showMessage(
        QString("  Задание 5: %1 папок, %2 файлов в «%3»")
            .arg(folders).arg(files).arg(dir.dirName()));
}
