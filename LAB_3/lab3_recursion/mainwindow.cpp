#include "mainwindow.h"
#include <QSplitter>
#include <QDesktopServices>
#include <QUrl>
#include <cmath>
#include <climits>
#include <stdexcept>
#include <QLinearGradient>

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
        "font-size:22px; font-weight:700; color:%1;").arg(Theme::TEXT_PRI));

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
        }
        QPushButton#runBtn:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #a078f8, stop:1 #0ea5c9);
        }
        QPushButton#runBtn:pressed { background: %4; }
        QPushButton#browseBtn {
            background: %8;
            color: %6;
            border: 1px solid %7;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#browseBtn:hover { background: %3; color: %2; }
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
            background: %3; width: 8px; border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: %4; border-radius: 4px; min-height: 20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
        QLabel#taskDesc  { color: %5; font-size: 13px; background: transparent; }
        QLabel#inputLabel{ color: %6; font-size: 13px; font-weight: 600; background: transparent; }
        QTreeWidget {
            background: %12; color: %2; border: 1px solid %4;
            border-radius: 8px;
            font-family: 'Cascadia Code','Consolas','Courier New',monospace;
            font-size: 12px;
        }
        QTreeWidget::item:hover { background: %8; }
        QTreeWidget::item:selected { background: %7; color: #fff; }
        QStatusBar { font-size: 12px; }
    )").arg(
        Theme::BG_MAIN,    // %1
        Theme::TEXT_PRI,   // %2
        Theme::BG_CARD,    // %3
        Theme::BORDER,     // %4
        Theme::TEXT_SEC,   // %5
        Theme::HIGHLIGHT,  // %6
        Theme::ACCENT,     // %7
        Theme::BG_INPUT,   // %8
        Theme::ACCENT2,    // %9
        "#000000",         // %10
        "#0891b2",         // %11
        Theme::BG_OUTPUT   // %12
    ));
}

// ═══════════════════════════════════════════════════════════════════════
//  UI-ХЕЛПЕРЫ
// ═══════════════════════════════════════════════════════════════════════
QGroupBox* MainWindow::makeCard(const QString &title)
{
    return new QGroupBox(title);
}

QPushButton* MainWindow::makeRunButton(const QString &text)
{
    auto *btn = new QPushButton(text);
    btn->setObjectName("runBtn");
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}

QTextEdit* MainWindow::makeOutput()
{
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
        "<div><span style='color:%1;'>%2</span>"
        "<span style='color:%3;font-weight:700;font-size:15px;'>%4</span></div>")
        .arg(Theme::TEXT_SEC, key, valColor, val));
}

void MainWindow::printSep(QTextEdit *out)
{
    out->append(QString("<div style='color:%1;font-size:11px;'>"
                        "──────────────────────────────────────"
                        "</div>").arg(Theme::BORDER));
}

void MainWindow::scrollBottom(QTextEdit *out)
{
    out->verticalScrollBar()->setValue(out->verticalScrollBar()->maximum());
}

QString MainWindow::badge(const QString &text, const QString &color)
{
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

    auto *desc = makeCard("Задание 1 · Десятичное → Двоичное");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "Рекурсивно переводит целое десятичное число в двоичную запись.<br>"
        "<span style='color:#f59e0b;'>Принцип:</span> "
        "<code>bin(n) = bin(n÷2) + (n%2)</code>");
    info->setObjectName("taskDesc");
    info->setWordWrap(true);
    dLay->addWidget(info);
    lay->addWidget(desc);

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
        printLine(t1Out, "✗ Ошибка: введите целое неотрицательное число.", Theme::ERROR_CLR);
        return;
    }

    printKV(t1Out, "Входное число:  ", QString::number(n), Theme::ACCENT2);
    printSep(t1Out);

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
            bin + QString("  <span style='color:%1;font-size:11px;'>[%2 бит]</span>")
                      .arg(Theme::TEXT_DIM).arg(bin.length()),
            Theme::SUCCESS);

    QString bits;
    for (QChar c : bin)
        bits += (c == '1')
            ? QString("<span style='color:%1;background:%2;"
                      "border-radius:3px;padding:1px 5px;margin:1px;'>1</span> ")
                  .arg(Theme::TEXT_PRI).arg(Theme::ACCENT)
            : QString("<span style='color:%1;background:%2;"
                      "border-radius:3px;padding:1px 5px;margin:1px;'>0</span> ")
                  .arg(Theme::TEXT_DIM).arg(Theme::BG_CARD);
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
        "<code>A(0,n)=n+1 | A(m,0)=A(m-1,1) | A(m,n)=A(m-1,A(m,n-1))</code><br>"
        "<span style='color:#ef4444;'>⚠</span> При m≥4 рост катастрофически быстрый.");
    info->setObjectName("taskDesc");
    info->setWordWrap(true);
    dLay->addWidget(info);
    lay->addWidget(desc);

    auto *inputCard = makeCard("Ввод");
    auto *iLay = new QHBoxLayout(inputCard);
    QLabel *lm = new QLabel("m:"); lm->setObjectName("inputLabel"); lm->setFixedWidth(28);
    t2M = new QSpinBox; t2M->setRange(0,4); t2M->setValue(2); t2M->setFixedWidth(80);
    QLabel *ln = new QLabel("n:"); ln->setObjectName("inputLabel"); ln->setFixedWidth(28);
    t2N = new QSpinBox; t2N->setRange(0,10); t2N->setValue(2); t2N->setFixedWidth(80);
    auto *btn = makeRunButton();
    connect(btn, &QPushButton::clicked, this, &MainWindow::runTask2);
    iLay->addWidget(lm); iLay->addWidget(t2M);
    iLay->addSpacing(16);
    iLay->addWidget(ln); iLay->addWidget(t2N);
    iLay->addStretch();
    iLay->addWidget(btn);
    lay->addWidget(inputCard);

    auto *tableCard = makeCard("Справочная таблица A(m,n)");
    auto *tLay = new QVBoxLayout(tableCard);
    QTextEdit *table = makeOutput();
    table->setMaximumHeight(120);
    QString hdr = "<table style='border-collapse:collapse;font-size:12px;'>";
    hdr += "<tr><td style='padding:2px 8px;color:" + Theme::TEXT_DIM + ";'>m\\n</td>";
    for (int n2=0;n2<=5;n2++)
        hdr += QString("<td style='padding:2px 10px;color:%1;font-weight:700;'>%2</td>")
                   .arg(Theme::ACCENT2).arg(n2);
    hdr += "</tr>";
    for (int m2=0;m2<=3;m2++) {
        hdr += QString("<tr><td style='padding:2px 8px;color:%1;font-weight:700;'>%2</td>")
                   .arg(Theme::ACCENT).arg(m2);
        for (int n2=0;n2<=5;n2++) {
            int d=0;
            try { long long v=ackermann(m2,n2,d);
                hdr+=QString("<td style='padding:2px 10px;color:%1;'>%2</td>")
                         .arg(Theme::SUCCESS).arg(v);
            } catch(...) {
                hdr+=QString("<td style='color:%1;'>…</td>").arg(Theme::TEXT_DIM);
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
        if (calls > 100000)
            printLine(t2Out, "⚡ Функция Аккермана растёт быстрее любой примитивно-рекурсивной!", Theme::WARNING);
    } catch (const std::overflow_error &e) {
        printLine(t2Out, QString("✗ Переполнение: %1").arg(e.what()), Theme::ERROR_CLR);
        printLine(t2Out, "  Попробуйте меньшие значения.", Theme::TEXT_SEC);
    }

    scrollBottom(t2Out);
    statusBar()->showMessage(QString("  Задание 2: A(%1,%2)").arg(m).arg(n));
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
        "Разворачивает цифры числа <b>без циклов, строк и массивов</b>.<br>"
        "<code>rev(n) = (n%10) × 10^len(n÷10) + rev(n÷10)</code>");
    info->setObjectName("taskDesc");
    info->setWordWrap(true);
    dLay->addWidget(info);
    lay->addWidget(desc);

    auto *inputCard = makeCard("Ввод");
    auto *iLay = new QHBoxLayout(inputCard);
    QLabel *lbl = new QLabel("Число:"); lbl->setObjectName("inputLabel"); lbl->setFixedWidth(80);
    t3Input = new QLineEdit("972");
    t3Input->setPlaceholderText("Число без нулей в записи");
    auto *btn = makeRunButton();
    connect(btn, &QPushButton::clicked, this, &MainWindow::runTask3);
    iLay->addWidget(lbl); iLay->addWidget(t3Input); iLay->addWidget(btn);
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
        printLine(t3Out, "✗ Ошибка: введите положительное число без нулей.", Theme::ERROR_CLR);
        return;
    }
    QString s = QString::number(n);
    if (s.contains('0')) {
        printLine(t3Out, "✗ Ошибка: число не должно содержать нулей.", Theme::ERROR_CLR);
        return;
    }

    printKV(t3Out, "Входное число:  ", QString::number(n), Theme::ACCENT2);

    QString digits;
    for (int i = 0; i < s.length(); i++)
        digits += QString("<span style='background:%1;color:#fff;"
                          "border-radius:4px;padding:2px 8px;margin:2px;"
                          "font-size:16px;font-weight:700;'>%2</span> ")
                      .arg(Theme::ACCENT).arg(s[i]);
    t3Out->append("<div style='margin:6px 0;'>"
                  "<span style='color:" + Theme::TEXT_SEC + ";'>Цифры:  </span>"
                  + digits + "</div>");
    printSep(t3Out);

    printLine(t3Out, "Шаги рекурсии:", Theme::TEXT_SEC);
    long long tmp = n;
    int depth = 0;
    while (tmp > 9) {
        QString indent(depth * 2, QChar(' '));
        t3Out->append(QString(
            "<div style='color:%1;font-size:12px;'>"
            "  <span style='color:%2;'>%3rev(%4)</span>"
            " → (%5 %% 10) × 10^%6 + rev(%7)</div>")
            .arg(Theme::TEXT_PRI, Theme::ACCENT)
            .arg(indent).arg(tmp).arg(tmp)
            .arg(countDigits(tmp / 10)).arg(tmp / 10));
        tmp /= 10;
        depth++;
    }
    printLine(t3Out, QString("  базовый случай: rev(%1) = %1").arg(tmp), Theme::SUCCESS);

    printSep(t3Out);
    long long result = reverseNum(n);
    printKV(t3Out, "Результат:      ", QString::number(result), Theme::SUCCESS);

    QString sres = QString::number(result);
    QString revDigits;
    for (int i = 0; i < sres.length(); i++)
        revDigits += QString("<span style='background:%1;color:#fff;"
                             "border-radius:4px;padding:2px 8px;margin:2px;"
                             "font-size:16px;font-weight:700;'>%2</span> ")
                         .arg(Theme::SUCCESS).arg(sres[i]);
    t3Out->append("<div style='margin:6px 0;'>"
                  "<span style='color:" + Theme::TEXT_SEC + ";'>Итог:   </span>"
                  + revDigits + "</div>");

    scrollBottom(t3Out);
    statusBar()->showMessage(
        QString("  Задание 3: rev(%1) = %2").arg(n).arg(result));
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 4 — ХАНОЙСКАЯ БАШНЯ
//
//  Архитектура анимации:
//  • collectHanoiMoves()  — рекурсивно собирает все ходы в m_moves
//  • drawHanoiInitial()   — рисует начальное состояние (все кольца на A)
//  • animateNextHanoiMove() — пошагово анимирует каждый ход:
//      фаза 1 → подъём (OutCubic)
//      фаза 2 → горизонтальный скользок (InOutSine)
//      фаза 3 → падение (InCubic)
//  • m_generation — счётчик запусков; при новом запуске старая
//    анимация «отменяется» через проверку поколения.
// ═══════════════════════════════════════════════════════════════════════

// ── Константы сцены ────────────────────────────────────────────────────
static const double HNX[3] = { 117.0, 350.0, 583.0 }; // X-центры трёх стержней
static const double HBY    = 270.0;  // Y-координата основания (кольца кладутся выше)
static const double HRH    = 26.0;   // высота одного кольца (px)
static const double HSW    = 710.0;  // ширина сцены
static const double HSH    = 330.0;  // высота сцены

// Цвета колец (8 штук, повторяются по модулю)
static const QColor RING_CLR[] = {
    {139,  92, 246},   // фиолетовый
    {167, 139, 250},   // светло-фиолетовый
    {244, 114, 182},   // розовый
    {251, 113, 133},   // розово-красный
    {  6, 182, 212},   // циан
    { 34, 211, 238},   // светлый циан
    { 16, 185, 129},   // изумрудный
    { 52, 211, 153},   // светло-изумрудный
};

// ── Вспомогательные методы ─────────────────────────────────────────────

// Ширина кольца с номером ringNum (1=мин, n=макс)
double MainWindow::hanoiRingW(int ringNum) const
{
    if (m_hanoiN <= 1) return 88.0;
    const double minW = 50.0;
    const double maxW = qMin(50.0 + (double)m_hanoiN * 22.0, 198.0);
    return minW + (double)(ringNum - 1) * (maxW - minW) / (double)(m_hanoiN - 1);
}

// Y-координата левого верхнего угла кольца на позиции stackPos (0=дно)
double MainWindow::hanoiRingY(int stackPos) const
{
    return HBY - (double)(stackPos + 1) * HRH;
}

// Рекурсивный сбор ходов (from/to/via — индексы 0=A,1=B,2=C)
void MainWindow::collectHanoiMoves(int n, int from, int to, int via)
{
    if (n <= 0) return;
    collectHanoiMoves(n - 1, from, via, to);
    m_moves.append({n, from, to});
    collectHanoiMoves(n - 1, via, to, from);
}

// Строковый вывод для текстового лога (оставлен из исходника)
void MainWindow::hanoi(int n, char from, char to, char via,
                        QStringList &steps, int &stepNo)
{
    if (n == 1) {
        steps << QString("<span style='color:%1;'>Шаг %2:</span>"
                         " Кольцо 1: <b style='color:%3;'>%4</b>"
                         " → <b style='color:%5;'>%6</b>")
                     .arg(Theme::TEXT_DIM).arg(stepNo++)
                     .arg(Theme::WARNING).arg(from)
                     .arg(Theme::SUCCESS).arg(to);
        return;
    }
    hanoi(n - 1, from, via, to, steps, stepNo);
    steps << QString("<span style='color:%1;'>Шаг %2:</span>"
                     " Кольцо %3: <b style='color:%4;'>%5</b>"
                     " → <b style='color:%6;'>%7</b>")
                 .arg(Theme::TEXT_DIM).arg(stepNo++).arg(n)
                 .arg(Theme::WARNING).arg(from)
                 .arg(Theme::SUCCESS).arg(to);
    hanoi(n - 1, via, to, from, steps, stepNo);
}

// ── Отрисовка начального состояния ────────────────────────────────────
void MainWindow::drawHanoiInitial(int n)
{
    // Останавливаем текущую анимацию (если идёт)
    if (m_activeAnim) {
        m_activeAnim->stop();
        // Отключаем сигнал finished, чтобы callback не вызвался
        disconnect(m_activeAnim, nullptr, this, nullptr);
        m_activeAnim->deleteLater();
        m_activeAnim = nullptr;
    }

    t4Scene->clear();
    m_ringItems.clear();
    m_stacks.clear();
    m_stacks.resize(3);
    m_hanoiN = n;

    t4Scene->setSceneRect(0, 0, HSW, HSH);

    const double poleH   = (n + 2.5) * HRH;
    const double baseW   = 210.0;
    const double baseH   = 15.0;

    // ── Стержни и основания ────────────────────────
    for (int i = 0; i < 3; i++) {
        double x = HNX[i];

        // Основание
        QGraphicsRectItem *base =
            new QGraphicsRectItem(x - baseW/2, HBY + 1, baseW, baseH);
        base->setBrush(QColor("#252540"));
        base->setPen(QPen(QColor("#3a3a60"), 1.5));
        base->setZValue(0);
        t4Scene->addItem(base);

        // Стержень с градиентом
        QLinearGradient pg(x - 7, 0, x + 7, 0);
        pg.setColorAt(0.0, QColor("#6b7f90"));
        pg.setColorAt(0.5, QColor("#a8bfcc"));
        pg.setColorAt(1.0, QColor("#445566"));
        QGraphicsRectItem *pole =
            new QGraphicsRectItem(x - 7, HBY - poleH, 14, poleH);
        pole->setBrush(pg);
        pole->setPen(Qt::NoPen);
        pole->setZValue(1);
        t4Scene->addItem(pole);

        // Подпись (A / B / C)
        auto *lbl = new QGraphicsTextItem(QString(QChar('A' + i)));
        lbl->setDefaultTextColor(QColor("#94a3b8"));
        lbl->setFont(QFont("Segoe UI", 13, QFont::Bold));
        lbl->setPos(x - 8, HBY + 18);
        t4Scene->addItem(lbl);
    }

    // ── Кольца на стержне A ─────────────────────────
    // ringNum n (наибольшее) → stackPos 0 (дно),
    // ringNum 1 (наименьшее) → stackPos n-1 (верх)
    for (int ringNum = n; ringNum >= 1; ringNum--) {
        int stackPos = n - ringNum;
        double rw = hanoiRingW(ringNum);
        double rx = HNX[0];          // центр X
        double ry = hanoiRingY(stackPos); // верхний Y

        QColor c = RING_CLR[(ringNum - 1) % 8];

        // Скруглённый прямоугольник, начало в (-rw/2, 0) относительно item
        QPainterPath path;
        path.addRoundedRect(-rw / 2.0, 0.0, rw, HRH - 3.0, 7.0, 7.0);

        auto *item = new QGraphicsPathItem(path);

        QLinearGradient rg(-rw/2, 0, rw/2, HRH);
        rg.setColorAt(0.0, c.lighter(145));
        rg.setColorAt(0.45, c);
        rg.setColorAt(1.0, c.darker(135));
        item->setBrush(rg);
        item->setPen(QPen(c.lighter(175), 1.5));
        item->setZValue(2);
        item->setPos(rx, ry);

        // Свечение
        auto *glow = new QGraphicsDropShadowEffect;
        glow->setBlurRadius(16);
        glow->setColor(c);
        glow->setOffset(0, 2);
        item->setGraphicsEffect(glow);

        // Номер кольца поверх
        auto *numLbl = new QGraphicsSimpleTextItem(QString::number(ringNum), item);
        numLbl->setBrush(QColor(255, 255, 255, 200));
        numLbl->setFont(QFont("Segoe UI", 8, QFont::Bold));
        // Примерно центрируем (ширина цифры ~7px, высота ~10px)
        numLbl->setPos(-4.5, (HRH - 3.0) / 2.0 - 6.0);

        t4Scene->addItem(item);
        m_ringItems[ringNum] = item;
    }

    // Стек стержня A: снизу вверх = [n, n-1, ..., 1]
    for (int ringNum = n; ringNum >= 1; ringNum--)
        m_stacks[0].append(ringNum);
    // last() = 1 = верхнее кольцо

    t4View->fitInView(t4Scene->sceneRect(), Qt::KeepAspectRatio);
}

// ── Пошаговая анимация ────────────────────────────────────────────────
void MainWindow::animateNextHanoiMove(int generation)
{
    // Устаревший вызов — игнорируем
    if (generation != m_generation) return;

    if (m_moveIdx >= m_moves.size()) {
        statusBar()->showMessage(
            QString("  ✓ Ханойская башня решена! (%1 ходов)")
                .arg(m_moves.size()));
        return;
    }

    HanoiMove mv = m_moves[m_moveIdx++];

    QGraphicsPathItem *item = m_ringItems.value(mv.ringNum, nullptr);
    if (!item) {
        // Кольцо не найдено — пропускаем и идём дальше
        QTimer::singleShot(10, this, [this, generation]() {
            animateNextHanoiMove(generation);
        });
        return;
    }

    const double rw = hanoiRingW(mv.ringNum);

    // Исходная позиция: верхнее кольцо стека fromTower
    const double srcX = HNX[mv.fromTower];
    const double srcY = HBY - (double)m_stacks[mv.fromTower].size() * HRH;

    // Целевая позиция
    const double dstX = HNX[mv.toTower];
    const double dstY = HBY - (double)(m_stacks[mv.toTower].size() + 1) * HRH;

    // Высота подъёма — выше любого возможного стека + запас
    const double liftY = HBY - (double)(m_hanoiN + 3) * HRH;

    // Скорость: меньше колец → дольше; больше → быстрее
    const int totalMs = qMax(240, 1800 / m_hanoiN);
    const int liftMs  = totalMs * 30 / 100;
    const int slideMs = totalMs * 42 / 100;
    const int dropMs  = totalMs * 28 / 100;

    auto *group = new QSequentialAnimationGroup(this);

    // ── Фаза 1: ПОДЪЁМ ─────────────────────────────
    {
        auto *anim = new QVariantAnimation(this);
        anim->setDuration(liftMs);
        anim->setStartValue(srcY);
        anim->setEndValue(liftY);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        // item захватываем как сырой указатель; при сбросе сцены
        // generation-проверка предотвратит новые вызовы,
        // а m_activeAnim->stop() прекратит текущий
        connect(anim, &QVariantAnimation::valueChanged, this,
            [item, srcX](const QVariant &v) {
                item->setPos(srcX, v.toDouble());
            });
        group->addAnimation(anim);
    }

    // ── Фаза 2: ГОРИЗОНТАЛЬНОЕ СКОЛЬЖЕНИЕ ──────────
    {
        auto *anim = new QVariantAnimation(this);
        anim->setDuration(slideMs);
        anim->setStartValue(srcX);
        anim->setEndValue(dstX);
        anim->setEasingCurve(QEasingCurve::InOutSine);
        connect(anim, &QVariantAnimation::valueChanged, this,
            [item, liftY](const QVariant &v) {
                item->setPos(v.toDouble(), liftY);
            });
        group->addAnimation(anim);
    }

    // ── Фаза 3: ПАДЕНИЕ ────────────────────────────
    {
        auto *anim = new QVariantAnimation(this);
        anim->setDuration(dropMs);
        anim->setStartValue(liftY);
        anim->setEndValue(dstY);
        anim->setEasingCurve(QEasingCurve::InCubic);
        connect(anim, &QVariantAnimation::valueChanged, this,
            [item, dstX](const QVariant &v) {
                item->setPos(dstX, v.toDouble());
            });
        group->addAnimation(anim);
    }

    // Короткая пауза после «удара» об основание (звуковой эффект на глаз)
    group->addAnimation(new QPauseAnimation(qMax(25, 60 / m_hanoiN), this));

    // ── Завершение хода ────────────────────────────
    int gen   = generation;
    int idx   = m_moveIdx;
    int total = m_moves.size();
    int rNum  = mv.ringNum;
    int fT    = mv.fromTower;
    int tT    = mv.toTower;

    connect(group, &QSequentialAnimationGroup::finished, this,
        [this, gen, idx, total, rNum, fT, tT, mv]() {
            m_activeAnim = nullptr;

            // Обновляем стеки
            m_stacks[mv.fromTower].removeLast();
            m_stacks[mv.toTower].append(mv.ringNum);

            statusBar()->showMessage(
                QString("  Ход %1 / %2  ·  кольцо %3: %4 → %5")
                    .arg(idx).arg(total).arg(rNum)
                    .arg(QChar('A' + fT)).arg(QChar('A' + tT)));

            // Небольшая пауза между ходами
            const int pause = qMax(20, 50 / m_hanoiN);
            QTimer::singleShot(pause, this, [this, gen]() {
                animateNextHanoiMove(gen);
            });
        });

    m_activeAnim = group;
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

// ── Виджет вкладки 4 ──────────────────────────────────────────────────
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
        "на <b>C</b>, используя <b>B</b> как вспомогательный.<br>"
        "<span style='color:#fbbf24;'>Минимальное число ходов:</span> "
        "<code>2ⁿ − 1</code>&nbsp;&nbsp;&nbsp;"
        "<span style='color:#8b5cf6;font-size:11px;'>"
        "✨ Башня сначала показывается в собранном виде, затем анимированно решается.</span>");
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
    t4Rings->setRange(1, 8);
    t4Rings->setValue(4);
    t4Rings->setFixedWidth(90);
    auto *btn = makeRunButton();
    connect(btn, &QPushButton::clicked, this, &MainWindow::runTask4);
    iLay->addWidget(lbl);
    iLay->addWidget(t4Rings);
    iLay->addStretch();
    iLay->addWidget(btn);
    lay->addWidget(inputCard);

    // ── Визуализатор ───────────────────────────────
    auto *vizCard = makeCard("🎨 Визуализация  (A → C  через  B)");
    auto *vLay = new QVBoxLayout(vizCard);
    t4Scene = new QGraphicsScene(this);
    t4Scene->setBackgroundBrush(QColor("#07070f"));

    t4View = new QGraphicsView(t4Scene);
    t4View->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    t4View->setMinimumHeight(285);
    t4View->setMaximumHeight(325);
    t4View->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    t4View->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    t4View->setStyleSheet(
        "QGraphicsView { border:1px solid #2d2d4a; border-radius:8px; background:#07070f; }");
    vLay->addWidget(t4View);
    lay->addWidget(vizCard);

    // ── Шаги ───────────────────────────────────────
    auto *outCard = makeCard("Шаги решения");
    auto *oLay = new QVBoxLayout(outCard);
    t4Out = makeOutput();
    printHeader(t4Out, "Ханойская башня", "Задание 4");
    oLay->addWidget(t4Out);
    lay->addWidget(outCard, 1);

    return w;
}

// ── Слот «Запустить» ──────────────────────────────────────────────────
void MainWindow::runTask4()
{
    const int n = t4Rings->value();

    // Отменяем предыдущую анимацию, меняя поколение
    ++m_generation;
    const int gen = m_generation;

    // ── Текстовый лог ──────────────────────────────
    printHeader(t4Out, "Ханойская башня", "Задание 4");
    printKV(t4Out, "Колец:  ", QString::number(n), Theme::ACCENT2);
    const long long minMoves = (1LL << n) - 1;
    printKV(t4Out, "Мин. ходов:  ",
            QString::number(minMoves) +
            QString("  <span style='color:%1;font-size:11px;'>(2^%2 − 1)</span>")
                .arg(Theme::TEXT_DIM).arg(n),
            Theme::WARNING);
    printSep(t4Out);

    // Текстовые шаги (для лога)
    QStringList steps;
    int stepNo = 1;
    hanoi(n, 'A', 'C', 'B', steps, stepNo);

    const int maxShow = 64;
    for (int i = 0; i < qMin((int)steps.size(), maxShow); i++)
        t4Out->append("<div>" + steps[i] + "</div>");
    if ((int)steps.size() > maxShow)
        t4Out->append(QString(
            "<div style='color:%1;margin-top:4px;'>"
            "  … ещё %2 ходов (итого %3)</div>")
            .arg(Theme::TEXT_DIM)
            .arg(steps.size() - maxShow)
            .arg(steps.size()));

    printSep(t4Out);
    printLine(t4Out,
        QString("✓ %1 ход(ов) · "
                "<span style='color:%2;'>анимация запустится через ~1.5 с…</span>")
            .arg(steps.size()).arg(Theme::ACCENT2),
        Theme::SUCCESS);
    scrollBottom(t4Out);

    // ── Собираем ходы для анимации ─────────────────
    m_moves.clear();
    m_moveIdx = 0;
    collectHanoiMoves(n, 0, 2, 1);   // A=0, C=2, B=1

    // ── Рисуем начальное состояние (башня собрана) ─
    drawHanoiInitial(n);

    // ── Пауза 1.5 с, затем анимация ────────────────
    statusBar()->showMessage(
        QString("  Задание 4: %1 колец, %2 ходов · показ начального состояния…")
            .arg(n).arg(m_moves.size()));

    QTimer::singleShot(1500, this, [this, gen]() {
        animateNextHanoiMove(gen);
    });
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
            countItems(fi.absoluteFilePath(), files, folders);
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
                               QDir::DirsFirst))
            item->addChild(buildTree(child.absoluteFilePath()));
    } else {
        item->setText(1, QString("%1 KB").arg(fi.size() / 1024.0, 0, 'f', 1));
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

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

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

    t5Tree->clear();
    auto *root = buildTree(path);
    if (root) {
        t5Tree->addTopLevelItem(root);
        root->setExpanded(true);
    }

    printLine(t5Out, " ", Theme::TEXT_PRI);
    printKV(t5Out, "📁 Подпапок:  ", QString::number(folders), Theme::ACCENT2);
    printKV(t5Out, "📄 Файлов:    ", QString::number(files),   Theme::SUCCESS);
    printKV(t5Out, "📊 Итого:     ", QString::number(files + folders), Theme::WARNING);
    printSep(t5Out);
    printLine(t5Out, "✓ Готово. Двойной клик по файлу — открыть.", Theme::TEXT_SEC);

    scrollBottom(t5Out);
    statusBar()->showMessage(
        QString("  Задание 5: %1 папок, %2 файлов в «%3»")
            .arg(folders).arg(files).arg(dir.dirName()));
}