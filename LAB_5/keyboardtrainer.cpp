#include "keyboardtrainer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QFontMetrics>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QKeyEvent>
#include <QStyle>
#include <QRegularExpression>

// ================== Внутренний виджет для отрисовки слов ==================
class GameWidget : public QWidget {
public:
    explicit GameWidget(QWidget *parent, QList<KeyboardTrainer::Word> *words)
        : QWidget(parent), m_words(words)
    {
        setMinimumHeight(360);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillRect(rect(), QColor("#0f0f1a"));

        QFont font("Consolas", 18, QFont::Bold);
        painter.setFont(font);

        for (const auto &w : *m_words) {
            painter.setPen(w.highlighted ? QColor("#00ff88") : QColor("#cccccc"));
            painter.drawText(QPointF(w.x, w.y), w.text);
        }
    }

private:
    QList<KeyboardTrainer::Word> *m_words;
};

// ====================== KeyboardTrainer ======================
KeyboardTrainer::KeyboardTrainer(QWidget *parent)
    : QWidget(parent), rng(QRandomGenerator::global()->generate())
{
    englishWords = {"the", "quick", "brown", "fox", "jumps", "over", "lazy", "dog",
                    "hello", "world", "keyboard", "trainer", "practice", "typing", "speed",
                    "programming", "computer", "software", "developer", "algorithm", "data",
                    "function", "variable", "compile", "debug", "error", "exception", "thread"};
    russianWords = {"привет", "мир", "клавиатура", "тренажёр", "практика", "скорость",
                    "быстрая", "лиса", "прыгает", "через", "ленивого", "пса",
                    "программирование", "компьютер", "разработка", "алгоритм", "данные"};

    setupUI();

    // Создаём таймеры ДО вызова restartTraining()
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &KeyboardTrainer::gameLoop);
    gameTimer->start(TICK_INTERVAL);

    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &KeyboardTrainer::spawnWord);

    statsTimer = new QTimer(this);
    connect(statsTimer, &QTimer::timeout, this, &KeyboardTrainer::updateStats);
    statsTimer->start(1000);

    // Теперь можно безопасно запустить игру
    restartTraining();

    setFocusPolicy(Qt::StrongFocus);
}

void KeyboardTrainer::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // --- Верхняя панель управления ---
    auto *topLayout = new QHBoxLayout;
    languageCombo = new QComboBox;
    languageCombo->addItem("🇬🇧 English");
    languageCombo->addItem("🇷🇺 Русский");
    languageCombo->setStyleSheet(
        "QComboBox { background: #2a2a3a; color: #fff; border: 1px solid #4a4a6a; "
        "padding: 6px 14px; border-radius: 6px; font-weight: bold; }"
        "QComboBox::drop-down { border: none; } "
        "QComboBox::down-arrow { image:none; border-left:5px solid transparent; "
        "border-right:5px solid transparent; border-top:6px solid #fff; margin-right:8px; }");
    connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &KeyboardTrainer::onLanguageChanged);
    topLayout->addWidget(languageCombo);

    restartBtn = new QPushButton("🔄 Заново");
    restartBtn->setStyleSheet(
        "QPushButton { background: #3d3d5c; color: #fff; border: none; "
        "padding: 6px 20px; border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { background: #4a4a6a; }");
    connect(restartBtn, &QPushButton::clicked, this, &KeyboardTrainer::restartTraining);
    topLayout->addWidget(restartBtn);

    loadFileBtn = new QPushButton("📁 Загрузить файл");
    loadFileBtn->setStyleSheet(
        "QPushButton { background: #3d3d5c; color: #fff; border: none; "
        "padding: 6px 20px; border-radius: 6px; font-weight: bold; }"
        "QPushButton:hover { background: #4a4a6a; }");
    connect(loadFileBtn, &QPushButton::clicked, this, &KeyboardTrainer::loadTextFromFile);
    topLayout->addWidget(loadFileBtn);

    topLayout->addStretch();
    mainLayout->addLayout(topLayout);

    // --- Статистика ---
    auto *statsLayout = new QHBoxLayout;
    statsLayout->setSpacing(24);

    timerLabel = new QLabel("⏱️ 0:00");
    timerLabel->setStyleSheet("font-size: 18px; color: #00ff88; font-weight: bold;");
    statsLayout->addWidget(timerLabel);

    wpmLabel = new QLabel("⚡ 0 WPM");
    wpmLabel->setStyleSheet("font-size: 18px; color: #ffaa00; font-weight: bold;");
    statsLayout->addWidget(wpmLabel);

    accuracyLabel = new QLabel("🎯 100%");
    accuracyLabel->setStyleSheet("font-size: 18px; color: #00aaff; font-weight: bold;");
    statsLayout->addWidget(accuracyLabel);

    scoreLabel = new QLabel("🏆 0 слов");
    scoreLabel->setStyleSheet("font-size: 18px; color: #ff55aa; font-weight: bold;");
    statsLayout->addWidget(scoreLabel);

    statsLayout->addStretch();
    mainLayout->addLayout(statsLayout);

    // --- Игровое поле (падающие слова) ---
    gameField = new GameWidget(this, &words);
    gameField->setStyleSheet("background: #0f0f1a; border: 2px solid #2a2a3a; border-radius: 8px;");
    mainLayout->addWidget(gameField, 1);

    // --- Поле ввода ---
    inputField = new QLineEdit;
    inputField->setPlaceholderText("Начните вводить слово...");
    inputField->setStyleSheet(
        "QLineEdit { background: #2a2a3a; color: #fff; border: 2px solid #4a4a6a; "
        "border-radius: 8px; padding: 10px 16px; font-size: 18px; font-weight: bold; }"
        "QLineEdit:focus { border-color: #5a5a8a; }");
    inputField->setMaxLength(50);
    connect(inputField, &QLineEdit::textChanged, this, &KeyboardTrainer::checkInput);
    mainLayout->addWidget(inputField);

    setStyleSheet("background: #1a1a2e;");
}

void KeyboardTrainer::restartTraining()
{
    words.clear();
    totalCorrectChars = 0;
    totalMissedChars = 0;
    correctWords = 0;
    missedWords = 0;
    wpm = 0.0;
    accuracy = 100.0;

    inputField->clear();
    inputField->setStyleSheet(
        "QLineEdit { background: #2a2a3a; color: #fff; border: 2px solid #4a4a6a; "
        "border-radius: 8px; padding: 10px 16px; font-size: 18px; font-weight: bold; }");
    elapsedTimer.invalidate();
    elapsedTimer.start();
    spawnTimer->start(SPAWN_INTERVAL);
    gameField->update();
    updateStats();
    setFocus();
}

void KeyboardTrainer::onLanguageChanged(int index)
{
    currentLanguage = (index == 0) ? "English" : "Russian";
    restartTraining();
}

void KeyboardTrainer::loadTextFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть текстовый файл", "", "Text files (*.txt)");
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll().simplified();
    file.close();

    if (content.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Файл не содержит текста.");
        return;
    }

    QStringList newWords = content.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (newWords.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Файл не содержит слов.");
        return;
    }

    if (currentLanguage == "English" || currentLanguage == "Russian") {
        if (currentLanguage == "English")
            englishWords = newWords;
        else
            russianWords = newWords;
    } else {
        englishWords = newWords;
    }

    restartTraining();
}

void KeyboardTrainer::spawnWord()
{
    const QStringList &dict = (currentLanguage == "English") ? englishWords : russianWords;
    if (dict.isEmpty())
        return;

    QFontMetrics fm(QFont("Consolas", 18, QFont::Bold));
    QString text = dict.at(rng.bounded(dict.size()));

    qreal maxX = qMax(10.0, gameField->width() - fm.horizontalAdvance(text) - 20.0);
    qreal x = 20.0 + rng.bounded(maxX);

    Word w;
    w.text = text;
    w.x = x;
    w.y = -fm.height();
    w.highlighted = false;
    words.append(w);
}

void KeyboardTrainer::gameLoop()
{
    if (!elapsedTimer.isValid())
        return;

    bool needUpdate = false;
    for (auto &w : words) {
        w.y += WORD_SPEED;
        needUpdate = true;
    }

    int fieldHeight = gameField->height();
    for (int i = words.size() - 1; i >= 0; --i) {
        if (words[i].y > fieldHeight) {
            missedWords++;
            totalMissedChars += words[i].text.length();
            words.removeAt(i);
            needUpdate = true;
        }
    }

    if (needUpdate)
        gameField->update();
}

void KeyboardTrainer::checkInput()
{
    QString text = inputField->text();
    if (text.isEmpty()) {
        for (auto &w : words)
            w.highlighted = false;
        inputField->setStyleSheet(
            "QLineEdit { background: #2a2a3a; color: #fff; border: 2px solid #4a4a6a; "
            "border-radius: 8px; padding: 10px 16px; font-size: 18px; font-weight: bold; }");
        gameField->update();
        return;
    }

    bool matchFound = false;
    bool fullMatch = false;
    Word *bestCandidate = nullptr;

    for (auto &w : words) {
        if (w.text.startsWith(text, Qt::CaseInsensitive)) {
            matchFound = true;
            if (!bestCandidate || w.y > bestCandidate->y)
                bestCandidate = &w;
            if (w.text.compare(text, Qt::CaseInsensitive) == 0) {
                fullMatch = true;
                bestCandidate = &w;
                break;
            }
        }
    }

    for (auto &w : words)
        w.highlighted = (&w == bestCandidate);

    if (fullMatch && bestCandidate) {
        // Находим индекс указателя, чтобы безопасно удалить
        int idx = -1;
        for (int i = 0; i < words.size(); ++i) {
            if (&words[i] == bestCandidate) {
                idx = i;
                break;
            }
        }
        if (idx >= 0) {
            correctWords++;
            totalCorrectChars += words[idx].text.length();
            words.removeAt(idx);
            inputField->clear();
            gameField->update();
            updateStats();
            return;
        }
    }

    if (matchFound) {
        inputField->setStyleSheet(
            "QLineEdit { background: #1a3a2a; color: #00ff88; border: 2px solid #00ff88; "
            "border-radius: 8px; padding: 10px 16px; font-size: 18px; font-weight: bold; }");
    } else {
        inputField->setStyleSheet(
            "QLineEdit { background: #3a1a1a; color: #ff4444; border: 2px solid #ff4444; "
            "border-radius: 8px; padding: 10px 16px; font-size: 18px; font-weight: bold; }");
    }

    gameField->update();
}

void KeyboardTrainer::updateStats()
{
    if (!elapsedTimer.isValid())
        return;

    qint64 ms = elapsedTimer.elapsed();
    double minutes = ms / 60000.0;
    if (minutes < 0.01) minutes = 0.01;

    wpm = (totalCorrectChars / 5.0) / minutes;

    int totalChars = totalCorrectChars + totalMissedChars;
    accuracy = (totalChars > 0) ? (totalCorrectChars * 100.0 / totalChars) : 100.0;

    int mins = static_cast<int>(minutes);
    int secs = static_cast<int>((minutes - mins) * 60);
    timerLabel->setText(QString("⏱️ %1:%2").arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0')));
    wpmLabel->setText(QString("⚡ %1 WPM").arg(wpm, 0, 'f', 1));
    accuracyLabel->setText(QString("🎯 %1%").arg(accuracy, 0, 'f', 1));
    scoreLabel->setText(QString("🏆 %1 слов").arg(correctWords));
}