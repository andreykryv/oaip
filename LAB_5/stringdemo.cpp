#include "stringdemo.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QScrollArea>
#include <QDateTime>
#include <algorithm>
#include <QScrollBar>          // добавлено

// ═══════════════════════════════════════════════════════════
//  StringVisualizerWidget — строка как ячейки
// ═══════════════════════════════════════════════════════════
StringVisualizerWidget::StringVisualizerWidget(const QString &label, QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet(QString("QFrame{background:%1;border:1px solid %2;border-radius:8px;}")
        .arg(Theme::BG_DEEP, Theme::BORDER));
    setMinimumHeight(68);

    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(8, 6, 8, 6);
    lay->setSpacing(4);

    m_label = new QLabel(label);
    m_label->setStyleSheet(
        QString("color:%1;font-size:11px;font-weight:700;").arg(Theme::TEXT_SEC));
    lay->addWidget(m_label);

    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setFixedHeight(38);
    scroll->setStyleSheet("background:transparent;");

    m_cells = new QWidget;
    m_cellLayout = new QHBoxLayout(m_cells);
    m_cellLayout->setContentsMargins(0, 0, 0, 0);
    m_cellLayout->setSpacing(2);
    m_cellLayout->addStretch();

    scroll->setWidget(m_cells);
    lay->addWidget(scroll);
}

void StringVisualizerWidget::setString(const QString &s)
{
    // Очищаем
    QLayoutItem *item;
    while ((item = m_cellLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }

    static const QString colors[] = {
        Theme::NEON_GREEN, Theme::NEON_CYAN, Theme::NEON_PURPLE,
        Theme::NEON_PINK,  Theme::NEON_YELLOW
    };
    int ci = 0;

    for (int i = 0; i < s.length() && i < 80; ++i) {
        QChar c = s[i];
        auto *cell = new QFrame;
        cell->setFixedSize(28, 30);
        cell->setStyleSheet(
            QString("QFrame{background:%1;border:1px solid %2;"
                    "border-radius:4px;}")
            .arg(Theme::BG_SURFACE, colors[ci % 5]));

        auto *cellLay = new QVBoxLayout(cell);
        cellLay->setContentsMargins(0, 0, 0, 0);
        cellLay->setSpacing(0);

        // Символ
        auto *ch = new QLabel(c.isPrint() ? QString(c) : "·");
        ch->setAlignment(Qt::AlignCenter);
        ch->setStyleSheet(
            QString("color:%1;font-size:13px;font-weight:700;"
                    "font-family:'Cascadia Code','Consolas',monospace;background:transparent;")
            .arg(colors[ci % 5]));
        cellLay->addWidget(ch);

        m_cellLayout->addWidget(cell);
        ++ci;
    }

    // Нулевой терминатор
    if (!s.isEmpty()) {
        auto *termCell = new QFrame;
        termCell->setFixedSize(28, 30);
        termCell->setStyleSheet(
            QString("QFrame{background:%1;border:1px dashed %2;border-radius:4px;}")
            .arg(Theme::BG_CARD, Theme::TEXT_DIM));
        auto *termLbl = new QLabel("\\0");
        termLbl->setAlignment(Qt::AlignCenter);
        termLbl->setStyleSheet(
            QString("color:%1;font-size:10px;font-family:'Cascadia Code',monospace;"
                    "background:transparent;").arg(Theme::TEXT_DIM));
        auto *tl = new QVBoxLayout(termCell);
        tl->setContentsMargins(0, 0, 0, 0);
        tl->addWidget(termLbl);
        m_cellLayout->addWidget(termCell);
    }

    m_cellLayout->addStretch();
    m_label->setText(m_label->text().split(":")[0] +
                     QString(":  \"%1\"  (%2 симв.)").arg(s).arg(s.length()));
}

void StringVisualizerWidget::clear()
{
    QLayoutItem *item;
    while ((item = m_cellLayout->takeAt(0))) {
        delete item->widget();
        delete item;
    }
    m_cellLayout->addStretch();
    m_label->setText(m_label->text().split(":")[0] + ":");
}

// ═══════════════════════════════════════════════════════════
//  StringDemo
// ═══════════════════════════════════════════════════════════
StringDemo::StringDemo(QWidget *parent) : QWidget(parent)
{
    setupDescriptions();

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(10, 8, 10, 8);
    root->setSpacing(8);

    // ── Ввод ──────────────────────────────────────────────
    auto *inputCard = new QGroupBox("Входные данные");
    auto *form = new QFormLayout(inputCard);
    form->setSpacing(8);

    auto makeInput = [&](const QString &ph) -> QLineEdit* {
        auto *le = new QLineEdit;
        le->setPlaceholderText(ph);
        le->setStyleSheet(
            QString("QLineEdit{background:%1;color:%2;border:1px solid %3;"
                    "border-radius:6px;padding:7px 12px;font-size:14px;"
                    "font-family:'Cascadia Code','Consolas',monospace;}"
                    "QLineEdit:focus{border-color:%4;}")
            .arg(Theme::BG_INPUT, Theme::TEXT_PRI, Theme::BORDER, Theme::NEON_GREEN));
        return le;
    };

    input1 = makeInput("Строка 1  (источник / dest)");
    input2 = makeInput("Строка 2  (src / delim / символ для memset)");
    countEdit = makeInput("n  — параметр (для strncpy, memcmp, сдвига...)");

    form->addRow(makeLabel("Строка 1:"), input1);
    form->addRow(makeLabel("Строка 2:"), input2);
    form->addRow(makeLabel("Параметр n:"), countEdit);

    root->addWidget(inputCard);

    // ── Визуализаторы строк ───────────────────────────────
    vis1 = new StringVisualizerWidget("Строка 1");
    vis2 = new StringVisualizerWidget("Строка 2");
    visR = new StringVisualizerWidget("Результат");
    root->addWidget(vis1);
    root->addWidget(vis2);
    root->addWidget(visR);

    // Обновляем визуализаторы при вводе
    connect(input1, &QLineEdit::textChanged, [this](const QString &t){
        vis1->setString(t);
    });
    connect(input2, &QLineEdit::textChanged, [this](const QString &t){
        vis2->setString(t);
    });

    // ── Выбор операции ────────────────────────────────────
    auto *opCard = new QGroupBox("Операция");
    auto *opLay  = new QVBoxLayout(opCard);

    // Описание операции
    descLabel = new QLabel("Выберите операцию из списка");
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet(
        QString("color:%1;font-size:12px;padding:6px 10px;"
                "background:%2;border-radius:6px;border-left:3px solid %3;")
        .arg(Theme::TEXT_SEC, Theme::BG_SURFACE, Theme::NEON_CYAN));
    opLay->addWidget(descLabel);

    auto *opRow = new QHBoxLayout;
    opCombo = new QComboBox;
    QStringList ops = {
        "memcpy", "memmove", "strcpy", "strncpy", "strcat", "strncat",
        "memcmp", "strcmp", "strcoll", "strncmp", "strxfrm",
        "strtok", "memset", "strerror", "strlen"
    };
    opCombo->addItems(ops);
    connect(opCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &StringDemo::onOperationChanged);
    opRow->addWidget(opCombo);

    auto *execBtn = new QPushButton("▶  Выполнить");
    execBtn->setStyleSheet(
        QString("QPushButton{background:%1;color:#000;border:none;"
                "border-radius:7px;padding:8px 20px;font-weight:700;font-size:13px;}"
                "QPushButton:hover{background:#00ff99;}")
        .arg(Theme::NEON_GREEN));
    connect(execBtn, &QPushButton::clicked, this, &StringDemo::performOperation);
    opRow->addWidget(execBtn);

    auto *iterBtn = new QPushButton("↔  Итерировать строку 1");
    connect(iterBtn, &QPushButton::clicked, this, &StringDemo::iterateString);
    opRow->addWidget(iterBtn);

    opLay->addLayout(opRow);
    root->addWidget(opCard);

    // ── Вывод ─────────────────────────────────────────────
    auto *outCard = new QGroupBox("Результат операции");
    auto *outLay  = new QVBoxLayout(outCard);
    output = new QTextEdit;
    output->setReadOnly(true);
    output->setMinimumHeight(100);
    output->setMaximumHeight(160);
    outLay->addWidget(output);
    root->addWidget(outCard);

    onOperationChanged(0);
}

// ─── Вспомогательный QLabel ───────────────────────────────
QLabel* StringDemo::makeLabel(const QString &t)
{
    auto *l = new QLabel(t);
    l->setStyleSheet(QString("color:%1;font-size:12px;font-weight:600;").arg(Theme::TEXT_SEC));
    return l;
}

// ─── Описания операций ────────────────────────────────────
void StringDemo::setupDescriptions()
{
    m_descriptions = {
        {"memcpy",   "<b>memcpy(dest, src, n)</b> — копирует n байт из src в dest. "
                     "Области <b>не должны перекрываться</b>. Возвращает dest."},
        {"memmove",  "<b>memmove(dest, src, n)</b> — копирует n байт, "
                     "<b>безопасно при перекрытии</b>. Возвращает dest."},
        {"strcpy",   "<b>strcpy(dest, src)</b> — копирует строку src в dest "
                     "вместе с нулевым терминатором '\\0'. Возвращает dest."},
        {"strncpy",  "<b>strncpy(dest, src, n)</b> — копирует не более n байт из src. "
                     "Если src короче n — дополняет нулями."},
        {"strcat",   "<b>strcat(dest, src)</b> — <b>присоединяет</b> src к концу dest. "
                     "dest должна иметь место. Возвращает dest."},
        {"strncat",  "<b>strncat(dest, src, n)</b> — присоединяет не более n символов src."},
        {"memcmp",   "<b>memcmp(s1, s2, n)</b> — побайтово сравнивает n байт. "
                     "Возвращает: <0, 0, >0."},
        {"strcmp",   "<b>strcmp(s1, s2)</b> — лексикографическое сравнение строк. "
                     "<0 — s1 < s2;  0 — равны;  >0 — s1 > s2."},
        {"strcoll",  "<b>strcoll(s1, s2)</b> — сравнение с учётом <b>текущей локали</b> (LC_COLLATE)."},
        {"strncmp",  "<b>strncmp(s1, s2, n)</b> — сравнивает не более n символов."},
        {"strxfrm",  "<b>strxfrm(dest, src, n)</b> — преобразует src в форму для "
                     "strcmp с учётом локали."},
        {"strtok",   "<b>strtok(str, delim)</b> — разбивает строку на <b>токены</b> "
                     "по разделителям из delim. Первый вызов: str≠NULL, далее NULL."},
        {"memset",   "<b>memset(s, c, n)</b> — заполняет n байт значением c (0–255). "
                     "Символ заполнения берётся из Строки 2 (первый символ)."},
        {"strerror", "<b>strerror(errnum)</b> — возвращает текстовое описание кода ошибки. "
                     "Параметр n задаёт номер ошибки (0=OK, 1=EPERM, 2=ENOENT...)."},
        {"strlen",   "<b>strlen(s)</b> — длина строки <b>без</b> учёта нулевого терминатора."},
    };
}

void StringDemo::onOperationChanged(int)
{
    QString op = opCombo->currentText();
    descLabel->setText(m_descriptions.value(op, op));
}

// ─── Лог результата ───────────────────────────────────────
void StringDemo::logResult(const QString &op, const QString &result, const QString &color)
{
    QString ts = QDateTime::currentDateTime().toString("hh:mm:ss");
    output->append(
        QString("<span style='color:%1;'>[%2]</span>  "
                "<span style='color:%3;font-weight:700;'>%4</span>  "
                "<span style='color:%5;'>→  %6</span>")
        .arg(Theme::TEXT_DIM, ts, Theme::NEON_CYAN, op, color, result));
    output->verticalScrollBar()->setValue(output->verticalScrollBar()->maximum());
}

// ─── Основная операция ────────────────────────────────────
void StringDemo::performOperation()
{
    String s1(input1->text().toUtf8().constData());
    String s2(input2->text().toUtf8().constData());
    QString op = opCombo->currentText();
    int n = countEdit->text().toInt();

    visR->clear();

    if (op == "strlen") {
        size_t len = s1.strlen(s1.c_str());
        logResult("strlen(s1)", QString("%1  символов  (без \\0)").arg(len));
    }
    else if (op == "strcmp") {
        int r = s1.strcmp(s1.c_str(), s2.c_str());
        QString verdict = (r < 0) ? "s1 < s2" : (r == 0) ? "s1 == s2" : "s1 > s2";
        logResult("strcmp(s1, s2)", QString("%1  (%2)").arg(r).arg(verdict),
                  r == 0 ? Theme::NEON_GREEN : Theme::NEON_YELLOW);
    }
    else if (op == "strcoll") {
        int r = s1.strcoll(s1.c_str(), s2.c_str());
        logResult("strcoll(s1, s2)", QString("%1").arg(r));
    }
    else if (op == "strncmp") {
        if (n <= 0) { logResult("strncmp", "Укажите n > 0", Theme::ERROR_CLR); return; }
        int r = s1.strncmp(s1.c_str(), s2.c_str(), n);
        logResult(QString("strncmp(s1, s2, %1)").arg(n), QString("%1").arg(r));
    }
    else if (op == "memcmp") {
        if (n <= 0) { logResult("memcmp", "Укажите n > 0", Theme::ERROR_CLR); return; }
        int r = s1.memcmp(s1.c_str(), s2.c_str(), std::min(n, (int)std::min(s1.size(), s2.size())));
        logResult(QString("memcmp(s1, s2, %1)").arg(n), QString("%1").arg(r));
    }
    else if (op == "strcpy") {
        size_t sz = s2.size() + 1;
        char *buf = new char[sz + 1]();
        s1.strcpy(buf, s2.c_str());
        String res(buf); delete[] buf;
        visR->setString(QString::fromUtf8(res.c_str()));
        logResult("strcpy(dest, s2)", QString::fromUtf8(res.c_str()));
    }
    else if (op == "strncpy") {
        if (n <= 0) { logResult("strncpy", "Укажите n > 0", Theme::ERROR_CLR); return; }
        char *buf = new char[n + 1]();
        s1.strncpy(buf, s2.c_str(), n);
        String res(buf); delete[] buf;
        visR->setString(QString::fromUtf8(res.c_str()));
        logResult(QString("strncpy(dest, s2, %1)").arg(n), QString::fromUtf8(res.c_str()));
    }
    else if (op == "strcat") {
        size_t sz = s1.size() + s2.size() + 2;
        char *buf = new char[sz]();
        s1.strcpy(buf, s1.c_str());
        s1.strcat(buf, s2.c_str());
        String res(buf); delete[] buf;
        visR->setString(QString::fromUtf8(res.c_str()));
        logResult("strcat(s1, s2)", QString::fromUtf8(res.c_str()));
    }
    else if (op == "strncat") {
        if (n <= 0) { logResult("strncat", "Укажите n > 0", Theme::ERROR_CLR); return; }
        size_t sz = s1.size() + n + 2;
        char *buf = new char[sz]();
        s1.strcpy(buf, s1.c_str());
        s1.strncat(buf, s2.c_str(), n);
        String res(buf); delete[] buf;
        visR->setString(QString::fromUtf8(res.c_str()));
        logResult(QString("strncat(s1, s2, %1)").arg(n), QString::fromUtf8(res.c_str()));
    }
    else if (op == "memcpy") {
        if (n <= 0) { logResult("memcpy", "Укажите n > 0", Theme::ERROR_CLR); return; }
        size_t sz = std::max(s1.size(), (size_t)n) + 1;
        char *buf = new char[sz]();
        s1.strcpy(buf, s1.c_str());
        int copyN = std::min(n, (int)s2.size());
        s1.memcpy(buf, s2.c_str(), copyN);
        String res(buf); delete[] buf;
        visR->setString(QString::fromUtf8(res.c_str()));
        logResult(QString("memcpy(dest, s2, %1)").arg(copyN), QString::fromUtf8(res.c_str()));
    }
    else if (op == "memmove") {
        if (n <= 0) { logResult("memmove", "Укажите n > 0", Theme::ERROR_CLR); return; }
        size_t sz = std::max(s1.size(), (size_t)n) + 1;
        char *buf = new char[sz]();
        s1.strcpy(buf, s1.c_str());
        int copyN = std::min(n, (int)s2.size());
        s1.memmove(buf, s2.c_str(), copyN);
        String res(buf); delete[] buf;
        visR->setString(QString::fromUtf8(res.c_str()));
        logResult(QString("memmove(dest, s2, %1)").arg(copyN), QString::fromUtf8(res.c_str()));
    }
    else if (op == "memset") {
        if (s1.empty()) { logResult("memset", "Строка 1 пуста", Theme::ERROR_CLR); return; }
        if (n <= 0) n = (int)s1.size();
        char fillC = input2->text().isEmpty() ? '\0' : input2->text().at(0).toLatin1();
        size_t sz = s1.size() + 1;
        char *buf = new char[sz]();
        s1.strcpy(buf, s1.c_str());
        s1.memset(buf, (unsigned char)fillC, std::min(n, (int)s1.size()));
        String res(buf); delete[] buf;
        visR->setString(QString::fromUtf8(res.c_str()));
        logResult(QString("memset(s1, '%1', %2)").arg(fillC).arg(n),
                  QString::fromUtf8(res.c_str()));
    }
    else if (op == "strtok") {
        if (s2.empty()) { logResult("strtok", "Задайте разделитель в строке 2", Theme::ERROR_CLR); return; }
        char *buf = new char[s1.size() + 1]();
        s1.strcpy(buf, s1.c_str());
        QStringList tokens;
        char *tok = s1.strtok(buf, s2.c_str());
        while (tok) {
            tokens << QString::fromUtf8(tok);
            tok = s1.strtok(nullptr, s2.c_str());
        }
        delete[] buf;
        logResult(QString("strtok(s1, \"%1\")").arg(input2->text()),
                  tokens.join("  |  "));
    }
    else if (op == "strerror") {
        int errn = (n > 0) ? n : 0;
        char *msg = s1.strerror(errn);
        logResult(QString("strerror(%1)").arg(errn), QString::fromLocal8Bit(msg));
    }
    else if (op == "strxfrm") {
        char buf[256] = {};
        size_t len = s1.strxfrm(buf, s2.c_str(), 255);
        logResult("strxfrm(dest, s2, 255)",
                  QString("преобразовано %1 симв. → \"%2\"").arg(len).arg(buf));
    }
}

// ─── Итерация ─────────────────────────────────────────────
void StringDemo::iterateString()
{
    String s(input1->text().toUtf8().constData());
    if (s.empty()) { logResult("iterate", "Строка 1 пуста", Theme::ERROR_CLR); return; }

    QString chars;
    for (auto it = s.begin(); it != s.end(); ++it)
        chars += QString("<span style='color:%1;font-family:monospace;'>'%2' </span>")
                     .arg(Theme::NEON_CYAN)
                     .arg(QChar(*it));

    logResult(QString("iterate(s1)  len=%1").arg(s.size()), chars);
}