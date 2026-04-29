#include "keyboardtrainer.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QGridLayout>
#include <QTextStream>
#include <QLocale>
#include <algorithm>
#include <QRegularExpression>

KeyboardTrainer::KeyboardTrainer(QWidget *parent) : QWidget(parent), rng(std::random_device{}())
{
    setupUI();
    setFocusPolicy(Qt::StrongFocus);

    // Заполнение словарей
    englishWords = {"the", "quick", "brown", "fox", "jumps", "over", "lazy", "dog",
                    "hello", "world", "keyboard", "trainer", "practice", "typing", "speed"};
    russianWords = {"привет", "мир", "клавиатура", "тренажёр", "практика", "скорость",
                    "быстрая", "лиса", "прыгает", "через", "ленивого", "пса"};

    // Раскладка QWERTY (кнопка -> символ)
    englishLayout = {
        {"`", "`"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"},
        {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}, {"0", "0"}, {"-", "-"}, {"=", "="},
        {"q", "q"}, {"w", "w"}, {"e", "e"}, {"r", "r"}, {"t", "t"}, {"y", "y"}, {"u", "u"},
        {"i", "i"}, {"o", "o"}, {"p", "p"}, {"[", "["}, {"]", "]"}, {"\\", "\\"},
        {"a", "a"}, {"s", "s"}, {"d", "d"}, {"f", "f"}, {"g", "g"}, {"h", "h"}, {"j", "j"},
        {"k", "k"}, {"l", "l"}, {";", ";"}, {"'", "'"},
        {"z", "z"}, {"x", "x"}, {"c", "c"}, {"v", "v"}, {"b", "b"}, {"n", "n"}, {"m", "m"},
        {",", ","}, {".", "."}, {"/", "/"},
        {"space", " "}
    };

    // Русская ЙЦУКЕН
    russianLayout = {
        {"`", "ё"}, {"1", "1"}, {"2", "2"}, {"3", "3"}, {"4", "4"}, {"5", "5"},
        {"6", "6"}, {"7", "7"}, {"8", "8"}, {"9", "9"}, {"0", "0"}, {"-", "-"}, {"=", "="},
        {"q", "й"}, {"w", "ц"}, {"e", "у"}, {"r", "к"}, {"t", "е"}, {"y", "н"}, {"u", "г"},
        {"i", "ш"}, {"o", "щ"}, {"p", "з"}, {"[", "х"}, {"]", "ъ"}, {"\\", "\\"},
        {"a", "ф"}, {"s", "ы"}, {"d", "в"}, {"f", "а"}, {"g", "п"}, {"h", "р"}, {"j", "о"},
        {"k", "л"}, {"l", "д"}, {";", "ж"}, {"'", "э"},
        {"z", "я"}, {"x", "ч"}, {"c", "с"}, {"v", "м"}, {"b", "и"}, {"n", "т"}, {"m", "ь"},
        {",", "б"}, {".", "ю"}, {"/", "."},
        {"space", " "}
    };

    generateText();
    updateDisplayedText();
    statsTimer = new QTimer(this);
    connect(statsTimer, &QTimer::timeout, this, &KeyboardTrainer::updateStats);
    statsTimer->start(1000);
}

void KeyboardTrainer::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);

    languageCombo = new QComboBox;
    languageCombo->addItem("English");
    languageCombo->addItem("Русский");
    connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &KeyboardTrainer::onLanguageChanged);
    mainLayout->addWidget(languageCombo);

    textDisplay = new QLabel("Здесь будет текст...");
    textDisplay->setWordWrap(true);
    textDisplay->setStyleSheet("font-size: 18pt; background: #2b2b2b; color: white; padding: 10px; border-radius: 8px;");
    mainLayout->addWidget(textDisplay);

    timerLabel = new QLabel("Время: 0 с");
    wpmLabel = new QLabel("WPM: 0");
    auto *statsLayout = new QHBoxLayout;
    statsLayout->addWidget(timerLabel);
    statsLayout->addWidget(wpmLabel);
    mainLayout->addLayout(statsLayout);

    // Кнопка загрузки файла
    loadFileBtn = new QPushButton("Загрузить текст из файла");
    connect(loadFileBtn, &QPushButton::clicked, this, &KeyboardTrainer::loadTextFromFile);
    mainLayout->addWidget(loadFileBtn);

    // Создание клавиатуры
    keyboardWidget = new QWidget;
    auto *grid = new QGridLayout(keyboardWidget);
    grid->setSpacing(4);

    // Ряды кнопок согласно стандартной QWERTY
    QStringList row1 = {"`","1","2","3","4","5","6","7","8","9","0","-","="};
    QStringList row2 = {"q","w","e","r","t","y","u","i","o","p","[","]","\\"};
    QStringList row3 = {"a","s","d","f","g","h","j","k","l",";","'"};
    QStringList row4 = {"z","x","c","v","b","n","m",",",".","/"};
    QStringList spaceRow = {"space"};

    auto addKey = [&](const QString& key, int row, int col){
        QPushButton *btn = new QPushButton;
        btn->setText(key == "space" ? "␣" : key);
        btn->setFixedSize(50, 50);
        btn->setStyleSheet(
            "QPushButton { background-color: #3a3a3a; color: white; border: 2px solid #555; border-radius: 5px; font-weight: bold; }"
            "QPushButton:hover { background-color: #4a4a4a; }"
        );
        // Эффект тени
        auto *shadow = new QGraphicsDropShadowEffect;
        shadow->setBlurRadius(10);
        shadow->setOffset(2,2);
        shadow->setColor(QColor(0,0,0,180));
        btn->setGraphicsEffect(shadow);
        keyButtons[key] = btn;
        grid->addWidget(btn, row, col);
    };

    for (int i = 0; i < row1.size(); ++i) addKey(row1[i], 0, i);
    for (int i = 0; i < row2.size(); ++i) addKey(row2[i], 1, i);
    // Ряд 3 с отступом
    for (int i = 0; i < row3.size(); ++i) addKey(row3[i], 2, i+1);
    for (int i = 0; i < row4.size(); ++i) addKey(row4[i], 3, i+1);
    addKey("space", 4, 5);

    mainLayout->addWidget(keyboardWidget);
    setStyleSheet("background-color: #1e1e1e; color: #cccccc;");
}

void KeyboardTrainer::onLanguageChanged(int index)
{
    currentLanguage = (index == 0) ? "English" : "Russian";
    // Обновить текст на кнопках
    auto& layout = (currentLanguage == "English") ? englishLayout : russianLayout;
    for (auto it = keyButtons.begin(); it != keyButtons.end(); ++it) {
        QString displayText = layout.value(it.key(), it.key());
        if (it.key() == "space") displayText = "␣";
        it.value()->setText(displayText);
    }
    generateText();
    updateDisplayedText();
}

void KeyboardTrainer::loadTextFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть текстовый файл", "", "Text files (*.txt)");
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();
    // Разбить на слова (разделители: пробел, перенос строки)
    QStringList words = content.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    if (words.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Файл не содержит слов.");
        return;
    }
    targetWords.clear();
    for (const auto& w : words)
        targetWords.push_back(w);
    currentWordIndex = 0;
    currentCharIndex = 0;
    totalCorrectChars = 0;
    totalIncorrectChars = 0;
    timer.start();
    updateDisplayedText();
}

void KeyboardTrainer::generateText()
{
    targetWords.clear();
    const auto& words = (currentLanguage == "English") ? englishWords : russianWords;
    std::uniform_int_distribution<int> dist(0, words.size()-1);
    for (int i = 0; i < 20; ++i) {
        targetWords.push_back(words[dist(rng)]);
    }
    currentWordIndex = 0;
    currentCharIndex = 0;
    totalCorrectChars = 0;
    totalIncorrectChars = 0;
    timer.start();
}

void KeyboardTrainer::updateDisplayedText()
{
    QString html = "<p style='line-height:1.6;'>";
    for (size_t i = 0; i < targetWords.size(); ++i) {
        if (static_cast<size_t>(currentWordIndex) == i && currentCharIndex == 0) {
            // Текущее слово целиком подсвечено (ещё не вводили)
            html += "<span style='background-color: #ffaa00; color: black;'>" + targetWords[i] + "</span> ";
        } else if (static_cast<size_t>(currentWordIndex) == i && currentCharIndex > 0) {
            // Часть слова введена
            QString typed = targetWords[i].left(currentCharIndex);
            QString remaining = targetWords[i].mid(currentCharIndex);
            // Окраска зависит от правильности последнего символа
            // Но отображаем всё введённое зеленым/красным в зависимости от общей корректности, упростим
            bool lastCorrect = (currentCharIndex > 0) &&
                    (typed == targetWords[i].left(currentCharIndex));
            QString color = lastCorrect ? "lightgreen" : "red";
            html += "<span style='background-color: #333; color: " + color + ";'>" + typed + "</span>";
            html += "<span style='background-color: #ffaa00; color: black;'>" + remaining + "</span> ";
        } else {
            html += "<span style='color: #aaa;'>" + targetWords[i] + "</span> ";
        }
    }
    html += "</p>";
    textDisplay->setText(html);
}

void KeyboardTrainer::keyPressEvent(QKeyEvent *event)
{
    if (currentWordIndex >= static_cast<int>(targetWords.size()))
        return;
    QString text = event->text();
    if (text.isEmpty()) {
        QWidget::keyPressEvent(event);
        return;
    }
    // Игнорируем системные клавиши (Shift, Ctrl и т.д.)
    if (event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control ||
        event->key() == Qt::Key_Alt || event->key() == Qt::Key_Meta)
        return;

    QString expected = currentChar();
    bool correct = false;
    if (text == expected) {
        correct = true;
        totalCorrectChars++;
        currentCharIndex++;
        // Если слово завершено
        if (currentCharIndex >= targetWords[currentWordIndex].length()) {
            currentWordIndex++;
            currentCharIndex = 0;
            // Если весь текст пройден
            if (currentWordIndex >= static_cast<int>(targetWords.size())) {
                textDisplay->setText("Поздравляем! Текст введён. Нажмите 'Загрузить' или смените язык для нового.");
            }
        }
    } else {
        correct = false;
        totalIncorrectChars++;
    }
    highlightKey(text, correct);
    updateDisplayedText();
    // Анимация нажатой кнопки
    QPushButton *btn = keyButtons.value(event->key() == Qt::Key_Space ? "space" : event->text().toLower());
    if (!btn) {
        // для русских букв ищем в russianLayout по значению (символу), чтобы получить QWERTY-кнопку
        if (!btn && currentLanguage == "Russian") {
            for (auto it = russianLayout.constBegin(); it != russianLayout.constEnd(); ++it) {
                if (it.value() == text) {
                    btn = keyButtons.value(it.key());
                    break;
                }
            }
        }
    }
    if (btn) {
        // Анимация: временная смена цвета и масштаб
        QPropertyAnimation *colorAnim = new QPropertyAnimation(btn, "styleSheet");
        colorAnim->setDuration(200);
        QString originalStyle = btn->styleSheet();
        btn->setStyleSheet(correct ?
            "background-color: #00aa00; color: white; border: 2px solid #0f0; border-radius: 5px;" :
            "background-color: #aa0000; color: white; border: 2px solid #f00; border-radius: 5px;");
        QTimer::singleShot(200, [btn, originalStyle]() {
            btn->setStyleSheet(originalStyle);
        });
    }

    QWidget::keyPressEvent(event);
}

QString KeyboardTrainer::currentChar() const
{
    if (currentWordIndex >= static_cast<int>(targetWords.size())) return QString();
    const QString& word = targetWords[currentWordIndex];
    if (currentCharIndex >= word.length()) return QString();
    return QString(word[currentCharIndex]);
}

void KeyboardTrainer::highlightKey(const QString& key, bool correct)
{
    // Анимация уже реализована в keyPressEvent
    Q_UNUSED(key);
    Q_UNUSED(correct);
}

void KeyboardTrainer::updateStats()
{
    qint64 elapsed = timer.elapsed() / 1000;
    if (elapsed < 1) elapsed = 1;
    // WPM: стандартно (все символы введённые / 5) / (минуты)
    int totalChars = totalCorrectChars + totalIncorrectChars;
    double minutes = elapsed / 60.0;
    wpm = (totalChars / 5.0) / minutes;
    timerLabel->setText(QString("Время: %1 с").arg(elapsed));
    wpmLabel->setText(QString("WPM: %1").arg(wpm, 0, 'f', 1));
}
// Explicit definitions for methods declared in header
void KeyboardTrainer::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    setFocus();
}

void KeyboardTrainer::onKeyPressed(const QString& key, bool correct)
{
    Q_UNUSED(key);
    Q_UNUSED(correct);
}