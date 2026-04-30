#include "mainwindow.h"

#include <cstring>
#include <clocale>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cwchar>

// ═══════════════════════════════════════════════════════════════════════
//  КОНСТРУКТОР
// ═══════════════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Лабораторная работа №6  ·  Файлы  ·  C++ / Qt");
    resize(1150, 800);
    setMinimumSize(900, 640);

    applyAppStyle();

    // ── Центральный виджет ──────────────────────────────────────
    QWidget *central = new QWidget(this);
    QVBoxLayout *root = new QVBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Шапка ──────────────────────────────────────────────────
    QFrame *header = new QFrame(central);
    header->setFixedHeight(64);
    header->setObjectName("appHeader");
    QHBoxLayout *hLay = new QHBoxLayout(header);
    hLay->setContentsMargins(24, 0, 24, 0);

    QLabel *icon = new QLabel("📁", header);
    icon->setStyleSheet("font-size:26px; margin-right:8px;");

    QLabel *title = new QLabel("Файлы", header);
    title->setStyleSheet(QString(
        "font-size:22px; font-weight:700; color:%1;").arg(Theme::TEXT_PRI));

    QLabel *sub = new QLabel("Лабораторная работа №6  ·  Вариант 7: Государство  ·  Журнал №7: float '$'", header);
    sub->setStyleSheet(QString(
        "font-size:12px; color:%1; margin-left:12px; margin-top:6px;").arg(Theme::TEXT_SEC));

    QLabel *ver = new QLabel("Qt6 + C++17", header);
    ver->setStyleSheet(QString(
        "font-size:11px; color:%1; background:%2; border:1px solid %3;"
        "border-radius:4px; padding:2px 8px;")
        .arg(Theme::ACCENT2, Theme::BG_CARD, Theme::BORDER));

    hLay->addWidget(icon);
    hLay->addWidget(title);
    hLay->addWidget(sub);
    hLay->addStretch();
    hLay->addWidget(ver);

    // ── Вкладки ────────────────────────────────────────────────
    QTabWidget *tabs = new QTabWidget(central);
    tabs->setObjectName("mainTabs");
    tabs->addTab(createTask1Widget(), "  📐  Задание 1 · Структура  ");
    tabs->addTab(createTask2Widget(), "  💾  Задание 2 · Файлы       ");

    root->addWidget(header);
    root->addWidget(tabs);

    // ── Статус-бар ─────────────────────────────────────────────
    statusBar()->showMessage(
        QString("  Рабочая папка: %1").arg(QDir::currentPath()));
    statusBar()->setStyleSheet(
        QString("background:%1; color:%2; border-top:1px solid %3; font-size:12px;")
        .arg(Theme::BG_CARD, Theme::TEXT_DIM, Theme::BORDER));

    setCentralWidget(central);
}

// ═══════════════════════════════════════════════════════════════════════
//  СТИЛИ
// ═══════════════════════════════════════════════════════════════════════
void MainWindow::applyAppStyle()
{
    qApp->setStyleSheet(QString(R"(
        QMainWindow, QWidget {
            background:%1; color:%2;
            font-family:'Segoe UI','Ubuntu',sans-serif;
        }
        QFrame#appHeader {
            background:%3; border-bottom:1px solid %4;
        }
        QTabWidget#mainTabs::pane { border:none; background:%1; }
        QTabWidget#mainTabs QTabBar::tab {
            background:%3; color:%5; border:none;
            border-right:1px solid %4; padding:12px 20px;
            font-size:13px; font-weight:600; min-width:140px;
        }
        QTabWidget#mainTabs QTabBar::tab:selected {
            background:%1; color:%6; border-bottom:3px solid %7;
        }
        QTabWidget#mainTabs QTabBar::tab:hover:!selected {
            background:%8; color:%2;
        }
        QGroupBox {
            background:%3; border:1px solid %4; border-radius:8px;
            margin-top:16px; padding:14px 16px 16px 16px;
            font-size:13px; font-weight:700; color:%6;
        }
        QGroupBox::title {
            subcontrol-origin:margin; left:14px; top:-8px;
            padding:0 6px; color:%6; background:%3;
        }
        QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox {
            background:%8; color:%2; border:1px solid %4;
            border-radius:6px; padding:7px 11px; font-size:14px;
            font-family:'Cascadia Code','Consolas','Courier New',monospace;
            selection-background-color:%7;
        }
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus {
            border:1px solid %7;
        }
        QSpinBox::up-button, QSpinBox::down-button,
        QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
            background:%4; border-radius:3px; width:18px;
        }
        QComboBox::drop-down { background:%4; border-radius:3px; width:24px; }
        QCheckBox { color:%2; font-size:13px; spacing:8px; }
        QCheckBox::indicator {
            width:18px; height:18px; border:1px solid %4;
            border-radius:4px; background:%8;
        }
        QCheckBox::indicator:checked { background:%7; border-color:%7; }
        QPushButton#runBtn {
            background:qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 %7, stop:1 %9);
            color:#fff; border:none; border-radius:8px;
            padding:10px 28px; font-size:14px; font-weight:700;
        }
        QPushButton#runBtn:hover {
            background:qlineargradient(x1:0,y1:0,x2:1,y2:0,
                stop:0 #9d77f8, stop:1 #0891b2);
        }
        QPushButton#runBtn:pressed { background:%4; }
        QTextEdit {
            background:%10; color:%2; border:1px solid %4;
            border-radius:8px; font-family:'Cascadia Code','Consolas',monospace;
            font-size:13px; padding:12px;
            selection-background-color:%7;
        }
        QListWidget {
            background:%3; border:1px solid %4; border-radius:8px;
            font-size:13px; outline:0;
        }
        QListWidget::item {
            padding:10px 14px; border-radius:6px; color:%5;
        }
        QListWidget::item:selected {
            background:%7; color:#fff; font-weight:700;
        }
        QListWidget::item:hover:!selected { background:%8; color:%2; }
        QScrollBar:vertical {
            background:%3; width:8px; border-radius:4px;
        }
        QScrollBar::handle:vertical {
            background:%4; border-radius:4px; min-height:20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0; }
        QLabel#desc { color:%5; font-size:13px; background:transparent; }
        QLabel#lbl  { color:%6; font-size:13px; font-weight:600; background:transparent; }
        QSplitter::handle { background:%4; width:2px; }
        QStatusBar { font-size:12px; }
    )").arg(
        Theme::BG_MAIN,  Theme::TEXT_PRI, Theme::BG_CARD,  Theme::BORDER,
        Theme::TEXT_SEC, Theme::HIGHLIGHT,Theme::ACCENT,   Theme::BG_INPUT,
        Theme::ACCENT2,  Theme::BG_OUTPUT
    ));
}

// ═══════════════════════════════════════════════════════════════════════
//  UI-ХЕЛПЕРЫ
// ═══════════════════════════════════════════════════════════════════════
QGroupBox* MainWindow::makeCard(const QString &t)
{
    return new QGroupBox(t);
}

QPushButton* MainWindow::makeRunButton(const QString &txt)
{
    auto *b = new QPushButton(txt);
    b->setObjectName("runBtn");
    b->setCursor(Qt::PointingHandCursor);
    return b;
}

QTextEdit* MainWindow::makeOutput()
{
    auto *te = new QTextEdit;
    te->setReadOnly(true);
    te->setMinimumHeight(140);
    return te;
}

void MainWindow::printHeader(QTextEdit *o, const QString &title, const QString &sub)
{
    o->clear();
    o->append(QString(
        "<div style='margin-bottom:4px;'>"
        "<span style='color:%1;font-weight:700;font-size:14px;'>%2</span>"
        "&nbsp;&nbsp;"
        "<span style='color:%3;font-size:11px;'>%4</span>"
        "</div>"
        "<div style='color:%5;font-size:10px;margin-bottom:8px;'>"
        "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━</div>")
        .arg(Theme::ACCENT2, title, Theme::TEXT_DIM, sub, Theme::BORDER));
}

void MainWindow::printLine(QTextEdit *o, const QString &txt, const QString &col)
{
    o->append(QString("<div style='color:%1;'>%2</div>").arg(col, txt));
}

void MainWindow::printKV(QTextEdit *o, const QString &k, const QString &v,
                          const QString &vc)
{
    o->append(QString(
        "<div><span style='color:%1;'>%2</span>"
        "<span style='color:%3;font-weight:700;'>%4</span></div>")
        .arg(Theme::TEXT_SEC, k, vc, v));
}

void MainWindow::printSep(QTextEdit *o)
{
    o->append(QString(
        "<div style='color:%1;font-size:10px;'>"
        "──────────────────────────────────────"
        "</div>").arg(Theme::BORDER));
}

void MainWindow::scrollBottom(QTextEdit *o)
{
    o->verticalScrollBar()->setValue(o->verticalScrollBar()->maximum());
}

// ─── Форматирование структуры ──────────────────────────────────
QString MainWindow::formatStruct(const Gosudarstvo &g, const QString &sep)
{
    QString gdp;
    for (int i = 0; i < 5; i++) {
        if (i > 0) gdp += " ";
        gdp += QString::number(g.gdpHistory[i], 'f', 2);
    }
    return QString::fromUtf8(g.name) + sep +
           QString::number(g.population)   + sep +
           QString::number(g.area, 'f', 1) + sep +
           QString(g.continentCode)        + sep +
           (g.isUNMember ? "Да" : "Нет")  + sep +
           gdp;
}

// ─── Fade-in виджет ────────────────────────────────────────────
void MainWindow::fadeIn(QWidget *w)
{
    auto *eff = new QGraphicsOpacityEffect(w);
    w->setGraphicsEffect(eff);
    auto *anim = new QPropertyAnimation(eff, "opacity", w);
    anim->setDuration(280);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

// Вспомогательный метод: создать QLabel со стилем "lbl"
// (встраивается прямо здесь, чтобы не загромождать .h)
static QLabel* lbl(const QString &txt)
{
    auto *l = new QLabel(txt);
    l->setObjectName("lbl");
    return l;
}

// Вспомогательный метод: создать QLabel со стилем "lbl"
// (объявление перед использованием в makeStructForm)
static QLabel* lbl(const QString &txt);

// ─── Вспомогательный виджет-форма для ввода структуры ─────────
QWidget* MainWindow::makeStructForm(
    QLineEdit **nameEdit,
    QSpinBox  **popSpin,
    QDoubleSpinBox **areaSpin,
    QComboBox **contCombo,
    QCheckBox **unCheck,
    QDoubleSpinBox *gdp[5])
{
    QWidget *w = new QWidget;
    QFormLayout *form = new QFormLayout(w);
    form->setSpacing(10);
    form->setLabelAlignment(Qt::AlignRight);

    // Название
    *nameEdit = new QLineEdit;
    (*nameEdit)->setPlaceholderText("напр. Украина");
    form->addRow(lbl("Название:"), *nameEdit);

    // Население
    *popSpin = new QSpinBox;
    (*popSpin)->setRange(1, 2'000'000);
    (*popSpin)->setSuffix(" тыс. чел.");
    (*popSpin)->setValue(1000);
    form->addRow(lbl("Население:"), *popSpin);

    // Площадь
    *areaSpin = new QDoubleSpinBox;
    (*areaSpin)->setRange(0.01, 20000.0);
    (*areaSpin)->setSuffix(" тыс. кв.км");
    (*areaSpin)->setValue(100.0);
    (*areaSpin)->setDecimals(1);
    form->addRow(lbl("Площадь:"), *areaSpin);

    // Континент
    *contCombo = new QComboBox;
    (*contCombo)->addItems({"E — Европа","A — Азия",
                             "N — С.Америка","S — Ю.Америка",
                             "O — Океания","F — Африка"});
    form->addRow(lbl("Континент:"), *contCombo);

    // Член ООН
    *unCheck = new QCheckBox("Является членом ООН");
    (*unCheck)->setChecked(true);
    form->addRow(lbl("ООН:"), *unCheck);

    // ВВП за 5 лет
    QWidget *gdpW = new QWidget;
    QHBoxLayout *gdpH = new QHBoxLayout(gdpW);
    gdpH->setContentsMargins(0,0,0,0);
    gdpH->setSpacing(6);
    for (int i = 0; i < 5; i++) {
        gdp[i] = new QDoubleSpinBox;
        gdp[i]->setRange(0.0, 50000.0);
        gdp[i]->setSuffix(" млрд$");
        gdp[i]->setValue(100.0 + i * 10.0);
        gdp[i]->setDecimals(2);
        gdp[i]->setFixedWidth(128);
        gdpH->addWidget(gdp[i]);
    }
    form->addRow(lbl("ВВП (5 лет):"), gdpW);

    return w;
}

// Чтение структуры из формы
Gosudarstvo MainWindow::readStructFromForm(
    QLineEdit     *nameEdit,
    QSpinBox      *popSpin,
    QDoubleSpinBox *areaSpin,
    QComboBox     *contCombo,
    QCheckBox     *unCheck,
    QDoubleSpinBox *gdp[5])
{
    Gosudarstvo g{};
    QByteArray ba = nameEdit->text().toUtf8();
    std::strncpy(g.name, ba.constData(), 49);
    g.name[49] = '\0';
    g.population     = popSpin->value();
    g.area           = areaSpin->value();
    // Первый символ выбранного текста (E/A/N/S/O/F)
    QString contText = contCombo->currentText();
    g.continentCode  = contText.isEmpty() ? 'E' : contText.at(0).toLatin1();
    g.isUNMember     = unCheck->isChecked();
    for (int i = 0; i < 5; i++)
        g.gdpHistory[i] = gdp[i]->value();
    return g;
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 1 — СТРУКТУРА «ГОСУДАРСТВО»
// ═══════════════════════════════════════════════════════════════════════
QWidget* MainWindow::createTask1Widget()
{
    // Корневой прокручиваемый виджет
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    QWidget *root = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(root);
    lay->setContentsMargins(20, 16, 20, 20);
    lay->setSpacing(16);

    // ── Описание структуры ─────────────────────────────────────
    auto *infoCard = makeCard("Структура  Gosudarstvo  (Вариант 7)");
    auto *iLay = new QVBoxLayout(infoCard);
    QLabel *infoLbl = new QLabel(
        "<b style='color:#c4b5fd;'>struct Gosudarstvo</b> содержит:<br>"
        "&nbsp;&nbsp;<span style='color:#06b6d4;'>int</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;population&nbsp;&nbsp;&nbsp;&nbsp; // население (тыс. чел.)<br>"
        "&nbsp;&nbsp;<span style='color:#06b6d4;'>double</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;area&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// площадь (тыс. кв.км)<br>"
        "&nbsp;&nbsp;<span style='color:#06b6d4;'>char</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;continentCode&nbsp;&nbsp;// код континента (E/A/N/S/O/F)<br>"
        "&nbsp;&nbsp;<span style='color:#06b6d4;'>bool</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;isUNMember&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// является членом ООН<br>"
        "&nbsp;&nbsp;<span style='color:#06b6d4;'>char</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;name[50]&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;// название государства (статич. строк. массив)<br>"
        "&nbsp;&nbsp;<span style='color:#06b6d4;'>double</span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;gdpHistory[5]&nbsp;&nbsp;// ВВП за 5 лет, млрд $ (статич. числ. массив)");
    infoLbl->setObjectName("desc");
    infoLbl->setWordWrap(true);
    infoLbl->setStyleSheet(
        QString("font-family:'Cascadia Code','Consolas',monospace; font-size:13px;"));
    iLay->addWidget(infoLbl);
    lay->addWidget(infoCard);

    // ── 1. Явная инициализация + поэлементное присваивание ─────
    auto *c1 = makeCard("1.1 · Явная инициализация и поэлементное присваивание");
    auto *c1Lay = new QVBoxLayout(c1);
    QLabel *d1 = new QLabel(
        "Переменная <b>s1</b> инициализируется явной инициализирующей строкой.<br>"
        "Переменная <b>s2</b> — значения присваиваются каждому полю отдельно.");
    d1->setObjectName("desc"); d1->setWordWrap(true);
    c1Lay->addWidget(d1);
    auto *btn1 = makeRunButton("▶  Показать");
    connect(btn1, &QPushButton::clicked, this, &MainWindow::runTask1_explicitAndFields);
    c1Lay->addWidget(btn1);
    lay->addWidget(c1);

    // ── 2. Ввод с клавиатуры ───────────────────────────────────
    auto *c2 = makeCard("1.2 · Инициализация с клавиатуры");
    auto *c2Lay = new QVBoxLayout(c2);
    QLabel *d2 = new QLabel(
        "Переменная <b>s3</b> инициализируется значениями, введёнными пользователем.");
    d2->setObjectName("desc"); d2->setWordWrap(true);
    c2Lay->addWidget(d2);
    c2Lay->addWidget(makeStructForm(&t1i_name,&t1i_pop,&t1i_area,
                                     &t1i_cont,&t1i_un,t1i_gdp));
    auto *btn2 = makeRunButton("▶  Создать s3 и вывести");
    connect(btn2, &QPushButton::clicked, this, &MainWindow::runTask1_userInput);
    c2Lay->addWidget(btn2);
    lay->addWidget(c2);

    // ── 3. Указатель ────────────────────────────────────────────
    auto *c3 = makeCard("1.3 · Инициализация через указатель (ptr->поле)");
    auto *c3Lay = new QVBoxLayout(c3);
    QLabel *d3 = new QLabel(
        "Переменная <b>s4</b> и указатель <b>ptr = &s4</b>. "
        "Поля устанавливаются и читаются исключительно через <b>ptr-&gt;поле</b>.");
    d3->setObjectName("desc"); d3->setWordWrap(true);
    c3Lay->addWidget(d3);
    c3Lay->addWidget(makeStructForm(&t1p_name,&t1p_pop,&t1p_area,
                                     &t1p_cont,&t1p_un,t1p_gdp));
    auto *btn3 = makeRunButton("▶  Создать s4, ptr и вывести через ptr");
    connect(btn3, &QPushButton::clicked, this, &MainWindow::runTask1_pointer);
    c3Lay->addWidget(btn3);
    lay->addWidget(c3);

    // ── 4. Ссылка ───────────────────────────────────────────────
    auto *c4 = makeCard("1.4 · Инициализация через ссылку (ref.поле)");
    auto *c4Lay = new QVBoxLayout(c4);
    QLabel *d4 = new QLabel(
        "Переменная <b>s5</b> и ссылка <b>ref = s5</b>. "
        "Поля устанавливаются и читаются через <b>ref.поле</b>.");
    d4->setObjectName("desc"); d4->setWordWrap(true);
    c4Lay->addWidget(d4);
    c4Lay->addWidget(makeStructForm(&t1r_name,&t1r_pop,&t1r_area,
                                     &t1r_cont,&t1r_un,t1r_gdp));
    auto *btn4 = makeRunButton("▶  Создать s5, ref и вывести через ref");
    connect(btn4, &QPushButton::clicked, this, &MainWindow::runTask1_reference);
    c4Lay->addWidget(btn4);
    lay->addWidget(c4);

    // ── Вывод ───────────────────────────────────────────────────
    auto *outCard = makeCard("Результат");
    auto *oLay = new QVBoxLayout(outCard);
    t1Out = makeOutput();
    printHeader(t1Out, "Структура «Государство»", "Задание 1 · Вариант 7");
    oLay->addWidget(t1Out);
    lay->addWidget(outCard, 1);

    scroll->setWidget(root);
    return scroll;
}

// ─── Слоты задания 1 ───────────────────────────────────────────
void MainWindow::runTask1_explicitAndFields()
{
    printHeader(t1Out, "Задание 1.1 · Явная инициализация + поэлементное присваивание",
                "Структура Государство");

    // ── s1: явная инициализирующая строка ─────────────────────
    Gosudarstvo s1 = {
        145000,           // population
        17098.0,          // area
        'E',              // continentCode (E = Европа/Азия)
        true,             // isUNMember
        "Россия",         // name
        {2.29, 2.08, 1.70, 1.83, 2.24} // gdpHistory
    };

    // ── s2: поля присваиваются отдельно ───────────────────────
    Gosudarstvo s2;
    s2.population    = 44000;
    s2.area          = 603.5;
    s2.continentCode = 'E';
    s2.isUNMember    = true;
    std::strncpy(s2.name, "Украина", 49);
    s2.gdpHistory[0] = 0.18;
    s2.gdpHistory[1] = 0.15;
    s2.gdpHistory[2] = 0.16;
    s2.gdpHistory[3] = 0.18;
    s2.gdpHistory[4] = 0.16;

    const QString sep =
        "<span style='color:" + Theme::TEXT_DIM + ";'> │ </span>";

    printLine(t1Out, "── s1 (явная инициализирующая строка):", Theme::ACCENT2);
    printLine(t1Out,
        "<span style='color:" + Theme::HIGHLIGHT + ";font-weight:700;'>" +
        formatStruct(s1, sep) + "</span>");
    printSep(t1Out);

    printLine(t1Out, "── s2 (поэлементное присваивание):", Theme::ACCENT2);
    printLine(t1Out,
        "<span style='color:" + Theme::SUCCESS + ";font-weight:700;'>" +
        formatStruct(s2, sep) + "</span>");

    scrollBottom(t1Out);
    statusBar()->showMessage("  Задание 1.1 выполнено");
}

void MainWindow::runTask1_userInput()
{
    printHeader(t1Out, "Задание 1.2 · Инициализация с клавиатуры",
                "Структура Государство · s3");

    // ── s3: значения из формы ─────────────────────────────────
    Gosudarstvo s3 = readStructFromForm(
        t1i_name, t1i_pop, t1i_area, t1i_cont, t1i_un, t1i_gdp);

    const QString sep =
        "<span style='color:" + Theme::TEXT_DIM + ";'> │ </span>";

    printLine(t1Out, "── s3 (значения введены пользователем):", Theme::ACCENT2);
    printLine(t1Out,
        "<span style='color:" + Theme::SUCCESS + ";font-weight:700;'>" +
        formatStruct(s3, sep) + "</span>");

    printSep(t1Out);
    printKV(t1Out, "sizeof(Gosudarstvo) = ",
            QString::number(sizeof(Gosudarstvo)) + " байт", Theme::WARNING);

    scrollBottom(t1Out);
    statusBar()->showMessage("  Задание 1.2 выполнено: s3 создана");
}

void MainWindow::runTask1_pointer()
{
    printHeader(t1Out, "Задание 1.3 · Указатель на структуру",
                "Структура Государство · s4 · ptr->поле");

    // ── s4 + ptr ───────────────────────────────────────────────
    Gosudarstvo s4{};
    Gosudarstvo *ptr = &s4;

    // Инициализация ТОЛЬКО через указатель
    QByteArray ba4 = t1p_name->text().toUtf8();
    std::strncpy(ptr->name, ba4.constData(), 49);
    ptr->name[49]      = '\0';
    ptr->population    = t1p_pop->value();
    ptr->area          = t1p_area->value();
    QString ct = t1p_cont->currentText();
    ptr->continentCode = ct.isEmpty() ? 'E' : ct.at(0).toLatin1();
    ptr->isUNMember    = t1p_un->isChecked();
    for (int i = 0; i < 5; i++)
        ptr->gdpHistory[i] = t1p_gdp[i]->value();

    const QString sep =
        "<span style='color:" + Theme::TEXT_DIM + ";'> │ </span>";

    printLine(t1Out,
        "── s4 + <span style='color:" + Theme::ACCENT3 + ";'>Gosudarstvo *ptr = &s4;</span>",
        Theme::TEXT_PRI);
    printLine(t1Out, "── Вывод через <b>ptr-&gt;поле</b>:", Theme::ACCENT2);
    printLine(t1Out,
        "<span style='color:" + Theme::SUCCESS + ";font-weight:700;'>" +
        formatStruct(*ptr, sep) + "</span>");

    printSep(t1Out);
    printLine(t1Out,
        QString("<span style='color:%1;'>ptr</span>"
                " = <span style='color:%2;'>0x%3</span>"
                " &nbsp;→&nbsp; &s4 = <span style='color:%2;'>0x%4</span>")
        .arg(Theme::ACCENT3, Theme::WARNING)
        .arg(reinterpret_cast<quintptr>(ptr), 0, 16)
        .arg(reinterpret_cast<quintptr>(&s4), 0, 16));

    scrollBottom(t1Out);
    statusBar()->showMessage("  Задание 1.3 выполнено: ptr демонстрация");
}

void MainWindow::runTask1_reference()
{
    printHeader(t1Out, "Задание 1.4 · Ссылка на структуру",
                "Структура Государство · s5 · ref.поле");

    // ── s5 + ref ───────────────────────────────────────────────
    Gosudarstvo s5{};
    Gosudarstvo &ref = s5;

    // Инициализация ТОЛЬКО через ссылку
    QByteArray ba5 = t1r_name->text().toUtf8();
    std::strncpy(ref.name, ba5.constData(), 49);
    ref.name[49]      = '\0';
    ref.population    = t1r_pop->value();
    ref.area          = t1r_area->value();
    QString ct = t1r_cont->currentText();
    ref.continentCode = ct.isEmpty() ? 'E' : ct.at(0).toLatin1();
    ref.isUNMember    = t1r_un->isChecked();
    for (int i = 0; i < 5; i++)
        ref.gdpHistory[i] = t1r_gdp[i]->value();

    const QString sep =
        "<span style='color:" + Theme::TEXT_DIM + ";'> │ </span>";

    printLine(t1Out,
        "── s5 + <span style='color:" + Theme::ACCENT3 + ";'>Gosudarstvo &ref = s5;</span>",
        Theme::TEXT_PRI);
    printLine(t1Out, "── Вывод через <b>ref.поле</b>:", Theme::ACCENT2);
    printLine(t1Out,
        "<span style='color:" + Theme::SUCCESS + ";font-weight:700;'>" +
        formatStruct(ref, sep) + "</span>");

    printSep(t1Out);
    printLine(t1Out,
        QString("&s5 = <span style='color:%1;'>0x%2</span>"
                " &nbsp;·&nbsp; &ref = <span style='color:%1;'>0x%3</span>"
                " &nbsp;→&nbsp; адреса совпадают ✓")
        .arg(Theme::WARNING)
        .arg(reinterpret_cast<quintptr>(&s5), 0, 16)
        .arg(reinterpret_cast<quintptr>(&ref), 0, 16));

    scrollBottom(t1Out);
    statusBar()->showMessage("  Задание 1.4 выполнено: ref демонстрация");
}

// ═══════════════════════════════════════════════════════════════════════
//  ЗАДАНИЕ 2 — ФАЙЛЫ
// ═══════════════════════════════════════════════════════════════════════
QWidget* MainWindow::createTask2Widget()
{
    QWidget *w = new QWidget;
    QVBoxLayout *lay = new QVBoxLayout(w);
    lay->setContentsMargins(20, 16, 20, 16);
    lay->setSpacing(12);

    // Заголовок с именами файлов
    QLabel *desc = new QLabel(
        QString("<span style='color:%1;'>Рабочие файлы:</span>  "
                "<span style='color:%2;'>%3</span>  "
                "<span style='color:%4;'>·</span>  "
                "<span style='color:%2;'>%5</span>  "
                "<span style='color:%4;'>·</span>  "
                "<span style='color:%2;'>%6</span>  "
                "<span style='color:%4;'>·</span>  "
                "<span style='color:%2;'>%7</span>  "
                "<span style='color:%4;'>·</span>  "
                "<span style='color:%2;'>%8</span>")
        .arg(Theme::TEXT_DIM, Theme::ACCENT2, Theme::BORDER,
             FILE_TEXT, FILE_ARRAY, FILE_STRUCT_TXT,
             FILE_STRUCT_BIN, FILE_SENTENCES));
    desc->setObjectName("desc");
    lay->addWidget(desc);

    // ── Горизонтальный сплиттер: меню | содержимое ─────────────
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->setHandleWidth(3);

    // ── МЕНЮ ────────────────────────────────────────────────────
    t2Menu = new QListWidget;
    t2Menu->setFixedWidth(230);
    QStringList items = {
        "🏠  Описание",
        "1. Записать текст (wchar)",
        "2. Считать текст (wchar)",
        "3. Записать массив (<<)",
        "4. Считать массив (>>)",
        "5. Запись структуры (текст)",
        "6. Чтение структур (текст)",
        "7. Запись структуры (bin)",
        "8. Чтение структур (bin)",
        "9. Дозаписать предложение",
        "10. Считать предложения",
    };
    for (const QString &it : items)
        t2Menu->addItem(it);
    t2Menu->setCurrentRow(0);
    splitter->addWidget(t2Menu);

    // ── СТОПКА СТРАНИЦ ──────────────────────────────────────────
    t2Stack = new QStackedWidget;
    t2Stack->addWidget(makeT2Page_welcome());
    t2Stack->addWidget(makeT2Page_writeText());
    t2Stack->addWidget(makeT2Page_readText());
    t2Stack->addWidget(makeT2Page_writeArray());
    t2Stack->addWidget(makeT2Page_readArray());
    t2Stack->addWidget(makeT2Page_writeStructTxt());
    t2Stack->addWidget(makeT2Page_readStructsTxt());
    t2Stack->addWidget(makeT2Page_writeStructBin());
    t2Stack->addWidget(makeT2Page_readStructsBin());
    t2Stack->addWidget(makeT2Page_writeSentence());
    t2Stack->addWidget(makeT2Page_readSentences());

    splitter->addWidget(t2Stack);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    connect(t2Menu, &QListWidget::currentRowChanged,
            this,   &MainWindow::t2_menuChanged);

    lay->addWidget(splitter, 1);

    // ── ВЫВОД ───────────────────────────────────────────────────
    auto *outCard = makeCard("📤 Вывод");
    auto *oLay = new QVBoxLayout(outCard);
    t2Out = makeOutput();
    t2Out->setMaximumHeight(220);
    printHeader(t2Out, "Задание 2 · Файлы", "Вариант 7 — Государство  ·  float '$'");
    oLay->addWidget(t2Out);
    lay->addWidget(outCard);

    return w;
}

void MainWindow::t2_menuChanged(int row)
{
    if (row < 0) return;
    t2Stack->setCurrentIndex(row);
    fadeIn(t2Stack->currentWidget());
}

// ─────────────────────────────────────────────────────────────────
//  Страницы Task 2
// ─────────────────────────────────────────────────────────────────
QWidget* MainWindow::makeT2Page_welcome()
{
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Задание 2 · Меню файловых операций");
    auto *cl = new QVBoxLayout(card);
    QLabel *lbl2 = new QLabel(
        "Программа реализует <b>switch-case меню</b> в виде Qt ListWidget.<br>"
        "Выберите пункт меню слева, чтобы выполнить соответствующую операцию.<br><br>"
        "<span style='color:#fbbf24;'>Вариант 7</span> → структура <b>Государство</b><br>"
        "<span style='color:#fbbf24;'>Журнал №7</span> → тип <b>float</b>, разделитель <b>'$'</b><br><br>"
        "Все файлы сохраняются в рабочей директории программы.");
    lbl2->setObjectName("desc"); lbl2->setWordWrap(true);
    cl->addWidget(lbl2);
    l->addWidget(card); l->addStretch();
    return w;
}

QWidget* MainWindow::makeT2Page_writeText()
{
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Операция 1 · Запись текста в файл (wchar_t, посимвольно)");
    auto *cl = new QVBoxLayout(card);
    QLabel *d = new QLabel(
        "Вводится один абзац текста. Сохраняется в <b>FamiliyaLab20.txt</b> "
        "посимвольно (<code>fputwc</code>), тип символа <b>wchar_t</b>.");
    d->setObjectName("desc"); d->setWordWrap(true);
    cl->addWidget(d);
    auto *row = new QHBoxLayout;
    row->addWidget(lbl("Текст:"));
    t2_textInput = new QLineEdit("Государство — это основная форма политической организации общества.");
    row->addWidget(t2_textInput);
    cl->addLayout(row);
    auto *btn = makeRunButton("💾  Записать посимвольно (wchar_t)");
    connect(btn, &QPushButton::clicked, this, &MainWindow::t2_writeText);
    cl->addWidget(btn);
    l->addWidget(card); l->addStretch();
    return w;
}

QWidget* MainWindow::makeT2Page_readText()
{
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Операция 2 · Чтение текста из файла (wchar_t, посимвольно)");
    auto *cl = new QVBoxLayout(card);
    QLabel *d = new QLabel(
        "Посимвольно считывает текст из <b>FamiliyaLab20.txt</b> "
        "(<code>fgetwc</code>, тип <b>wchar_t</b>) и выводит на экран.");
    d->setObjectName("desc"); d->setWordWrap(true);
    cl->addWidget(d);
    auto *btn = makeRunButton("📂  Считать посимвольно (wchar_t)");
    connect(btn, &QPushButton::clicked, this, &MainWindow::t2_readText);
    cl->addWidget(btn);
    l->addWidget(card); l->addStretch();
    return w;
}

QWidget* MainWindow::makeT2Page_writeArray()
{
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Операция 3 · Запись массива float в файл (<<, разделитель '$')");
    auto *cl = new QVBoxLayout(card);
    QLabel *d = new QLabel(
        "Вводится массив <b>float</b>. Сохраняется в <b>FamiliyaLab20_0.txt</b> "
        "потоковым оператором <code>&lt;&lt;</code>, значения разделяются символом <b>'$'</b>.");
    d->setObjectName("desc"); d->setWordWrap(true);
    cl->addWidget(d);
    auto *row = new QHBoxLayout;
    row->addWidget(lbl("Значения float\n(через пробел):"));
    t2_arrInput = new QLineEdit("1.5 2.7 3.14 0.99 42.0 7.77");
    row->addWidget(t2_arrInput);
    cl->addLayout(row);
    auto *btn = makeRunButton("💾  Записать массив (<<) с разделителем '$'");
    connect(btn, &QPushButton::clicked, this, &MainWindow::t2_writeArray);
    cl->addWidget(btn);
    l->addWidget(card); l->addStretch();
    return w;
}

QWidget* MainWindow::makeT2Page_readArray()
{
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Операция 4 · Чтение массива float из файла (>>)");
    auto *cl = new QVBoxLayout(card);
    QLabel *d = new QLabel(
        "Считывает массив <b>float</b> из <b>FamiliyaLab20_0.txt</b> "
        "оператором <code>&gt;&gt;</code>, выводит элементы через пробел.");
    d->setObjectName("desc"); d->setWordWrap(true);
    cl->addWidget(d);
    auto *btn = makeRunButton("📂  Считать массив (>>)");
    connect(btn, &QPushButton::clicked, this, &MainWindow::t2_readArray);
    cl->addWidget(btn);
    l->addWidget(card); l->addStretch();
    return w;
}

QWidget* MainWindow::makeT2Page_writeStructTxt()
{
    QWidget *w = new QWidget;
    QScrollArea *sc = new QScrollArea;
    sc->setWidgetResizable(true); sc->setFrameShape(QFrame::NoFrame);
    QWidget *inner = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(inner);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Операция 5 · Запись структуры в текстовый файл (дозапись)");
    auto *cl = new QVBoxLayout(card);
    QLabel *d = new QLabel(
        "Поля структуры записываются в <b>FamiliyaLab20_1.txt</b> в режиме "
        "дозаписи (<code>ios::app</code>). Каждый запуск добавляет новую запись.");
    d->setObjectName("desc"); d->setWordWrap(true);
    cl->addWidget(d);
    cl->addWidget(makeStructForm(&t2st_name,&t2st_pop,&t2st_area,
                                  &t2st_cont,&t2st_un,t2st_gdp));
    auto *btn = makeRunButton("💾  Дозаписать структуру в текстовый файл");
    connect(btn, &QPushButton::clicked, this, &MainWindow::t2_writeStructText);
    cl->addWidget(btn);
    l->addWidget(card); l->addStretch();
    sc->setWidget(inner);
    QVBoxLayout *wl = new QVBoxLayout(w);
    wl->setContentsMargins(0,0,0,0);
    wl->addWidget(sc);
    return w;
}

QWidget* MainWindow::makeT2Page_readStructsTxt()
{
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Операция 6 · Чтение всех структур из текстового файла");
    auto *cl = new QVBoxLayout(card);
    QLabel *d = new QLabel(
        "Читает все структуры из <b>FamiliyaLab20_1.txt</b> до конца файла (EOF), "
        "выводит поля через <b>'|'</b>.");
    d->setObjectName("desc"); d->setWordWrap(true);
    cl->addWidget(d);
    auto *btn = makeRunButton("📂  Считать все структуры (текст)");
    connect(btn, &QPushButton::clicked, this, &MainWindow::t2_readStructsText);
    cl->addWidget(btn);
    l->addWidget(card); l->addStretch();
    return w;
}

QWidget* MainWindow::makeT2Page_writeStructBin()
{
    QWidget *w = new QWidget;
    QScrollArea *sc = new QScrollArea;
    sc->setWidgetResizable(true); sc->setFrameShape(QFrame::NoFrame);
    QWidget *inner = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(inner);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Операция 7 · Запись структуры в бинарный файл (дозапись)");
    auto *cl = new QVBoxLayout(card);
    QLabel *d = new QLabel(
        "Структура записывается в <b>FamiliyaLab20_2.bin</b> функцией "
        "<code>write((char*)&s, sizeof(Gosudarstvo))</code> в режиме дозаписи.");
    d->setObjectName("desc"); d->setWordWrap(true);
    cl->addWidget(d);
    cl->addWidget(makeStructForm(&t2sb_name,&t2sb_pop,&t2sb_area,
                                  &t2sb_cont,&t2sb_un,t2sb_gdp));
    auto *btn = makeRunButton("💾  Дозаписать структуру в бинарный файл");
    connect(btn, &QPushButton::clicked, this, &MainWindow::t2_writeStructBin);
    cl->addWidget(btn);
    l->addWidget(card); l->addStretch();
    sc->setWidget(inner);
    QVBoxLayout *wl = new QVBoxLayout(w);
    wl->setContentsMargins(0,0,0,0);
    wl->addWidget(sc);
    return w;
}

QWidget* MainWindow::makeT2Page_readStructsBin()
{
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Операция 8 · Чтение всех структур из бинарного файла");
    auto *cl = new QVBoxLayout(card);
    QLabel *d = new QLabel(
        "Читает все структуры из <b>FamiliyaLab20_2.bin</b> функцией "
        "<code>read((char*)&s, sizeof(Gosudarstvo))</code> до EOF, "
        "выводит поля через <b>' ; '</b>.");
    d->setObjectName("desc"); d->setWordWrap(true);
    cl->addWidget(d);
    auto *btn = makeRunButton("📂  Считать все структуры (binary)");
    connect(btn, &QPushButton::clicked, this, &MainWindow::t2_readStructsBin);
    cl->addWidget(btn);
    l->addWidget(card); l->addStretch();
    return w;
}

QWidget* MainWindow::makeT2Page_writeSentence()
{
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Операция 9 · Дозаписать предложение в файл");
    auto *cl = new QVBoxLayout(card);
    QLabel *d = new QLabel(
        "Введённое предложение добавляется как отдельная строка в конец "
        "<b>FamiliyaLab20_3.txt</b>.");
    d->setObjectName("desc"); d->setWordWrap(true);
    cl->addWidget(d);
    auto *row = new QHBoxLayout;
    row->addWidget(lbl("Предложение:"));
    t2_sentenceInput = new QLineEdit("Государство обеспечивает защиту прав граждан.");
    row->addWidget(t2_sentenceInput);
    cl->addLayout(row);
    auto *btn = makeRunButton("💾  Дозаписать предложение");
    connect(btn, &QPushButton::clicked, this, &MainWindow::t2_writeSentence);
    cl->addWidget(btn);
    l->addWidget(card); l->addStretch();
    return w;
}

QWidget* MainWindow::makeT2Page_readSentences()
{
    QWidget *w = new QWidget;
    QVBoxLayout *l = new QVBoxLayout(w);
    l->setContentsMargins(12,12,12,12);
    auto *card = makeCard("Операция 10 · Считать все предложения из файла");
    auto *cl = new QVBoxLayout(card);
    QLabel *d = new QLabel(
        "Построчно считывает все предложения из <b>FamiliyaLab20_3.txt</b> "
        "и выводит на экран.");
    d->setObjectName("desc"); d->setWordWrap(true);
    cl->addWidget(d);
    auto *btn = makeRunButton("📂  Считать все предложения");
    connect(btn, &QPushButton::clicked, this, &MainWindow::t2_readSentences);
    cl->addWidget(btn);
    l->addWidget(card); l->addStretch();
    return w;
}

// ═══════════════════════════════════════════════════════════════════════
//  СЛОТЫ ЗАДАНИЯ 2 — РЕАЛИЗАЦИЯ ФАЙЛОВЫХ ОПЕРАЦИЙ
// ═══════════════════════════════════════════════════════════════════════

// ── Операция 1: запись текста посимвольно (wchar_t) ─────────────
void MainWindow::t2_writeText()
{
    printHeader(t2Out, "Оп. 1 · Запись текста (wchar_t, посимвольно)",
                FILE_TEXT);

    QString text = t2_textInput->text();
    if (text.isEmpty()) {
        printLine(t2Out, "✗ Введите текст!", Theme::ERROR_CLR); return;
    }

    std::string fname = FILE_TEXT.toStdString();
    std::setlocale(LC_ALL, "");

    FILE *fp = std::fopen(fname.c_str(), "w");
    if (!fp) {
        printLine(t2Out, "✗ Не удалось открыть файл для записи!", Theme::ERROR_CLR);
        return;
    }

    // Посимвольная запись через fputwc — каждый wchar_t отдельно
    std::wstring wtext = text.toStdWString();
    int written = 0;
    for (wchar_t wc : wtext) {
        if (std::fputwc(wc, fp) == WEOF) {
            printLine(t2Out, "✗ Ошибка записи символа!", Theme::ERROR_CLR);
            break;
        }
        ++written;
    }
    std::fclose(fp);

    printKV(t2Out, "Файл:          ", FILE_TEXT, Theme::ACCENT2);
    printKV(t2Out, "Записано симв.:", QString::number(written), Theme::SUCCESS);
    printLine(t2Out, "Текст: <b>" + text.toHtmlEscaped() + "</b>", Theme::TEXT_PRI);
    printLine(t2Out, "✓ Запись завершена (wchar_t, посимвольно)", Theme::SUCCESS);

    scrollBottom(t2Out);
    statusBar()->showMessage("  Оп.1 выполнена: " + FILE_TEXT);
}

// ── Операция 2: чтение текста посимвольно (wchar_t) ─────────────
void MainWindow::t2_readText()
{
    printHeader(t2Out, "Оп. 2 · Чтение текста (wchar_t, посимвольно)",
                FILE_TEXT);

    std::string fname = FILE_TEXT.toStdString();
    std::setlocale(LC_ALL, "");

    FILE *fp = std::fopen(fname.c_str(), "r");
    if (!fp) {
        printLine(t2Out, "✗ Файл не найден: " + FILE_TEXT, Theme::ERROR_CLR);
        printLine(t2Out, "  Сначала выполните Операцию 1.", Theme::TEXT_DIM);
        return;
    }

    std::wstring result;
    wint_t wc;
    // Посимвольное чтение через fgetwc
    while ((wc = std::fgetwc(fp)) != WEOF) {
        result += static_cast<wchar_t>(wc);
    }
    std::fclose(fp);

    QString text = QString::fromStdWString(result);
    printKV(t2Out, "Файл:          ", FILE_TEXT, Theme::ACCENT2);
    printKV(t2Out, "Считано симв.: ", QString::number(result.size()), Theme::SUCCESS);
    printLine(t2Out, "Текст: <b>" + text.toHtmlEscaped() + "</b>", Theme::TEXT_PRI);
    printLine(t2Out, "✓ Чтение завершено (fgetwc, посимвольно)", Theme::SUCCESS);

    scrollBottom(t2Out);
    statusBar()->showMessage("  Оп.2 выполнена: считано " +
                             QString::number(result.size()) + " симв.");
}

// ── Операция 3: запись массива float (<<) с разделителем '$' ───
void MainWindow::t2_writeArray()
{
    printHeader(t2Out, "Оп. 3 · Запись массива float (<<, разделитель '$')",
                FILE_ARRAY);

    QString raw = t2_arrInput->text().trimmed();
    if (raw.isEmpty()) {
        printLine(t2Out, "✗ Введите значения!", Theme::ERROR_CLR); return;
    }

    // Парсим float значения
    std::vector<float> arr;
    std::istringstream ss(raw.toStdString());
    float v;
    while (ss >> v) arr.push_back(v);

    if (arr.empty()) {
        printLine(t2Out, "✗ Не удалось распарсить значения!", Theme::ERROR_CLR);
        return;
    }

    std::ofstream f(FILE_ARRAY.toStdString());
    if (!f.is_open()) {
        printLine(t2Out, "✗ Не удалось открыть файл!", Theme::ERROR_CLR); return;
    }

    // Потоковая запись с разделителем ARRAY_SEP='$'
    for (float val : arr) {
        f << val << ARRAY_SEP;
    }
    f << '\n';
    f.close();

    printKV(t2Out, "Файл:            ", FILE_ARRAY, Theme::ACCENT2);
    printKV(t2Out, "Тип элементов:   ", "float", Theme::WARNING);
    printKV(t2Out, "Разделитель:     ", QString("'$'"), Theme::WARNING);
    printKV(t2Out, "Элементов:       ", QString::number(arr.size()), Theme::SUCCESS);

    QString vals;
    for (size_t i = 0; i < arr.size(); i++) {
        if (i) vals += "<span style='color:" + Theme::TEXT_DIM + ";'>$</span>";
        vals += QString("<span style='color:%1;'>%2</span>")
                    .arg(Theme::SUCCESS)
                    .arg(arr[i]);
    }
    printLine(t2Out, "Содержимое: " + vals);
    printLine(t2Out, "✓ Запись завершена (<<)", Theme::SUCCESS);

    scrollBottom(t2Out);
    statusBar()->showMessage("  Оп.3 выполнена: " + FILE_ARRAY);
}

// ── Операция 4: чтение массива float (>>) ──────────────────────
void MainWindow::t2_readArray()
{
    printHeader(t2Out, "Оп. 4 · Чтение массива float (>>, разделитель '$')",
                FILE_ARRAY);

    std::ifstream f(FILE_ARRAY.toStdString());
    if (!f.is_open()) {
        printLine(t2Out, "✗ Файл не найден: " + FILE_ARRAY, Theme::ERROR_CLR);
        printLine(t2Out, "  Сначала выполните Операцию 3.", Theme::TEXT_DIM);
        return;
    }

    std::vector<float> arr;
    float v;
    char sep;
    // Читаем: >> для числа, затем get() для '$'
    while (f >> v) {
        arr.push_back(v);
        if (f.peek() == ARRAY_SEP) f.get(sep);
    }
    f.close();

    printKV(t2Out, "Файл:          ", FILE_ARRAY, Theme::ACCENT2);
    printKV(t2Out, "Считано эл.:   ", QString::number(arr.size()), Theme::SUCCESS);

    QString line;
    for (size_t i = 0; i < arr.size(); i++) {
        if (i) line += " ";
        line += QString("<span style='color:%1;'>%2</span>")
                    .arg(Theme::SUCCESS).arg(arr[i]);
    }
    printLine(t2Out, "Массив: " + line);
    printLine(t2Out, "✓ Чтение завершено (>>)", Theme::SUCCESS);

    scrollBottom(t2Out);
    statusBar()->showMessage("  Оп.4 выполнена: считано " +
                             QString::number(arr.size()) + " эл.");
}

// ── Операция 5: запись структуры в текстовый файл (дозапись) ───
void MainWindow::t2_writeStructText()
{
    printHeader(t2Out, "Оп. 5 · Запись структуры в текстовый файл (дозапись)",
                FILE_STRUCT_TXT);

    Gosudarstvo g = readStructFromForm(
        t2st_name, t2st_pop, t2st_area, t2st_cont, t2st_un, t2st_gdp);

    // Открываем в режиме ДОЗАПИСИ (ios::app)
    std::ofstream f(FILE_STRUCT_TXT.toStdString(), std::ios::app);
    if (!f.is_open()) {
        printLine(t2Out, "✗ Не удалось открыть файл!", Theme::ERROR_CLR); return;
    }

    // Записываем каждое поле на отдельной строке
    f << g.name           << '\n';
    f << g.population     << '\n';
    f << g.area           << '\n';
    f << g.continentCode  << '\n';
    f << (g.isUNMember ? 1 : 0) << '\n';
    for (int i = 0; i < 5; i++) f << g.gdpHistory[i] << ' ';
    f << '\n';
    f << "---\n";    // разделитель между записями
    f.close();

    const QString sep = " <span style='color:" + Theme::TEXT_DIM + ";'>│</span> ";
    printKV(t2Out, "Файл (append): ", FILE_STRUCT_TXT, Theme::ACCENT2);
    printLine(t2Out, formatStruct(g, sep), Theme::SUCCESS);
    printLine(t2Out, "✓ Структура дозаписана (ios::app)", Theme::SUCCESS);

    scrollBottom(t2Out);
    statusBar()->showMessage("  Оп.5 выполнена: дозаписана в " + FILE_STRUCT_TXT);
}

// ── Операция 6: чтение всех структур из текстового файла ───────
void MainWindow::t2_readStructsText()
{
    printHeader(t2Out, "Оп. 6 · Чтение всех структур из текстового файла (до EOF)",
                FILE_STRUCT_TXT);

    std::ifstream f(FILE_STRUCT_TXT.toStdString());
    if (!f.is_open()) {
        printLine(t2Out, "✗ Файл не найден: " + FILE_STRUCT_TXT, Theme::ERROR_CLR);
        printLine(t2Out, "  Сначала выполните Операцию 5.", Theme::TEXT_DIM);
        return;
    }

    int count = 0;
    std::string nameLine;
    // Читаем до EOF — программа не знает количество записей
    while (std::getline(f, nameLine)) {
        if (nameLine == "---" || nameLine.empty()) continue;

        Gosudarstvo g{};
        std::strncpy(g.name, nameLine.c_str(), 49);

        std::string line;
        if (!std::getline(f, line)) break;
        g.population = std::stoi(line);
        if (!std::getline(f, line)) break;
        g.area = std::stod(line);
        if (!std::getline(f, line)) break;
        g.continentCode = line.empty() ? 'E' : line[0];
        if (!std::getline(f, line)) break;
        g.isUNMember = (std::stoi(line) != 0);
        if (!std::getline(f, line)) break;
        std::istringstream gdpSS(line);
        for (int i = 0; i < 5; i++) gdpSS >> g.gdpHistory[i];

        ++count;
        printLine(t2Out,
            QString("[%1] ").arg(count) +
            "<span style='color:" + Theme::ACCENT2 + ";font-weight:700;'>" +
            formatStruct(g, " <span style='color:" +
                            Theme::TEXT_DIM + ";'>|</span> ") +
            "</span>");
    }
    f.close();

    printSep(t2Out);
    printKV(t2Out, "Всего записей: ", QString::number(count),
            count > 0 ? Theme::SUCCESS : Theme::WARNING);
    if (count == 0)
        printLine(t2Out, "  Файл пуст. Выполните Операцию 5 сначала.", Theme::TEXT_DIM);

    scrollBottom(t2Out);
    statusBar()->showMessage("  Оп.6 выполнена: прочитано " +
                             QString::number(count) + " структур");
}

// ── Операция 7: запись структуры в бинарный файл (дозапись) ────
void MainWindow::t2_writeStructBin()
{
    printHeader(t2Out, "Оп. 7 · Запись структуры в бинарный файл (дозапись, write)",
                FILE_STRUCT_BIN);

    Gosudarstvo g = readStructFromForm(
        t2sb_name, t2sb_pop, t2sb_area, t2sb_cont, t2sb_un, t2sb_gdp);

    // Открываем бинарный файл в режиме ДОЗАПИСИ
    std::ofstream f(FILE_STRUCT_BIN.toStdString(),
                    std::ios::binary | std::ios::app);
    if (!f.is_open()) {
        printLine(t2Out, "✗ Не удалось открыть файл!", Theme::ERROR_CLR); return;
    }

    // write((char*)&записываемыйЭлемент, размерВБайтах)
    f.write(reinterpret_cast<const char*>(&g), sizeof(Gosudarstvo));
    f.close();

    const QString sep = " <span style='color:" + Theme::TEXT_DIM + ";'>│</span> ";
    printKV(t2Out, "Файл (binary app): ", FILE_STRUCT_BIN, Theme::ACCENT2);
    printKV(t2Out, "sizeof(Gosudarstvo): ",
            QString::number(sizeof(Gosudarstvo)) + " байт", Theme::WARNING);
    printLine(t2Out, formatStruct(g, sep), Theme::SUCCESS);
    printLine(t2Out, "✓ Структура дозаписана (write, binary)", Theme::SUCCESS);

    scrollBottom(t2Out);
    statusBar()->showMessage("  Оп.7 выполнена: дозаписана в " + FILE_STRUCT_BIN);
}

// ── Операция 8: чтение всех структур из бинарного файла ────────
void MainWindow::t2_readStructsBin()
{
    printHeader(t2Out, "Оп. 8 · Чтение всех структур из бинарного файла (до EOF, read)",
                FILE_STRUCT_BIN);

    std::ifstream f(FILE_STRUCT_BIN.toStdString(), std::ios::binary);
    if (!f.is_open()) {
        printLine(t2Out, "✗ Файл не найден: " + FILE_STRUCT_BIN, Theme::ERROR_CLR);
        printLine(t2Out, "  Сначала выполните Операцию 7.", Theme::TEXT_DIM);
        return;
    }

    int count = 0;
    Gosudarstvo g{};
    // read((char*)&считываемыйЭлемент, размерСчитываемогоЭлементаВБайтах)
    // читаем до тех пор, пока есть что читать (EOF)
    while (f.read(reinterpret_cast<char*>(&g), sizeof(Gosudarstvo))) {
        ++count;
        QString sep = QString(" <span style='color:%1;'> ; </span>").arg(Theme::TEXT_DIM);
        printLine(t2Out,
            QString("[%1] ").arg(count) +
            "<span style='color:" + Theme::SUCCESS + ";font-weight:700;'>" +
            formatStruct(g, sep) + "</span>");
    }
    f.close();

    printSep(t2Out);
    printKV(t2Out, "Всего структур: ", QString::number(count),
            count > 0 ? Theme::SUCCESS : Theme::WARNING);
    if (count == 0)
        printLine(t2Out, "  Файл пуст. Выполните Операцию 7 сначала.", Theme::TEXT_DIM);

    scrollBottom(t2Out);
    statusBar()->showMessage("  Оп.8 выполнена: прочитано " +
                             QString::number(count) + " структур");
}

// ── Операция 9: дозаписать предложение ─────────────────────────
void MainWindow::t2_writeSentence()
{
    printHeader(t2Out, "Оп. 9 · Дозаписать предложение в файл", FILE_SENTENCES);

    QString sentence = t2_sentenceInput->text().trimmed();
    if (sentence.isEmpty()) {
        printLine(t2Out, "✗ Введите предложение!", Theme::ERROR_CLR); return;
    }

    // Режим дозаписи — ios::app
    std::ofstream f(FILE_SENTENCES.toStdString(), std::ios::app);
    if (!f.is_open()) {
        printLine(t2Out, "✗ Не удалось открыть файл!", Theme::ERROR_CLR); return;
    }

    f << sentence.toUtf8().constData() << '\n';
    f.close();

    printKV(t2Out, "Файл (append): ", FILE_SENTENCES, Theme::ACCENT2);
    printLine(t2Out, "Предложение: <b>" + sentence.toHtmlEscaped() + "</b>",
              Theme::SUCCESS);
    printLine(t2Out, "✓ Предложение дозаписано", Theme::SUCCESS);

    scrollBottom(t2Out);
    statusBar()->showMessage("  Оп.9 выполнена: дозаписано в " + FILE_SENTENCES);
}

// ── Операция 10: построчное чтение предложений ─────────────────
void MainWindow::t2_readSentences()
{
    printHeader(t2Out, "Оп. 10 · Считать все предложения из файла (до EOF)",
                FILE_SENTENCES);

    std::ifstream f(FILE_SENTENCES.toStdString());
    if (!f.is_open()) {
        printLine(t2Out, "✗ Файл не найден: " + FILE_SENTENCES, Theme::ERROR_CLR);
        printLine(t2Out, "  Сначала выполните Операцию 9.", Theme::TEXT_DIM);
        return;
    }

    std::string line;
    int count = 0;
    // Построчное чтение до EOF
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        ++count;
        printLine(t2Out,
            QString("<span style='color:%1;'>[%2]</span> "
                    "<span style='color:%3;'>%4</span>")
            .arg(Theme::TEXT_DIM)
            .arg(count)
            .arg(Theme::TEXT_PRI)
            .arg(QString::fromUtf8(line.c_str()).toHtmlEscaped()));
    }
    f.close();

    printSep(t2Out);
    printKV(t2Out, "Всего строк: ", QString::number(count),
            count > 0 ? Theme::SUCCESS : Theme::WARNING);

    scrollBottom(t2Out);
    statusBar()->showMessage("  Оп.10 выполнена: прочитано " +
                             QString::number(count) + " строк");
}
