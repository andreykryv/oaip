#include "keyboardtrainer.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPainter>
#include <QFontMetrics>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <cmath>

// ═══════════════════════════════════════════════════════════
//  GameWidget
// ═══════════════════════════════════════════════════════════
GameWidget::GameWidget(QWidget *parent, QList<KeyboardTrainer::Word> *words)
    : QWidget(parent), m_words(words)
{
    setMinimumHeight(300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GameWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    // Фон с градиентом
    QLinearGradient bg(0, 0, 0, height());
    bg.setColorAt(0.0, QColor("#080a12"));
    bg.setColorAt(1.0, QColor("#0d0f1e"));
    p.fillRect(rect(), bg);

    // Тонкая сетка
    p.setPen(QPen(QColor(30, 35, 60, 60), 1));
    for (int x = 0; x < width(); x += 40)
        p.drawLine(x, 0, x, height());
    for (int y = 0; y < height(); y += 40)
        p.drawLine(0, y, width(), y);

    // Линия-опасность внизу
    p.setPen(QPen(QColor("#ef4444"), 2));
    p.setOpacity(0.6);
    p.drawLine(0, height() - 4, width(), height() - 4);
    p.setOpacity(1.0);

    // GAME OVER overlay
    if (m_gameOver) {
        p.fillRect(rect(), QColor(0, 0, 0, 160));
        QFont gf("Cascadia Code", 36, QFont::Bold);
        p.setFont(gf);
        p.setPen(QColor("#ef4444"));
        p.drawText(rect(), Qt::AlignCenter, "GAME OVER");
        return;
    }

    // Слова
    for (const auto &w : *m_words) {
        QFont f("Cascadia Code", 16, QFont::Bold);
        p.setFont(f);
        QFontMetrics fm(f);

        // Подсвечиваем совпадающую часть
        if (w.highlighted && !m_input.isEmpty()) {
            int match = 0;
            while (match < m_input.length() && match < w.text.length() &&
                   w.text[match].toLower() == m_input[match].toLower())
                match++;

            // Уже введённая часть
            if (match > 0) {
                p.setPen(QColor(Theme::NEON_GREEN));
                p.drawText(QPointF(w.x, w.y), w.text.left(match));
            }
            // Оставшаяся часть
            qreal doneW = fm.horizontalAdvance(w.text.left(match));
            p.setPen(QColor(Theme::NEON_CYAN));
            p.drawText(QPointF(w.x + doneW, w.y), w.text.mid(match));

            // Подчёркивание текущего символа
            if (match < w.text.length()) {
                qreal cx = w.x + doneW;
                qreal cy = w.y + 3;
                int cw = fm.horizontalAdvance(w.text[match]);
                p.setPen(QPen(QColor(Theme::NEON_GREEN), 2));
                p.drawLine(QPointF(cx, cy), QPointF(cx + cw, cy));
            }
        } else {
            p.setPen(w.color);
            p.drawText(QPointF(w.x, w.y), w.text);
        }
    }
}

// ═══════════════════════════════════════════════════════════
//  KeyboardHintWidget
// ═══════════════════════════════════════════════════════════
KeyboardHintWidget::KeyboardHintWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumHeight(100);
    setMaximumHeight(110);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void KeyboardHintWidget::buildLayout()
{
    m_keys.clear();
    QStringList rows = {"1234567890-=", "qwertyuiop[]", "asdfghjkl;'", "zxcvbnm,./"};
    QList<int> counts = {12, 12, 11, 10};
    QList<qreal> offsets = {0, 14, 22, 34}; // отступы рядов

    qreal mg = 4.0, sp = 3.0;
    qreal totalW = width() - 2 * mg;
    qreal rowH = (height() - 2 * mg - 3 * sp) / 4.0;
    qreal keyW = (totalW - (12 - 1) * sp) / 12.0;
    qreal y = mg;

    for (int row = 0; row < 4; ++row) {
        qreal x = mg + offsets[row];
        for (int col = 0; col < counts[row] && col < rows[row].size(); ++col) {
            m_keys.append({rows[row].at(col), QRectF(x, y, keyW, rowH), false});
            x += keyW + sp;
        }
        y += rowH + sp;
    }
    // Пробел
    qreal spW = totalW * 0.4;
    qreal spX = mg + (totalW - spW) / 2.0;
    m_keys.append({QChar(' '), QRectF(spX, y, spW, rowH * 0.85), false});
}

void KeyboardHintWidget::resizeEvent(QResizeEvent *) { buildLayout(); }

void KeyboardHintWidget::setNextChar(QChar c)
{
    m_active = c.toLower();
    for (auto &k : m_keys)
        k.active = (k.ch == m_active);
    update();
}

void KeyboardHintWidget::clearHint()
{
    m_active = QChar();
    for (auto &k : m_keys) k.active = false;
    update();
}

void KeyboardHintWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), QColor("#0a0c18"));

    QFont f("Cascadia Code", 9);
    p.setFont(f);

    for (const auto &k : m_keys) {
        QColor fill   = k.active ? QColor(Theme::NEON_GREEN)  : QColor("#141828");
        QColor border = k.active ? QColor(Theme::NEON_GREEN)  : QColor("#1e2238");
        QColor text   = k.active ? QColor("#000000")           : QColor(Theme::TEXT_SEC);

        p.setPen(QPen(border, 1));
        p.setBrush(fill);
        p.drawRoundedRect(k.rect, 3, 3);

        p.setPen(text);
        QString lbl = (k.ch == ' ') ? "SPC" : QString(k.ch.toUpper());
        p.drawText(k.rect, Qt::AlignCenter, lbl);
    }
}

// ═══════════════════════════════════════════════════════════
//  KeyboardTrainer
// ═══════════════════════════════════════════════════════════
KeyboardTrainer::KeyboardTrainer(QWidget *parent)
    : QWidget(parent), rng(QRandomGenerator::global()->generate())
{
    englishWords = {
        "the","quick","brown","fox","jumps","over","lazy","dog",
        "hello","world","keyboard","trainer","practice","typing","speed",
        "programming","computer","software","developer","algorithm",
        "function","variable","compile","debug","error","exception",
        "array","pointer","struct","class","template","iterator",
        "memory","cache","thread","mutex","lock","async","await",
        "binary","search","sort","tree","graph","hash","stack","queue"
    };
    russianWords = {
        "привет","мир","клавиатура","тренажёр","практика","скорость",
        "быстрый","алгоритм","данные","память","функция","переменная",
        "компилятор","отладка","программа","структура","указатель",
        "массив","строка","символ","число","цикл","условие","класс",
        "метод","объект","наследование","полиморфизм","шаблон","итератор"
    };

    loadPersonalBest();
    setupUI();

    gameTimer  = new QTimer(this);
    spawnTimer = new QTimer(this);
    statsTimer = new QTimer(this);

    connect(gameTimer,  &QTimer::timeout, this, &KeyboardTrainer::gameLoop);
    connect(spawnTimer, &QTimer::timeout, this, &KeyboardTrainer::spawnWord);
    connect(statsTimer, &QTimer::timeout, this, &KeyboardTrainer::updateStats);

    gameTimer->start(TICK_INTERVAL);
    statsTimer->start(500);

    restartTraining();
    setFocusPolicy(Qt::StrongFocus);
}

// ─── UI ───────────────────────────────────────────────────
void KeyboardTrainer::setupUI()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(12, 10, 12, 10);
    root->setSpacing(8);

    // ── Панель управления ─────────────────────────────────
    auto *ctrlFrame = new QFrame;
    ctrlFrame->setObjectName("ctrlFrame");
    ctrlFrame->setStyleSheet(QString("QFrame#ctrlFrame{"
        "background:%1;border:1px solid %2;border-radius:8px;}")
        .arg(Theme::BG_CARD, Theme::BORDER));
    auto *ctrlLay = new QHBoxLayout(ctrlFrame);
    ctrlLay->setContentsMargins(12, 8, 12, 8);
    ctrlLay->setSpacing(10);

    // Язык
    languageCombo = new QComboBox;
    languageCombo->addItems({"🇬🇧  English", "🇷🇺  Русский"});
    languageCombo->setFixedWidth(130);
    connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &KeyboardTrainer::onLanguageChanged);
    ctrlLay->addWidget(languageCombo);

    // Сложность
    QLabel *diffLbl = new QLabel("Сложность:");
    diffLbl->setStyleSheet(QString("color:%1;font-size:12px;").arg(Theme::TEXT_SEC));
    ctrlLay->addWidget(diffLbl);

    diffCombo = new QComboBox;
    diffCombo->addItems({"🟢  Легко", "🟡  Нормально", "🔴  Сложно", "💀  Безумие"});
    diffCombo->setCurrentIndex(1);
    diffCombo->setFixedWidth(140);
    connect(diffCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &KeyboardTrainer::onDifficultyChanged);
    ctrlLay->addWidget(diffCombo);

    ctrlLay->addStretch();

    loadFileBtn = new QPushButton("📁  Файл");
    loadFileBtn->setFixedWidth(90);
    connect(loadFileBtn, &QPushButton::clicked, this, &KeyboardTrainer::loadTextFromFile);
    ctrlLay->addWidget(loadFileBtn);

    restartBtn = new QPushButton("🔄  Заново");
    restartBtn->setFixedWidth(100);
    restartBtn->setStyleSheet(
        QString("QPushButton{background:%1;color:%2;border:1px solid %3;"
                "border-radius:7px;font-weight:700;padding:6px 12px;}"
                "QPushButton:hover{background:%4;border-color:%5;}")
        .arg(Theme::BG_SURFACE, Theme::NEON_GREEN, Theme::NEON_GREEN,
             Theme::BORDER_LIT, Theme::NEON_GREEN));
    connect(restartBtn, &QPushButton::clicked, this, &KeyboardTrainer::restartTraining);
    ctrlLay->addWidget(restartBtn);

    root->addWidget(ctrlFrame);

    // ── Статистика ────────────────────────────────────────
    statsBar = new QFrame;
    statsBar->setStyleSheet(QString("background:%1;border:1px solid %2;border-radius:8px;")
        .arg(Theme::BG_CARD, Theme::BORDER));
    auto *statsLay = new QHBoxLayout(statsBar);
    statsLay->setContentsMargins(14, 6, 14, 6);
    statsLay->setSpacing(20);

    auto makeStat = [&](const QString &icon, const QString &color) -> QLabel* {
        auto *l = new QLabel(icon + " --");
        l->setStyleSheet(QString("color:%1;font-size:15px;font-weight:700;"
                                  "font-family:'Cascadia Code','Consolas',monospace;")
                          .arg(color));
        return l;
    };

    timerLabel    = makeStat("⏱", Theme::NEON_CYAN);
    wpmLabel      = makeStat("⚡", Theme::NEON_YELLOW);
    accuracyLabel = makeStat("🎯", "#38bdf8");
    scoreLabel    = makeStat("✅", Theme::NEON_GREEN);
    streakLabel   = makeStat("🔥", "#f97316");
    livesLabel    = makeStat("❤", "#ef4444");
    bestLabel     = makeStat("🏆", Theme::NEON_PURPLE);

    statsLay->addWidget(timerLabel);
    statsLay->addWidget(wpmLabel);
    statsLay->addWidget(accuracyLabel);
    statsLay->addWidget(scoreLabel);
    statsLay->addWidget(streakLabel);
    statsLay->addWidget(livesLabel);
    statsLay->addStretch();
    statsLay->addWidget(bestLabel);

    root->addWidget(statsBar);

    // ── Игровое поле ──────────────────────────────────────
    gameField = new GameWidget(this, &words);
    root->addWidget(gameField, 1);

    // ── Клавиатурная подсказка ────────────────────────────
    QLabel *hintLbl = new QLabel("  Следующая клавиша:");
    hintLbl->setStyleSheet(QString("color:%1;font-size:11px;").arg(Theme::TEXT_DIM));
    root->addWidget(hintLbl);

    kbHint = new KeyboardHintWidget;
    root->addWidget(kbHint);

    // ── Поле ввода ────────────────────────────────────────
    inputField = new QLineEdit;
    inputField->setPlaceholderText("Начните вводить слово — нажмите пробел, если не можете набрать текущее");
    inputField->setStyleSheet(
        QString("QLineEdit{background:%1;color:%2;border:2px solid %3;"
                "border-radius:8px;padding:10px 16px;font-size:17px;"
                "font-family:'Cascadia Code','Consolas',monospace;font-weight:600;}"
                "QLineEdit:focus{border-color:%4;background:%5;}")
        .arg(Theme::BG_INPUT, Theme::TEXT_PRI, Theme::BORDER_LIT,
             Theme::NEON_GREEN, Theme::BG_SURFACE));
    connect(inputField, &QLineEdit::textChanged, this, [this](){
        checkInput();
        gameField->setCurrentInput(inputField->text());
        updateKeyboardHint();
    });
    root->addWidget(inputField);
}

// ─── Генерация случайного слова ───────────────────────────
QString KeyboardTrainer::pickRandomWord() 
{
    const QStringList &dict = (currentLanguage == "English") ? englishWords : russianWords;
    if (dict.isEmpty()) return "word";
    return dict.at(rng.bounded(dict.size()));
}

// ─── Рестарт ─────────────────────────────────────────────
void KeyboardTrainer::restartTraining()
{
    words.clear();
    totalCorrectChars = 0;
    totalMissedChars  = 0;
    correctWords      = 0;
    missedWords       = 0;
    streak            = 0;
    lives             = MAX_LIVES[difficultyLevel];
    isGameOver        = false;
    wpm               = 0.0;
    accuracy          = 100.0;

    inputField->clear();
    inputField->setEnabled(true);
    inputField->setStyleSheet(
        QString("QLineEdit{background:%1;color:%2;border:2px solid %3;"
                "border-radius:8px;padding:10px 16px;font-size:17px;"
                "font-family:'Cascadia Code','Consolas',monospace;font-weight:600;}"
                "QLineEdit:focus{border-color:%4;background:%5;}")
        .arg(Theme::BG_INPUT, Theme::TEXT_PRI, Theme::BORDER_LIT,
             Theme::NEON_GREEN, Theme::BG_SURFACE));

    gameField->setGameOver(false);
    kbHint->clearHint();
    elapsedTimer.start();
    spawnTimer->start(SPAWNS[difficultyLevel]);
    updateLivesDisplay();
    setFocus();
}

// ─── Смена языка ──────────────────────────────────────────
void KeyboardTrainer::onLanguageChanged(int idx)
{
    currentLanguage = (idx == 0) ? "English" : "Russian";
    restartTraining();
}

// ─── Смена сложности ─────────────────────────────────────
void KeyboardTrainer::onDifficultyChanged(int idx)
{
    difficultyLevel = idx;
    spawnTimer->setInterval(SPAWNS[idx]);
    lives = MAX_LIVES[idx];
    updateLivesDisplay();
}

// ─── Загрузка файла ───────────────────────────────────────
void KeyboardTrainer::loadTextFromFile()
{
    QString fn = QFileDialog::getOpenFileName(this, "Открыть текст", "", "*.txt");
    if (fn.isEmpty()) return;

    QFile f(fn);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }
    QStringList ws = QTextStream(&f).readAll()
        .simplified().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    f.close();

    if (ws.isEmpty()) { QMessageBox::warning(this, "Ошибка", "Файл пуст."); return; }
    if (currentLanguage == "English") englishWords = ws;
    else                              russianWords  = ws;
    restartTraining();
}

// ─── Спаун слова ──────────────────────────────────────────
void KeyboardTrainer::spawnWord()
{
    if (isGameOver) return;

    QFont f("Cascadia Code", 16, QFont::Bold);
    QFontMetrics fm(f);
    QString text = pickRandomWord();

    qreal maxX = qMax(10.0, gameField->width() - fm.horizontalAdvance(text) - 20.0);
    qreal x = 20.0 + rng.bounded(maxX < 20 ? 1.0 : maxX);

    // Цвет слова по длине: короткие — светлые, длинные — тёмно-красные
    static const QColor palette[] = {
        QColor("#e2e8f0"), QColor("#94a3b8"),
        QColor("#60a5fa"), QColor("#a78bfa"),
        QColor("#f472b6"), QColor("#fb923c")
    };
    int ci = qMin(text.length() / 2, 5);

    Word w;
    w.text  = text;
    w.x     = x;
    w.y     = -20.0;
    w.color = palette[ci];
    words.append(w);
}

// ─── Игровой цикл ─────────────────────────────────────────
void KeyboardTrainer::gameLoop()
{
    if (!elapsedTimer.isValid() || isGameOver) return;

    qreal speed = SPEEDS[difficultyLevel];
    for (auto &w : words) w.y += speed;

    int h = gameField->height();
    for (int i = words.size() - 1; i >= 0; --i) {
        if (words[i].y > h) {
            if (!words[i].highlighted) {   // слово не было в процессе набора
                missedWords++;
                totalMissedChars += words[i].text.length();
                lives--;
                streak = 0;
                updateLivesDisplay();
                if (lives <= 0) { gameOver(); return; }
            }
            words.removeAt(i);
        }
    }
    gameField->update();
}

// ─── Проверка ввода ───────────────────────────────────────
void KeyboardTrainer::checkInput()
{
    if (isGameOver) return;

    QString text = inputField->text();
    if (text.isEmpty()) {
        for (auto &w : words) w.highlighted = false;
        kbHint->clearHint();
        gameField->update();
        return;
    }

    // Пробел — пропуск текущего слова (штраф)
    if (text == " ") {
        inputField->clear();
        return;
    }

    Word *best = nullptr;
    for (auto &w : words) {
        w.highlighted = false;
        if (w.text.startsWith(text, Qt::CaseInsensitive))
            if (!best || w.y > best->y) best = &w;
    }

    if (best) {
        best->highlighted = true;

        // Полное совпадение
        if (best->text.compare(text, Qt::CaseInsensitive) == 0) {
            correctWords++;
            totalCorrectChars += text.length();
            streak++;
            // Удаляем слово
            for (int i = 0; i < words.size(); ++i)
                if (&words[i] == best) { words.removeAt(i); break; }
            inputField->clear();
            kbHint->clearHint();
            gameField->update();

            // Звуковая обратная связь (цвет)
            inputField->setStyleSheet(
                QString("QLineEdit{background:%1;color:%2;border:2px solid %3;"
                        "border-radius:8px;padding:10px 16px;font-size:17px;"
                        "font-family:'Cascadia Code','Consolas',monospace;font-weight:600;}")
                .arg(Theme::BG_SURFACE, Theme::NEON_GREEN, Theme::NEON_GREEN));
            QTimer::singleShot(200, this, [this](){
                inputField->setStyleSheet(
                    QString("QLineEdit{background:%1;color:%2;border:2px solid %3;"
                            "border-radius:8px;padding:10px 16px;font-size:17px;"
                            "font-family:'Cascadia Code','Consolas',monospace;font-weight:600;}"
                            "QLineEdit:focus{border-color:%4;background:%5;}")
                    .arg(Theme::BG_INPUT, Theme::TEXT_PRI, Theme::BORDER_LIT,
                         Theme::NEON_GREEN, Theme::BG_SURFACE));
            });
            return;
        }

        // Частичное совпадение — зелёная граница
        inputField->setStyleSheet(
            QString("QLineEdit{background:%1;color:%2;border:2px solid %3;"
                    "border-radius:8px;padding:10px 16px;font-size:17px;"
                    "font-family:'Cascadia Code','Consolas',monospace;font-weight:600;}")
            .arg("#0a1f12", Theme::NEON_GREEN, Theme::NEON_GREEN));
    } else {
        // Нет совпадений — красный
        inputField->setStyleSheet(
            QString("QLineEdit{background:%1;color:#ff6b6b;border:2px solid %2;"
                    "border-radius:8px;padding:10px 16px;font-size:17px;"
                    "font-family:'Cascadia Code','Consolas',monospace;font-weight:600;}")
            .arg("#1f0a0a", Theme::ERROR_CLR));
        streak = 0;
    }
    gameField->update();
}

// ─── Обновление подсказки клавиатуры ─────────────────────
void KeyboardTrainer::updateKeyboardHint()
{
    QString text = inputField->text();
    // Ищем ближайшее слово (по вертикали — самое низкое)
    Word *best = nullptr;
    for (auto &w : words) {
        if (w.highlighted || text.isEmpty())
            if (!best || w.y > best->y) best = &w;
    }
    if (!best && !words.isEmpty()) best = &words[0];

    if (best) {
        int pos = text.length();
        if (pos < best->text.length())
            kbHint->setNextChar(best->text[pos]);
        else
            kbHint->clearHint();
    } else {
        kbHint->clearHint();
    }
}

// ─── Game Over ────────────────────────────────────────────
void KeyboardTrainer::gameOver()
{
    isGameOver = true;
    spawnTimer->stop();
    inputField->setEnabled(false);
    gameField->setGameOver(true);

    if (wpm > personalBestWPM) {
        personalBestWPM = wpm;
        savePersonalBest();
    }
}

// ─── Обновление статистики ────────────────────────────────
void KeyboardTrainer::updateStats()
{
    if (!elapsedTimer.isValid()) return;

    qint64 ms = elapsedTimer.elapsed();
    double minutes = qMax(ms / 60000.0, 0.01);

    wpm = (totalCorrectChars / 5.0) / minutes;
    int total = totalCorrectChars + totalMissedChars;
    accuracy  = (total > 0) ? (totalCorrectChars * 100.0 / total) : 100.0;

    int mm = static_cast<int>(minutes);
    int ss = static_cast<int>((minutes - mm) * 60);
    timerLabel->setText(
        QString("⏱  %1:%2")
        .arg(mm, 2, 10, QChar('0'))
        .arg(ss, 2, 10, QChar('0')));

    wpmLabel->setText(QString("⚡  %1 WPM").arg(wpm, 0, 'f', 1));

    QString accColor = accuracy >= 90 ? Theme::NEON_GREEN :
                       accuracy >= 70 ? Theme::NEON_YELLOW : Theme::ERROR_CLR;
    accuracyLabel->setStyleSheet(
        QString("color:%1;font-size:15px;font-weight:700;"
                "font-family:'Cascadia Code','Consolas',monospace;").arg(accColor));
    accuracyLabel->setText(QString("🎯  %1%").arg(accuracy, 0, 'f', 1));

    scoreLabel->setText(QString("✅  %1 слов").arg(correctWords));
    streakLabel->setText(QString("🔥  x%1").arg(streak));
    bestLabel->setText(QString("🏆  %1 WPM").arg(personalBestWPM, 0, 'f', 1));
}

void KeyboardTrainer::updateLivesDisplay()
{
    QString hearts;
    for (int i = 0; i < MAX_LIVES[difficultyLevel]; ++i)
        hearts += (i < lives) ? "❤ " : "🖤 ";
    livesLabel->setText(hearts.trimmed());
}

void KeyboardTrainer::savePersonalBest()
{
    QSettings s("University", "Lab5");
    s.setValue("personalBestWPM", personalBestWPM);
}

void KeyboardTrainer::loadPersonalBest()
{
    QSettings s("University", "Lab5");
    personalBestWPM = s.value("personalBestWPM", 0.0).toDouble();
}