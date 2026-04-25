#include "mainwindow.h"
#include <QSplitter>
#include <QDesktopServices>
#include <QUrl>
#include <cmath>
#include <climits>
#include <stdexcept>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include <QRegularExpression>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>

// ═══════════════════════════════════════════════════════════════════════
//  КОНСТРУКТОР
// ═══════════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Лабораторная работа №3 · Рекурсия · Qt 6 Modern");
    resize(1280, 800);
    setMinimumSize(1024, 680);

    applyModernStyle();

    // ── Центральный виджет ──────────────────────────────
    QWidget *central = new QWidget(this);
    QVBoxLayout *rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);
    rootLayout->setStretchFactor(new QWidget(), 1);

    // ── Шапка с градиентом ──────────────────────────────
    QFrame *header = new QFrame(central);
    header->setFixedHeight(80);
    header->setObjectName("appHeader");
    
    QHBoxLayout *hLay = new QHBoxLayout(header);
    hLay->setContentsMargins(32, 0, 32, 0);

    // Иконка приложения
    QLabel *icon = new QLabel("⟳", header);
    icon->setStyleSheet(QString(
        "font-size:36px; color:%1; margin-right:16px;"
        "font-weight:300;").arg(ModernTheme::ACCENT_PRIMARY));

    // Заголовок
    QVBoxLayout *titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(2);
    QLabel *title = new QLabel("Рекурсивные алгоритмы", header);
    title->setStyleSheet(QString(
        "font-size:26px; font-weight:700; color:%1;"
        "font-family:'Segoe UI',sans-serif; letter-spacing:0.5px;")
        .arg(ModernTheme::TEXT_PRIMARY));
    
    QLabel *sub = new QLabel("Лабораторная работа №3 | КПИ им. Игоря Сикорского", header);
    sub->setStyleSheet(QString(
        "font-size:13px; color:%1; font-weight:400;")
        .arg(ModernTheme::TEXT_SECONDARY));
    
    titleLayout->addWidget(title);
    titleLayout->addWidget(sub);

    // Бейдж версии
    QLabel *ver = new QLabel("Qt 6 + C++17", header);
    ver->setStyleSheet(QString(
        "font-size:11px; color:%1; background:qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 %2, stop:1 %3); border-radius:12px; padding:6px 14px;"
        "font-weight:600; letter-spacing:0.3px;")
        .arg(ModernTheme::BG_PRIMARY)
        .arg(ModernTheme::ACCENT_GRAD_START)
        .arg(ModernTheme::ACCENT_GRAD_END));
    ver->setMinimumHeight(30);

    hLay->addWidget(icon);
    hLay->addLayout(titleLayout);
    hLay->addStretch();
    hLay->addWidget(ver);

    // ── Вкладки ────────────────────────────────────────
    mainTabs = new QTabWidget(central);
    mainTabs->setObjectName("mainTabs");
    mainTabs->setIconSize(QSize(20, 20));
    
    mainTabs->addTab(createTask1Widget(), "\U0001F522 Dec \u2192 Bin");
    mainTabs->addTab(createTask2Widget(), "\u221E \u0410\u043A\u043A\u0435\u0440\u043C\u0430\u043D");
    mainTabs->addTab(createTask3Widget(), "\u21BB \u0420\u0435\u0432\u0435\u0440\u0441");
    mainTabs->addTab(createTask4Widget(), "\U0001F3DB \u0425\u0430\u043D\u043E\u0439");
    mainTabs->addTab(createTask5Widget(), "\U0001F4C1 \u0424\u0430\u0439\u043B\u044B");

    // ── Статус-бар ─────────────────────────────────────
    statusBarWidget = statusBar();
    statusBarWidget->showMessage("  Выберите задачу и нажмите «Запустить» для выполнения");
    statusBarWidget->setStyleSheet(QString(
        "background:%1; color:%2; border-top:1px solid %3; "
        "font-size:13px; padding:6px 16px; font-weight:500;")
        .arg(ModernTheme::BG_SECONDARY)
        .arg(ModernTheme::TEXT_SECONDARY)
        .arg(ModernTheme::BORDER));

    central->setLayout(rootLayout);
    rootLayout->addWidget(header);
    rootLayout->addWidget(mainTabs, 1);
    setCentralWidget(central);
}

// ═══════════════════════════════════════════════════════════════════════
//  СОВРЕМЕННЫЙ СТИЛЬ (Qt 6)
// ═══════════════════════════════════════════════════════════════════════
void MainWindow::applyModernStyle()
{
    qApp->setStyleSheet(QString(R"(
        QMainWindow, QWidget {
            background: %1;
            color: %2;
            font-family: 'Segoe UI', 'Ubuntu', 'Roboto', sans-serif;
        }
        
        /* Шапка */
        QFrame#appHeader {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 %3, stop:0.5 %4, stop:1 %3);
            border-bottom: 1px solid %5;
        }
        
        /* Вкладки */
        QTabWidget#mainTabs::pane {
            border: none;
            background: %1;
            border-top: 1px solid %5;
        }
        
        QTabWidget#mainTabs QTabBar::tab {
            background: %3;
            color: %6;
            border: none;
            border-right: 1px solid %5;
            padding: 14px 24px;
            font-size: 14px;
            font-weight: 600;
            min-width: 140px;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
        }
        
        QTabWidget#mainTabs QTabBar::tab:selected {
            background: %1;
            color: %7;
            border-bottom: 3px solid %8;
        }
        
        QTabWidget#mainTabs QTabBar::tab:hover:!selected {
            background: %9;
            color: %2;
        }
        
        /* Карточки */
        QGroupBox {
            background: %3;
            border: 1px solid %5;
            border-radius: 12px;
            margin-top: 20px;
            padding: 18px 20px 20px 20px;
            font-size: 14px;
            font-weight: 700;
            color: %7;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 16px;
            top: -10px;
            padding: 0 10px;
            color: %8;
            background: %3;
        }
        
        /* Поля ввода */
        QLineEdit, QSpinBox {
            background: %9;
            color: %2;
            border: 2px solid %5;
            border-radius: 8px;
            padding: 10px 16px;
            font-size: 15px;
            font-family: 'Cascadia Code','Consolas','Courier New',monospace;
            selection-background-color: %8;
        }
        
        QLineEdit:focus, QSpinBox:focus {
            border: 2px solid %8;
        }
        
        QSpinBox::up-button, QSpinBox::down-button {
            background: %5;
            border-radius: 4px;
            width: 22px;
            margin: 2px;
        }
        
        QSpinBox::up-button:hover, QSpinBox::down-button:hover {
            background: %8;
        }
        
        /* Кнопки действий */
        QPushButton#actionBtnPrimary {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 %8, stop:1 %10);
            color: #ffffff;
            border: none;
            border-radius: 10px;
            padding: 12px 32px;
            font-size: 15px;
            font-weight: 700;
            letter-spacing: 0.5px;
        }
        
        QPushButton#actionBtnPrimary:hover {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #a78bfa, stop:1 #22d3ee);
        }
        
        QPushButton#actionBtnPrimary:pressed {
            background: %5;
        }
        
        QPushButton#actionBtnSecondary {
            background: %9;
            color: %7;
            border: 2px solid %8;
            border-radius: 10px;
            padding: 10px 24px;
            font-size: 14px;
            font-weight: 600;
        }
        
        QPushButton#actionBtnSecondary:hover {
            background: %3;
            color: %2;
        }
        
        /* Консоль вывода */
        QTextEdit {
            background: %11;
            color: %2;
            border: 1px solid %5;
            border-radius: 10px;
            font-family: 'Cascadia Code','Consolas','Courier New',monospace;
            font-size: 13px;
            padding: 14px;
            selection-background-color: %8;
        }
        
        /* Полосы прокрутки */
        QScrollBar:vertical {
            background: %3;
            width: 10px;
            border-radius: 5px;
        }
        
        QScrollBar::handle:vertical {
            background: %5;
            border-radius: 5px;
            min-height: 30px;
        }
        
        QScrollBar::handle:vertical:hover {
            background: %8;
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
        
        QScrollBar:horizontal {
            background: %3;
            height: 10px;
            border-radius: 5px;
        }
        
        QScrollBar::handle:horizontal {
            background: %5;
            border-radius: 5px;
            min-width: 30px;
        }
        
        /* Дерево файлов */
        QTreeWidget {
            background: %11;
            color: %2;
            border: 1px solid %5;
            border-radius: 10px;
            font-family: 'Cascadia Code','Consolas','Courier New',monospace;
            font-size: 12px;
            alternate-background-color: %9;
        }
        
        QTreeWidget::item:hover { 
            background: %9; 
            border-radius: 4px;
        }
        
        QTreeWidget::item:selected { 
            background: %8; 
            color: #fff; 
            border-radius: 4px;
        }
        
        QTreeWidget::branch:has-children:!has-siblings:closed,
        QTreeWidget::branch:closed:has-children:has-siblings {
            image: none;
        }
        
        /* Графика */
        QGraphicsView {
            background: %11;
            border: 1px solid %5;
            border-radius: 10px;
        }
        
        /* Метки */
        QLabel#sectionLabel {
            color: %7;
            font-size: 14px;
            font-weight: 600;
            background: transparent;
        }
        
        QLabel#inputLabel {
            color: %8;
            font-size: 13px;
            font-weight: 600;
            background: transparent;
        }
        
        /* Разделители */
        QFrame#separator {
            background: %5;
            max-height: 1px;
        }
        
        /* Статус бар */
        QStatusBar { 
            font-size: 13px; 
            font-weight: 500;
        }
        
        /* Прогресс бар */
        QProgressBar {
            background: %9;
            border: none;
            border-radius: 6px;
            text-align: center;
            font-weight: 600;
            color: %2;
        }
        
        QProgressBar::chunk {
            background: qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 %8, stop:1 %10);
            border-radius: 6px;
        }
    )")
    .arg(ModernTheme::BG_PRIMARY)       // %1 - основной фон
    .arg(ModernTheme::TEXT_PRIMARY)     // %2 - основной текст
    .arg(ModernTheme::BG_SECONDARY)     // %3 - фон карточек
    .arg(ModernTheme::BG_TERTIARY)      // %4 - доп. фон
    .arg(ModernTheme::BORDER)           // %5 - границы
    .arg(ModernTheme::TEXT_SECONDARY)   // %6 - вторичный текст
    .arg(ModernTheme::TEXT_HIGHLIGHT)   // %7 - подсветка текста
    .arg(ModernTheme::ACCENT_PRIMARY)   // %8 - акцент
    .arg(ModernTheme::BG_INPUT)         // %9 - фон ввода
    .arg(ModernTheme::ACCENT_SECOND)    // %10 - акцент 2
    .arg(ModernTheme::BG_OUTPUT)        // %11 - фон вывода
    );
}

// ═══════════════════════════════════════════════════════════════════════
//  UI-ХЕЛПЕРЫ
// ═══════════════════════════════════════════════════════════════════════
QGroupBox* MainWindow::createCard(const QString &title, const QString &icon)
{
    auto *gb = new QGroupBox(title);
    if (!icon.isEmpty()) {
        gb->setTitle(icon + "  " + title);
    }
    return gb;
}

QPushButton* MainWindow::createActionButton(const QString &text, const QString &type)
{
    auto *btn = new QPushButton(text);
    btn->setObjectName(type == "primary" ? "actionBtnPrimary" : "actionBtnSecondary");
    btn->setCursor(Qt::PointingHandCursor);
    return btn;
}

QTextEdit* MainWindow::createOutputConsole()
{
    auto *te = new QTextEdit;
    te->setReadOnly(true);
    te->setMinimumHeight(180);
    te->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    return te;
}

QLabel* MainWindow::createSectionLabel(const QString &text)
{
    auto *lbl = new QLabel(text);
    lbl->setObjectName("sectionLabel");
    return lbl;
}

QLineEdit* MainWindow::createInputField(const QString &placeholder)
{
    auto *edit = new QLineEdit;
    edit->setPlaceholderText(placeholder);
    return edit;
}

QSpinBox* MainWindow::createSpinBox(int min, int max, int value)
{
    auto *sb = new QSpinBox;
    sb->setRange(min, max);
    sb->setValue(value);
    return sb;
}

void MainWindow::printHeader(QTextEdit *out, const QString &title, const QString &task)
{
    out->clear();
    out->append(QString(
        "<div style='margin-bottom:8px;'>"
        "<span style='color:%1;font-weight:700;font-size:16px;'>⚡ %2</span>"
        "&nbsp;&nbsp;"
        "<span style='color:%3;font-size:12px;'>%4</span>"
        "</div>"
        "<div style='color:%5;font-size:11px;margin-bottom:10px;'>"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━</div>")
        .arg(ModernTheme::ACCENT_SECOND, title, ModernTheme::TEXT_DIM, task, ModernTheme::BORDER));
}

void MainWindow::printLine(QTextEdit *out, const QString &text, const QString &color)
{
    out->append(QString("<div style='color:%1;line-height:1.6;'>%2</div>").arg(color, text));
}

void MainWindow::printKV(QTextEdit *out, const QString &key, const QString &val,
                          const QString &valColor)
{
    out->append(QString(
        "<div style='margin:4px 0;'>"
        "<span style='color:%1;'>%2</span>"
        "<span style='color:%3;font-weight:700;font-size:16px;font-family:\"Cascadia Code\";'>%4</span>"
        "</div>")
        .arg(ModernTheme::TEXT_SECONDARY, key, valColor, val));
}

void MainWindow::printSep(QTextEdit *out)
{
    out->append(QString("<div style='color:%1;font-size:10px;margin:8px 0;'>"
                        "────────────────────────────────────────────────</div>")
                        .arg(ModernTheme::BORDER));
}

void MainWindow::scrollBottom(QTextEdit *out)
{
    out->verticalScrollBar()->setValue(out->verticalScrollBar()->maximum());
}

QString MainWindow::badge(const QString &text, const QString &color)
{
    return QString("<span style='background:%1;color:#fff;"
                   "border-radius:6px;padding:2px 8px;"
                   "font-size:11px;font-weight:700;margin:0 4px;'>%2</span>")
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
    lay->setContentsMargins(24, 20, 24, 20);
    lay->setSpacing(18);

    // Описание
    auto *desc = createCard("Десятичное → Двоичное", "🔢");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "<span style='color:" + ModernTheme::TEXT_SECONDARY + ";'>Рекурсивно переводит целое десятичное число в двоичную запись.</span><br>"
        "<span style='color:" + ModernTheme::WARNING + ";'>Принцип:</span> "
        "<code style='background:" + ModernTheme::BG_INPUT + ";padding:2px 6px;border-radius:4px;'>bin(n) = bin(n÷2) + (n%2)</code>"
    );
    info->setWordWrap(true);
    info->setStyleSheet("font-size: 14px; line-height: 1.6;");
    dLay->addWidget(info);
    lay->addWidget(desc);

    // Ввод
    auto *inputCard = createCard("Ввод данных", "⌨️");
    auto *iLay = new QHBoxLayout(inputCard);
    
    QLabel *lbl = createSectionLabel("Число:");
    lbl->setFixedWidth(70);
    t1Input = createInputField("Введите целое число ≥ 0");
    t1Input->setText("66");
    t1Input->setMinimumWidth(200);
    
    auto *btn = createActionButton("▶ Запустить", "primary");
    connect(btn, &QPushButton::clicked, this, &MainWindow::runTask1);
    
    iLay->addWidget(lbl);
    iLay->addWidget(t1Input, 1);
    iLay->addWidget(btn);
    lay->addWidget(inputCard);

    // Вывод
    auto *outCard = createCard("Результат вычисления", "📊");
    auto *oLay = new QVBoxLayout(outCard);
    t1Out = createOutputConsole();
    t1Out->setMaximumHeight(320);
    printHeader(t1Out, "Dec → Bin", "Задание №1");
    oLay->addWidget(t1Out);
    lay->addWidget(outCard);

    lay->addStretch();
    w->setLayout(lay);
    return w;
}

void MainWindow::runTask1()
{
    bool ok;
    long long n = t1Input->text().toLongLong(&ok);
    printHeader(t1Out, "Dec → Bin", "Задание №1");

    if (!ok || n < 0) {
        printLine(t1Out, "✗ Ошибка: введите целое неотрицательное число.", ModernTheme::ERROR_CLR);
        return;
    }

    printKV(t1Out, "Входное число:  ", QString::number(n), ModernTheme::ACCENT_SECOND);
    printSep(t1Out);

    // Пошаговое разложение
    printLine(t1Out, "Шаги рекурсии:", ModernTheme::TEXT_SECONDARY);
    long long tmp = n;
    QStringList steps;
    while (tmp > 1) {
        steps << QString("  %1 ÷ 2 = %2  остаток <b style='color:%3;'>%4</b>")
                   .arg(tmp).arg(tmp/2).arg(ModernTheme::WARNING).arg(tmp%2);
        tmp /= 2;
    }
    steps << QString("  базовый случай: <b style='color:%1;'>%2</b>")
               .arg(ModernTheme::SUCCESS).arg(tmp);
    for (const auto &s : steps) 
        printLine(t1Out, s, ModernTheme::TEXT_PRIMARY);

    printSep(t1Out);

    QString bin = (n == 0) ? "0" : decToBinRec(n);
    printKV(t1Out, "Результат:      ", 
            bin + QString("  <span style='color:%1;font-size:11px;'>[%2 бит]</span>")
                .arg(ModernTheme::TEXT_DIM).arg(bin.length()),
            ModernTheme::SUCCESS);

    // Визуальное представление битов
    QString bits;
    for (QChar c : bin) {
        if (c == '1')
            bits += QString("<span style='color:%1;background:%2;"
                            "border-radius:4px;padding:2px 6px;margin:2px;font-weight:700;'>1</span> ")
                        .arg(ModernTheme::TEXT_PRIMARY).arg(ModernTheme::ACCENT_PRIMARY);
        else
            bits += QString("<span style='color:%1;background:%2;"
                            "border-radius:4px;padding:2px 6px;margin:2px;font-weight:700;'>0</span> ")
                        .arg(ModernTheme::TEXT_DIM).arg(ModernTheme::BG_TERTIARY);
    }
    t1Out->append("<div style='margin-top:10px;'>" + bits + "</div>");

    scrollBottom(t1Out);
    statusBarWidget->showMessage(
        QString("  ✓ Задание 1 выполнено: %1₁₀ = %2₂").arg(n).arg(bin));
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
    lay->setContentsMargins(24, 20, 24, 20);
    lay->setSpacing(18);

    // Описание
    auto *desc = createCard("Функция Аккермана", "∞");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "<span style='color:" + ModernTheme::TEXT_SECONDARY + ";'>Функция Аккермана A(m,n) — классический пример взаимной рекурсии.</span><br>"
        "<code style='background:" + ModernTheme::BG_INPUT + ";padding:3px 8px;border-radius:4px;display:inline-block;margin:4px 0;'>"
        "A(0,n) = n+1 &nbsp;&nbsp;|&nbsp;&nbsp; A(m,0) = A(m-1,1) &nbsp;&nbsp;|&nbsp;&nbsp; A(m,n) = A(m-1, A(m, n-1))</code><br>"
        "<span style='color:" + ModernTheme::ERROR_CLR + ";'>⚠</span> При m≥4 рост катастрофически быстрый."
    );
    info->setWordWrap(true);
    info->setStyleSheet("font-size: 14px; line-height: 1.7;");
    dLay->addWidget(info);
    
    // Таблица значений
    QLabel *tableInfo = new QLabel(
        "<table style='width:100%;border-collapse:collapse;margin-top:8px;'>"
        "<tr style='background:" + ModernTheme::BG_TERTIARY + ";'><th style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";'>m\\n</th>"
        "<th style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";'>0</th>"
        "<th style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";'>1</th>"
        "<th style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";'>2</th>"
        "<th style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";'>3</th></tr>"
        "<tr><td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";font-weight:700;'>0</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>1</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>2</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>3</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>4</td></tr>"
        "<tr><td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";font-weight:700;'>1</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>2</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>3</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>4</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>5</td></tr>"
        "<tr><td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";font-weight:700;'>2</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>3</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>5</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>7</td>"
        "<td style='padding:6px;border:1px solid " + ModernTheme::BORDER + ";text-align:center;'>9</td></tr>"
        "</table>"
    );
    tableInfo->setStyleSheet("color: " + ModernTheme::TEXT_PRIMARY + "; font-size: 12px;");
    dLay->addWidget(tableInfo);
    
    lay->addWidget(desc);

    // Ввод
    auto *inputCard = createCard("Параметры функции", "⌨️");
    auto *iLay = new QHBoxLayout(inputCard);

    QLabel *lm = createSectionLabel("m:");
    lm->setFixedWidth(30);
    t2M = createSpinBox(0, 4, 2);
    t2M->setFixedWidth(80);

    QLabel *ln = createSectionLabel("n:");
    ln->setFixedWidth(30);
    t2N = createSpinBox(0, 10, 3);
    t2N->setFixedWidth(80);

    auto *btn = createActionButton("▶ Вычислить A(m,n)", "primary");
    connect(btn, &QPushButton::clicked, this, &MainWindow::runTask2);

    iLay->addWidget(lm);
    iLay->addWidget(t2M);
    iLay->addSpacing(20);
    iLay->addWidget(ln);
    iLay->addWidget(t2N);
    iLay->addStretch();
    iLay->addWidget(btn);
    lay->addWidget(inputCard);

    // Вывод
    auto *outCard = createCard("Результат вычисления", "📊");
    auto *oLay = new QVBoxLayout(outCard);
    t2Out = createOutputConsole();
    t2Out->setMaximumHeight(320);
    printHeader(t2Out, "Ackermann Function", "Задание №2");
    oLay->addWidget(t2Out);
    lay->addWidget(outCard);

    lay->addStretch();
    w->setLayout(lay);
    return w;
}

void MainWindow::runTask2()
{
    int m = t2M->value();
    int n = t2N->value();
    printHeader(t2Out, "Ackermann Function", "Задание №2");

    printKV(t2Out, "Параметры:      ", QString("A(%1, %2)").arg(m).arg(n), ModernTheme::ACCENT_SECOND);
    printSep(t2Out);

    try {
        int calls = 0;
        long long result = ackermann(m, n, calls);

        printLine(t2Out, "Вычисление прошло успешно:", ModernTheme::SUCCESS);
        printKV(t2Out, "Результат:      ", QString("A(%1,%2) = %3").arg(m).arg(n).arg(result), ModernTheme::SUCCESS);
        printKV(t2Out, "Рекурсивных вызовов: ", QString::number(calls), ModernTheme::WARNING);

        // Инфо о сложности
        QString complexity;
        if (m == 0) complexity = "O(1) — константная сложность";
        else if (m == 1) complexity = "O(n) — линейная сложность";
        else if (m == 2) complexity = "O(n²) — квадратичная сложность";
        else if (m == 3) complexity = "O(2ⁿ) — экспоненциальная сложность";
        else complexity = "O(↑↑n) — сверхэкспоненциальный рост";
        
        printLine(t2Out, "Сложность:      " + complexity, ModernTheme::INFO);

    } catch (const std::overflow_error &e) {
        printLine(t2Out, QString("✗ Ошибка: %1").arg(e.what()), ModernTheme::ERROR_CLR);
    } catch (...) {
        printLine(t2Out, "✗ Неизвестная ошибка вычисления", ModernTheme::ERROR_CLR);
    }

    scrollBottom(t2Out);
    statusBarWidget->showMessage(QString("  ✓ Задание 2: A(%1,%2) вычислено").arg(m).arg(n));
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 3 — РЕВЕРС ЧИСЛА
// ═══════════════════════════════════════════════════════════════════════
int MainWindow::countDigits(long long n)
{
    if (n == 0) return 1;
    int count = 0;
    n = qAbs(n);
    while (n > 0) {
        ++count;
        n /= 10;
    }
    return count;
}

long long MainWindow::reverseNum(long long n)
{
    bool negative = n < 0;
    n = qAbs(n);
    
    long long reversed = 0;
    while (n > 0) {
        reversed = reversed * 10 + n % 10;
        n /= 10;
    }
    return negative ? -reversed : reversed;
}

QWidget* MainWindow::createTask3Widget()
{
    QWidget *w = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(24, 20, 24, 20);
    lay->setSpacing(18);

    // Описание
    auto *desc = createCard("Реверс числа", "↻");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "<span style='color:" + ModernTheme::TEXT_SECONDARY + ";'>Рекурсивно переворачивает число задом наперёд.</span><br>"
        "<span style='color:" + ModernTheme::WARNING + ";'>Алгоритм:</span> извлекаем последнюю цифру и добавляем её к результату умноженному на 10."
    );
    info->setWordWrap(true);
    info->setStyleSheet("font-size: 14px; line-height: 1.6;");
    dLay->addWidget(info);
    lay->addWidget(desc);

    // Ввод
    auto *inputCard = createCard("Ввод данных", "⌨️");
    auto *iLay = new QHBoxLayout(inputCard);
    
    QLabel *lbl = createSectionLabel("Число:");
    lbl->setFixedWidth(70);
    t3Input = createInputField("Введите целое число");
    t3Input->setText("972");
    t3Input->setMinimumWidth(200);
    
    auto *btn = createActionButton("▶ Реверсировать", "primary");
    connect(btn, &QPushButton::clicked, this, &MainWindow::runTask3);
    
    iLay->addWidget(lbl);
    iLay->addWidget(t3Input, 1);
    iLay->addWidget(btn);
    lay->addWidget(inputCard);

    // Вывод
    auto *outCard = createCard("Результат вычисления", "📊");
    auto *oLay = new QVBoxLayout(outCard);
    t3Out = createOutputConsole();
    t3Out->setMaximumHeight(320);
    printHeader(t3Out, "Number Reverse", "Задание №3");
    oLay->addWidget(t3Out);
    lay->addWidget(outCard);

    lay->addStretch();
    w->setLayout(lay);
    return w;
}

void MainWindow::runTask3()
{
    bool ok;
    long long n = t3Input->text().toLongLong(&ok);
    printHeader(t3Out, "Number Reverse", "Задание №3");

    if (!ok) {
        printLine(t3Out, "✗ Ошибка: введите целое число.", ModernTheme::ERROR_CLR);
        return;
    }

    printKV(t3Out, "Исходное число: ", QString::number(n), ModernTheme::ACCENT_SECOND);
    printSep(t3Out);

    // Пошаговое разложение
    printLine(t3Out, "Шаги реверса:", ModernTheme::TEXT_SECONDARY);
    long long tmp = qAbs(n);
    QStringList digits;
    int pos = 0;
    while (tmp > 0) {
        int digit = tmp % 10;
        digits << QString("  Позиция %1: извлекаем цифру <b style='color:%2;'>%3</b>")
                    .arg(pos).arg(ModernTheme::WARNING).arg(digit);
        tmp /= 10;
        pos++;
    }
    for (const auto &s : digits) 
        printLine(t3Out, s, ModernTheme::TEXT_PRIMARY);

    printSep(t3Out);

    long long reversed = reverseNum(n);
    printKV(t3Out, "Результат:      ", QString::number(reversed), ModernTheme::SUCCESS);
    
    // Визуальное представление
    QString origStr = QString::number(qAbs(n));
    QString revStr = QString::number(qAbs(reversed));
    QString visual;
    for (int i = 0; i < origStr.length(); ++i) {
        visual += QString("<span style='color:%1;background:%2;"
                         "border-radius:4px;padding:2px 6px;margin:2px;font-weight:700;font-size:15px;'>%3</span>")
                     .arg(ModernTheme::TEXT_PRIMARY).arg(ModernTheme::BG_TERTIARY).arg(origStr[i]);
        if (i < origStr.length() - 1)
            visual += " → ";
    }
    visual += "&nbsp;&nbsp;<span style='color:" + ModernTheme::ACCENT_PRIMARY + ";font-size:18px;'>⇄</span>&nbsp;&nbsp;";
    for (int i = 0; i < revStr.length(); ++i) {
        visual += QString("<span style='color:%1;background:%2;"
                         "border-radius:4px;padding:2px 6px;margin:2px;font-weight:700;font-size:15px;'>%3</span>")
                     .arg(ModernTheme::SUCCESS).arg(ModernTheme::BG_INPUT).arg(revStr[i]);
    }
    if (n < 0) visual = "<span style='color:" + ModernTheme::ERROR_CLR + ";'>−</span>" + visual;
    if (reversed < 0 && n > 0) visual += "<span style='color:" + ModernTheme::ERROR_CLR + ";'> −</span>";
    
    t3Out->append("<div style='margin-top:10px;line-height:2;'>" + visual + "</div>");

    scrollBottom(t3Out);
    statusBarWidget->showMessage(
        QString("  ✓ Задание 3 выполнено: %1 → %2").arg(n).arg(reversed));
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 4 — ХАНОЙСКАЯ БАШНЯ
// ═══════════════════════════════════════════════════════════════════════
void MainWindow::hanoi(int n, char from, char to, char via, QStringList &steps, int &stepNo)
{
    if (n == 1) {
        steps << QString("Шаг %1: Переместить кольцо 1 с %2 на %3").arg(++stepNo).arg(from).arg(to);
        return;
    }
    hanoi(n - 1, from, via, to, steps, stepNo);
    steps << QString("Шаг %1: Переместить кольцо %2 с %3 на %4").arg(++stepNo).arg(n).arg(from).arg(to);
    hanoi(n - 1, via, to, from, steps, stepNo);
}

void MainWindow::setupHanoiScene(int rings)
{
    t4Scene->clear();
    t4RingsData.clear();
    
    const double sceneWidth = 500;
    const double sceneHeight = 300;
    t4Scene->setSceneRect(0, 0, sceneWidth, sceneHeight);
    
    // Координаты стержней
    t4TowerX[0] = 100;
    t4TowerX[1] = 250;
    t4TowerX[2] = 400;
    t4BaseY = 260;
    t4RingHeight = 18;
    
    // Рисуем основание
    auto *base = new QGraphicsRectItem(20, t4BaseY + 5, 460, 15);
    base->setBrush(QColor(ModernTheme::ACCENT_PRIMARY));
    base->setPen(Qt::NoPen);
    t4Scene->addItem(base);
    
    // Рисуем стержни
    for (int i = 0; i < 3; ++i) {
        auto *pole = new QGraphicsRectItem(t4TowerX[i] - 4, t4BaseY - 150, 8, 150);
        pole->setBrush(QColor(ModernTheme::ACCENT_SECOND));
        pole->setPen(Qt::NoPen);
        t4Scene->addItem(pole);
        
        // Метки стержней
        auto *label = new QGraphicsTextItem(QString(QChar('A' + i)));
        label->setPos(t4TowerX[i] - 5, t4BaseY + 25);
        label->setDefaultTextColor(QColor(ModernTheme::TEXT_SECONDARY));
        label->setFont(QFont("Segoe UI", 14, QFont::Bold));
        t4Scene->addItem(label);
    }
    
    // Создаём кольца на первом стержне
    const double maxWidth = 140;
    const double minRingWidth = 40;
    for (int i = rings - 1; i >= 0; --i) {
        double width = minRingWidth + (maxWidth - minRingWidth) * i / (rings > 1 ? rings - 1 : 1);
        double x = t4TowerX[0] - width / 2;
        double y = t4BaseY - t4RingHeight * (rings - i);
        
        auto *ring = new QGraphicsRectItem(x, y, width, t4RingHeight - 2);
        
        // Градиентный цвет для каждого кольца
        QColor ringColor;
        ringColor.setHsl((i * 60) % 360, 200, 150);
        ring->setBrush(ringColor);
        ring->setPen(QPen(ringColor.lighter(130), 2));
        
        // Тень
        auto *shadow = new QGraphicsDropShadowEffect;
        shadow->setBlurRadius(8);
        shadow->setColor(QColor(0, 0, 0, 100));
        shadow->setOffset(2, 2);
        ring->setGraphicsEffect(shadow);
        
        t4Scene->addItem(ring);
        
        RingInfo info;
        info.item = ring;
        info.ringIndex = i;
        info.currentTower = 0;
        t4RingsData.append(info);
    }
    
    t4Visualizer->setScene(t4Scene);
}

void MainWindow::onHanoiStart()
{
    if (t4IsAnimating) return;
    
    int n = t4Rings->value();
    printHeader(t4Out, "Towers of Hanoi", "Задание №4");
    
    // Настройка сцены
    setupHanoiScene(n);
    
    // Генерация ходов
    QStringList steps;
    int stepNo = 0;
    hanoi(n, 'A', 'C', 'B', steps, stepNo);
    
    printLine(t4Out, QString("Всего колец: <b style='color:%1;'>%2</b>").arg(ModernTheme::ACCENT_SECOND).arg(n), ModernTheme::TEXT_PRIMARY);
    printLine(t4Out, QString("Минимальное количество ходов: <b style='color:%1;'>%2</b>").arg(ModernTheme::SUCCESS).arg(steps.size()), ModernTheme::TEXT_PRIMARY);
    printSep(t4Out);
    
    // Показываем первые несколько шагов
    printLine(t4Out, "Первые шаги:", ModernTheme::TEXT_SECONDARY);
    int showSteps = qMin(5, steps.size());
    for (int i = 0; i < showSteps; ++i) {
        printLine(t4Out, steps[i], ModernTheme::TEXT_PRIMARY);
    }
    if (steps.size() > 5) {
        printLine(t4Out, QString("... и ещё %1 ходов").arg(steps.size() - 5), ModernTheme::TEXT_DIM);
    }
    
    scrollBottom(t4Out);
    statusBarWidget->showMessage(QString("  ✓ Ханойская башня: %1 колец, %2 ходов").arg(n).arg(steps.size()));
}

QWidget* MainWindow::createTask4Widget()
{
    QWidget *w = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(24, 20, 24, 20);
    lay->setSpacing(18);

    // Описание
    auto *desc = createCard("Ханойская башня", "🏛️");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "<span style='color:" + ModernTheme::TEXT_SECONDARY + ";'>Классическая задача о перемещении колец между стержнями.</span><br>"
        "<span style='color:" + ModernTheme::WARNING + ";'>Правила:</span> можно перемещать только одно кольцо за раз, нельзя класть большее кольцо на меньшее."
    );
    info->setWordWrap(true);
    info->setStyleSheet("font-size: 14px; line-height: 1.6;");
    dLay->addWidget(info);
    lay->addWidget(desc);

    // Управление
    auto *controlCard = createCard("Управление", "⌨️");
    auto *cLay = new QHBoxLayout(controlCard);
    
    QLabel *lbl = createSectionLabel("Колец:");
    lbl->setFixedWidth(70);
    t4Rings = createSpinBox(1, 8, 4);
    t4Rings->setFixedWidth(80);
    
    auto *btn = createActionButton("▶ Начать", "primary");
    connect(btn, &QPushButton::clicked, this, &MainWindow::onHanoiStart);
    
    cLay->addWidget(lbl);
    cLay->addWidget(t4Rings);
    cLay->addStretch();
    cLay->addWidget(btn);
    lay->addWidget(controlCard);

    // Визуализатор
    auto *vizCard = createCard("Визуализация", "🎨");
    auto *vLay = new QVBoxLayout(vizCard);
    
    t4Scene = new QGraphicsScene(this);
    t4Visualizer = new QGraphicsView(t4Scene);
    t4Visualizer->setMinimumHeight(280);
    t4Visualizer->setRenderHint(QPainter::Antialiasing);
    t4Visualizer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    t4Visualizer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    t4Visualizer->setStyleSheet("background: " + ModernTheme::BG_OUTPUT + "; border: none;");
    
    vLay->addWidget(t4Visualizer);
    lay->addWidget(vizCard);

    // Вывод
    auto *outCard = createCard("Результат вычисления", "📊");
    auto *oLay = new QVBoxLayout(outCard);
    t4Out = createOutputConsole();
    t4Out->setMaximumHeight(200);
    printHeader(t4Out, "Towers of Hanoi", "Задание №4");
    oLay->addWidget(t4Out);
    lay->addWidget(outCard);

    lay->addStretch();
    w->setLayout(lay);
    return w;
}

void MainWindow::runTask4()
{
    onHanoiStart();
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 5 — ОБХОД ФАЙЛОВОЙ СИСТЕМЫ
// ═══════════════════════════════════════════════════════════════════════
void MainWindow::countItems(const QString &path, int &files, int &folders)
{
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    
    for (const QFileInfo &fi : entries) {
        if (fi.isDir()) {
            ++folders;
            countItems(fi.absoluteFilePath(), files, folders);
        } else {
            ++files;
        }
    }
}

QTreeWidgetItem* MainWindow::buildTree(const QString &path)
{
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::IgnoreCase);
    
    for (const QFileInfo &fi : entries) {
        // Создаём элемент
        auto *item = new QTreeWidgetItem();
        item->setText(0, fi.fileName());
        
        if (fi.isDir()) {
            item->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DirIcon));
            item->setForeground(0, QBrush(QColor(ModernTheme::ACCENT_SECOND)));
            item->setFont(0, QFont("Segoe UI", 12, QFont::Bold));
            
            // Рекурсивно добавляем подэлементы (с ограничением глубины)
            if (dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot).size() < 20) {
                QTreeWidgetItem *child = buildTree(fi.absoluteFilePath());
                if (child) {
                    item->addChild(child);
                }
            } else {
                auto *placeholder = new QTreeWidgetItem(item);
                placeholder->setText(0, "[Содержимое скрыто]");
                placeholder->setForeground(0, QBrush(QColor(ModernTheme::TEXT_DIM)));
            }
        } else {
            // Иконка файла в зависимости от расширения
            QIcon fileIcon;
            QString ext = fi.suffix().toLower();
            if (ext == "cpp" || ext == "h" || ext == "hpp" || ext == "c")
                fileIcon = QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView);
            else if (ext == "txt" || ext == "md" || ext == "log")
                fileIcon = QApplication::style()->standardIcon(QStyle::SP_FileDialogListView);
            else
                fileIcon = QApplication::style()->standardIcon(QStyle::SP_FileIcon);
            
            item->setIcon(0, fileIcon);
            item->setForeground(0, QBrush(QColor(ModernTheme::TEXT_PRIMARY)));
            
            // Добавляем размер файла
            QString sizeStr;
            qint64 size = fi.size();
            if (size < 1024) sizeStr = QString("%1 Б").arg(size);
            else if (size < 1024 * 1024) sizeStr = QString("%1 КБ").arg(size / 1024.0, 0, 'f', 1);
            else sizeStr = QString("%1 МБ").arg(size / (1024.0 * 1024.0), 0, 'f', 1);
            
            item->setText(1, sizeStr);
            item->setForeground(1, QBrush(QColor(ModernTheme::TEXT_SECONDARY)));
        }
        
        return item;
    }
    
    return nullptr;
}

void MainWindow::browseFolder()
{
    QString path = QFileDialog::getExistingDirectory(this, "Выберите папку", 
                                                     t5Path->text(), 
                                                     QFileDialog::ShowDirsOnly);
    if (!path.isEmpty()) {
        t5Path->setText(path);
        runTask5();
    }
}

QWidget* MainWindow::createTask5Widget()
{
    QWidget *w = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(24, 20, 24, 20);
    lay->setSpacing(18);

    // Описание
    auto *desc = createCard("Обход файловой системы", "📁");
    auto *dLay = new QVBoxLayout(desc);
    QLabel *info = new QLabel(
        "<span style='color:" + ModernTheme::TEXT_SECONDARY + ";'>Рекурсивный обход директории с построением дерева файлов.</span><br>"
        "<span style='color:" + ModernTheme::WARNING + ";'>Алгоритм:</span> DFS (Depth-First Search) — обход в глубину."
    );
    info->setWordWrap(true);
    info->setStyleSheet("font-size: 14px; line-height: 1.6;");
    dLay->addWidget(info);
    lay->addWidget(desc);

    // Ввод пути
    auto *inputCard = createCard("Путь к директории", "⌨️");
    auto *iLay = new QHBoxLayout(inputCard);
    
    t5Path = createInputField("/home/user или C:/Users");
    t5Path->setText(QDir::homePath());
    
    auto *browseBtn = createActionButton("📂 Обзор", "secondary");
    connect(browseBtn, &QPushButton::clicked, this, &MainWindow::browseFolder);
    
    auto *scanBtn = createActionButton("▶ Сканировать", "primary");
    connect(scanBtn, &QPushButton::clicked, this, &MainWindow::runTask5);
    
    iLay->addWidget(t5Path, 1);
    iLay->addWidget(browseBtn);
    iLay->addWidget(scanBtn);
    lay->addWidget(inputCard);

    // Дерево и вывод (сплиттер)
    auto *contentCard = createCard("Структура файлов", "🌳");
    auto *cLay = new QVBoxLayout(contentCard);
    
    auto *splitter = new QSplitter(Qt::Vertical);
    
    // Дерево
    t5Tree = new QTreeWidget;
    t5Tree->setHeaderLabels(QStringList() << "Имя" << "Размер");
    t5Tree->setColumnWidth(0, 400);
    t5Tree->setMinimumHeight(250);
    t5Tree->setAnimated(true);
    t5Tree->setExpandsOnDoubleClick(true);
    
    // Консоль
    t5Out = createOutputConsole();
    t5Out->setMaximumHeight(150);
    printHeader(t5Out, "File System Scanner", "Задание №5");
    
    splitter->addWidget(t5Tree);
    splitter->addWidget(t5Out);
    splitter->setStretchFactor(0, 3);
    splitter->setStretchFactor(1, 1);
    
    cLay->addWidget(splitter);
    lay->addWidget(contentCard);

    lay->addStretch();
    w->setLayout(lay);
    return w;
}

void MainWindow::runTask5()
{
    QString path = t5Path->text();
    printHeader(t5Out, "File System Scanner", "Задание №5");
    
    if (!QDir(path).exists()) {
        printLine(t5Out, QString("✗ Ошибка: директория '%1' не найдена.").arg(path), ModernTheme::ERROR_CLR);
        return;
    }
    
    t5Tree->clear();
    
    // Корневой элемент
    auto *rootItem = new QTreeWidgetItem(t5Tree);
    QFileInfo rootFi(path);
    rootItem->setText(0, rootFi.fileName().isEmpty() ? rootFi.absolutePath() : rootFi.fileName());
    rootItem->setIcon(0, QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon));
    rootItem->setForeground(0, QBrush(QColor(ModernTheme::ACCENT_PRIMARY)));
    rootItem->setFont(0, QFont("Segoe UI", 13, QFont::Bold));
    rootItem->setExpanded(true);
    
    // Считаем файлы и папки
    int files = 0, folders = 0;
    countItems(path, files, folders);
    
    printKV(t5Out, "Путь:           ", path, ModernTheme::ACCENT_SECOND);
    printSep(t5Out);
    printKV(t5Out, "Найдено файлов:  ", QString::number(files), ModernTheme::SUCCESS);
    printKV(t5Out, "Найдено папок:   ", QString::number(folders), ModernTheme::INFO);
    printKV(t5Out, "Всего объектов:  ", QString::number(files + folders), ModernTheme::WARNING);
    
    // Строим дерево
    t5Tree->setUpdatesEnabled(false);
    QTreeWidgetItem *children = buildTree(path);
    if (children) {
        while (QTreeWidgetItem *child = children->takeChild(0)) {
            rootItem->addChild(child);
        }
        delete children;
    }
    t5Tree->setUpdatesEnabled(true);
    
    scrollBottom(t5Out);
    statusBarWidget->showMessage(QString("  ✓ Сканировано: %1 файлов, %2 папок").arg(files).arg(folders));
}
