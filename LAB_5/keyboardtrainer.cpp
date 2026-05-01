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
#include <QPainter>
#include <QFontMetrics>
#include <QRandomGenerator>

// Класс для отрисовки текста с курсором
class TextContainerPainter : public QWidget {
public:
    TextContainerPainter(QWidget *parent) : QWidget(parent), cursorBlink(true) {
        // Запускаем таймер для мигания курсора
        QTimer *cursorTimer = new QTimer(this);
        connect(cursorTimer, &QTimer::timeout, this, &TextContainerPainter::toggleCursorBlink);
        cursorTimer->start(530);
    }

    void setTextData(const QString& text, int pos, int lineStart, int lineEnd) {
        fullText = text;
        currentPosition = pos;
        this->lineStart = lineStart;
        this->lineEnd = lineEnd;
        update();
    }

    void toggleCursorBlink() {
        cursorBlink = !cursorBlink;
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Фон
        painter.fillRect(rect(), QColor("#1a1a2e"));

        QFont font("Consolas", 16, QFont::Bold);
        painter.setFont(font);

        int x = 15;
        int y = 25;
        int charWidth = 11;

        for (int i = lineStart; i < lineEnd && i < fullText.length(); ++i) {
            QChar ch = fullText[i];
            QString s = QString(ch);

            if (i < currentPosition) {
                // Уже введено - проверяем правильность (упрощенно - считаем все верным)
                painter.setPen(QColor("#00ff88"));  // Зеленый
            } else if (i == currentPosition) {
                // Текущий символ с курсором
                painter.setPen(QColor("#ffffff"));
                painter.fillRect(x - 2, y - 22, charWidth + 4, 28, QColor("#4a4a8a"));
            } else {
                // Еще не введено
                painter.setPen(QColor("#666688"));
            }

            painter.drawText(x, y, s);
            x += charWidth;
        }

        // Рисуем мигающий курсор
        if (currentPosition < fullText.length()) {
            if (cursorBlink) {
                int cursorX = 15 + (currentPosition - lineStart) * charWidth;
                painter.fillRect(cursorX, y - 22, 2, 28, QColor("#00ff88"));
            }
        }
    }

private:
    QString fullText;
    int currentPosition = 0;
    int lineStart = 0;
    int lineEnd = 0;
    bool cursorBlink;
};

KeyboardTrainer::KeyboardTrainer(QWidget *parent) : QWidget(parent), rng(std::random_device{}())
{
    setupUI();
    setFocusPolicy(Qt::StrongFocus);

    // Инициализация раскладок
    initLayouts();

    // Заполнение словарей
    englishWords = QStringList{"the", "quick", "brown", "fox", "jumps", "over", "lazy", "dog",
                    "hello", "world", "keyboard", "trainer", "practice", "typing", "speed",
                    "programming", "computer", "software", "developer", "algorithm", "data"};
    russianWords = QStringList{"привет", "мир", "клавиатура", "тренажёр", "практика", "скорость",
                    "быстрая", "лиса", "прыгает", "через", "ленивого", "пса",
                    "программирование", "компьютер", "разработка", "алгоритм", "данные"};

    generateText();
    updateDisplayedText();

    statsTimer = new QTimer(this);
    connect(statsTimer, &QTimer::timeout, this, &KeyboardTrainer::updateStats);
    statsTimer->start(1000);
}

void KeyboardTrainer::initLayouts()
{
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
}

void KeyboardTrainer::setupUI()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Верхняя панель с настройками
    auto *topPanel = new QWidget;
    auto *topLayout = new QHBoxLayout(topPanel);
    topLayout->setContentsMargins(0, 0, 0, 0);

    languageCombo = new QComboBox;
    languageCombo->addItem("🇬🇧 English");
    languageCombo->addItem("🇷🇺 Русский");
    languageCombo->setStyleSheet(
        "QComboBox { "
        "   background-color: #2a2a3a; color: #ffffff; border: 2px solid #4a4a6a; "
        "   padding: 8px 16px; border-radius: 8px; font-size: 14px; font-weight: bold; "
        "} "
        "QComboBox::drop-down { border: none; width: 30px; } "
        "QComboBox::down-arrow { image: none; border-left: 6px solid transparent; "
        "   border-right: 6px solid transparent; border-top: 8px solid #ffffff; margin-right: 10px; }"
    );
    connect(languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &KeyboardTrainer::onLanguageChanged);
    topLayout->addWidget(languageCombo);

    restartBtn = new QPushButton("🔄 Заново");
    restartBtn->setStyleSheet(
        "QPushButton { "
        "   background-color: #4a4a8a; color: white; border: none; "
        "   padding: 8px 20px; border-radius: 8px; font-size: 14px; font-weight: bold; "
        "} "
        "QPushButton:hover { background-color: #5a5a9a; } "
        "QPushButton:pressed { background-color: #3a3a7a; }"
    );
    connect(restartBtn, &QPushButton::clicked, this, &KeyboardTrainer::restartTraining);
    topLayout->addWidget(restartBtn);

    loadFileBtn = new QPushButton("📁 Загрузить файл");
    loadFileBtn->setStyleSheet(
        "QPushButton { "
        "   background-color: #4a4a8a; color: white; border: none; "
        "   padding: 8px 20px; border-radius: 8px; font-size: 14px; font-weight: bold; "
        "} "
        "QPushButton:hover { background-color: #5a5a9a; } "
        "QPushButton:pressed { background-color: #3a3a7a; }"
    );
    connect(loadFileBtn, &QPushButton::clicked, this, &KeyboardTrainer::loadTextFromFile);
    topLayout->addWidget(loadFileBtn);

    topLayout->addStretch();
    mainLayout->addWidget(topPanel);

    // Панель статистики
    auto *statsPanel = new QWidget;
    auto *statsLayout = new QHBoxLayout(statsPanel);
    statsLayout->setContentsMargins(0, 0, 0, 0);
    statsLayout->setSpacing(30);

    timerLabel = new QLabel("⏱️ 0:00");
    timerLabel->setStyleSheet("font-size: 20px; color: #00ff88; font-weight: bold;");
    statsLayout->addWidget(timerLabel);

    wpmLabel = new QLabel("⚡ 0 WPM");
    wpmLabel->setStyleSheet("font-size: 20px; color: #ffaa00; font-weight: bold;");
    statsLayout->addWidget(wpmLabel);

    accuracyLabel = new QLabel("🎯 100%");
    accuracyLabel->setStyleSheet("font-size: 20px; color: #00aaff; font-weight: bold;");
    statsLayout->addWidget(accuracyLabel);

    progressLabel = new QLabel("");
    progressLabel->setStyleSheet("font-size: 16px; color: #888888;");
    statsLayout->addWidget(progressLabel);

    statsLayout->addStretch();
    mainLayout->addWidget(statsPanel);

    // Область для отображения текста с прокруткой
    textScrollArea = new QScrollArea;
    textScrollArea->setWidgetResizable(true);
    textScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    textScrollArea->setStyleSheet(
        "QScrollArea { "
        "   background-color: #1a1a2e; border: 2px solid #3a3a5a; border-radius: 12px; "
        "} "
        "QScrollBar:vertical { background-color: #2a2a3a; width: 12px; border-radius: 6px; } "
        "QScrollBar::handle:vertical { background-color: #4a4a6a; border-radius: 6px; min-height: 20px; } "
        "QScrollBar::handle:vertical:hover { background-color: #5a5a7a; }"
    );

    textScrollArea->setMinimumHeight(180);
    textScrollArea->setMaximumHeight(220);

    // Создаем виджет для отображения текста
    textDisplayWidget = new TextContainerPainter(this);
    textDisplayWidget->setMinimumHeight(180);
    textDisplayWidget->setMaximumHeight(220);
    textScrollArea->setWidget(textDisplayWidget);

    mainLayout->addWidget(textScrollArea);

    // Создание клавиатуры
    keyboardWidget = new QWidget;
    auto *grid = new QGridLayout(keyboardWidget);
    grid->setSpacing(6);
    grid->setContentsMargins(10, 10, 10, 10);

    // Ряды кнопок согласно стандартной QWERTY
    QStringList row1 = {"`","1","2","3","4","5","6","7","8","9","0","-","="};
    QStringList row2 = {"q","w","e","r","t","y","u","i","o","p","[","]","\\"};
    QStringList row3 = {"a","s","d","f","g","h","j","k","l",";","'"};
    QStringList row4 = {"z","x","c","v","b","n","m",",",".","/"};

    auto addKey = [&](const QString& key, int row, int col){
        QPushButton *btn = new QPushButton;
        QString displayText = (key == "space") ? "␣ SPACE" : key.toUpper();
        btn->setText(displayText);
        btn->setMinimumSize(55, 55);
        btn->setMaximumSize(55, 55);
        btn->setStyleSheet(
            "QPushButton { "
            "   background-color: #2a2a3a; color: #cccccc; "
            "   border: 2px solid #3a3a5a; border-radius: 8px; "
            "   font-size: 16px; font-weight: bold; "
            "} "
            "QPushButton:hover { background-color: #3a3a5a; border-color: #5a5a7a; } "
            "QPushButton:pressed { background-color: #4a4a6a; }"
        );
        // Эффект тени
        auto *shadow = new QGraphicsDropShadowEffect;
        shadow->setBlurRadius(8);
        shadow->setOffset(2,2);
        shadow->setColor(QColor(0,0,0,150));
        btn->setGraphicsEffect(shadow);
        keyButtons[key] = btn;
        grid->addWidget(btn, row, col);
    };

    for (int i = 0; i < row1.size(); ++i) addKey(row1[i], 0, i);
    for (int i = 0; i < row2.size(); ++i) addKey(row2[i], 1, i);
    // Ряд 3 с отступом
    for (int i = 0; i < row3.size(); ++i) addKey(row3[i], 2, i+1);
    for (int i = 0; i < row4.size(); ++i) addKey(row4[i], 3, i+2);

    // Пробел
    QPushButton *spaceBtn = new QPushButton("␣ SPACE");
    spaceBtn->setMinimumSize(300, 55);
    spaceBtn->setMaximumSize(300, 55);
    spaceBtn->setStyleSheet(
        "QPushButton { "
        "   background-color: #2a2a3a; color: #cccccc; "
        "   border: 2px solid #3a3a5a; border-radius: 8px; "
        "   font-size: 16px; font-weight: bold; "
        "} "
        "QPushButton:hover { background-color: #3a3a5a; border-color: #5a5a7a; } "
        "QPushButton:pressed { background-color: #4a4a6a; }"
    );
    auto *spaceShadow = new QGraphicsDropShadowEffect;
    spaceShadow->setBlurRadius(8);
    spaceShadow->setOffset(2,2);
    spaceShadow->setColor(QColor(0,0,0,150));
    spaceBtn->setGraphicsEffect(spaceShadow);
    keyButtons["space"] = spaceBtn;
    grid->addWidget(spaceBtn, 4, 2, 1, 6);

    mainLayout->addWidget(keyboardWidget);

    setStyleSheet("background-color: #0f0f1a; color: #cccccc;");

    // Обновляем текст на кнопках в зависимости от языка
    onLanguageChanged(0);
}

void KeyboardTrainer::onLanguageChanged(int index)
{
    currentLanguage = (index == 0) ? "English" : "Russian";
    // Обновить текст на кнопках
    auto& layout = (currentLanguage == "English") ? englishLayout : russianLayout;
    for (auto it = keyButtons.begin(); it != keyButtons.end(); ++it) {
        QString displayText = layout.value(it.key(), it.key());
        if (it.key() == "space") {
            displayText = "␣ SPACE";
        } else {
            displayText = displayText.toUpper();
        }
        it.value()->setText(displayText);
    }
    generateText();
    updateDisplayedText();
}

void KeyboardTrainer::restartTraining()
{
    currentPosition = 0;
    totalCorrectChars = 0;
    totalIncorrectChars = 0;
    timer.invalidate();
    wpm = 0.0;
    accuracy = 100.0;
    generateText();
    updateDisplayedText();
    updateStats();
    setFocus();
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

    // Очищаем текст от лишних пробелов и переносов
    content = content.simplified();
    if (content.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Файл не содержит текста.");
        return;
    }

    fullText = content;
    currentPosition = 0;
    totalCorrectChars = 0;
    totalIncorrectChars = 0;
    timer.invalidate();
    wpm = 0.0;
    accuracy = 100.0;
    updateDisplayedText();
    setFocus();
}

void KeyboardTrainer::generateText()
{
    const auto& words = (currentLanguage == "English") ? englishWords : russianWords;

    if (words.isEmpty()) {
        fullText = "No words available";
        currentPosition = 0;
        totalCorrectChars = 0;
        totalIncorrectChars = 0;
        timer.invalidate();
        wpm = 0.0;
        accuracy = 100.0;
        return;
    }
    std::uniform_int_distribution<int> dist(0, words.size()-1);

    fullText.clear();
    int wordCount = 50;  // Генерируем 50 слов
    for (int i = 0; i < wordCount; ++i) {
        if (i > 0) fullText += " ";
        fullText += words[dist(rng)];
    }

    currentPosition = 0;
    totalCorrectChars = 0;
    totalIncorrectChars = 0;
    timer.invalidate();
    wpm = 0.0;
    accuracy = 100.0;
}

void KeyboardTrainer::updateDisplayedText()
{
    // Определяем текущую строку для отображения
    int lineStart = getCurrentLineStartIndex();
    int lineEnd = getCurrentLineEndIndex();

    // Обновляем прогресс
    int progressPercent = (fullText.isEmpty()) ? 0 : (currentPosition * 100 / fullText.length());
    progressLabel->setText(QString("📊 %1% (%2/%3)").arg(progressPercent).arg(currentPosition).arg(fullText.length()));

    // Передаем данные в виджет текста для отрисовки
    if (textDisplayWidget) {
        TextContainerPainter *painterWidget = qobject_cast<TextContainerPainter*>(textDisplayWidget);
        if (painterWidget) {
            painterWidget->setTextData(fullText, currentPosition, lineStart, lineEnd);
        }
    }

    // Прокрутка к курсору
    int cursorLine = currentPosition / maxCharsPerLine;
    textScrollArea->verticalScrollBar()->setValue(cursorLine * lineHeight);
}

int KeyboardTrainer::getCurrentLineStartIndex() const
{
    int currentLine = currentPosition / maxCharsPerLine;
    return currentLine * maxCharsPerLine;
}

int KeyboardTrainer::getCurrentLineEndIndex() const
{
    int currentLine = currentPosition / maxCharsPerLine;
    int start = currentLine * maxCharsPerLine;
    int end = qMin(start + maxCharsPerLine, fullText.length());
    return end;
}

void KeyboardTrainer::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    // Рисуем текст в textContainer через его paintEvent
    // Но так как textContainer - отдельный виджет, мы должны рисовать там
}

void KeyboardTrainer::keyPressEvent(QKeyEvent *event)
{
    if (currentPosition >= fullText.length())
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

    // Запускаем таймер при первом нажатии
    if (!timer.isValid()) {
        timer.start();
    }

    QString expected = currentChar();
    bool correct = false;
    if (text == expected) {
        correct = true;
        totalCorrectChars++;
        currentPosition++;

        // Проверяем завершение
        if (currentPosition >= fullText.length()) {
            QMessageBox::information(this, "Поздравляем!",
                QString("Текст введён!\nWPM: %1\nТочность: %2%").arg(wpm, 0, 'f', 1).arg(accuracy, 0, 'f', 1));
        }
    } else {
        correct = false;
        totalIncorrectChars++;
    }

    highlightKey(text, correct);
    updateDisplayedText();
    updateStats();

    // Анимация нажатой кнопки
    QPushButton *btn = nullptr;
    QString keyName = event->text().toLower();

    // Для пробела
    if (event->key() == Qt::Key_Space) {
        btn = keyButtons.value("space");
    } else {
        // Пытаемся найти кнопку по символу
        btn = keyButtons.value(keyName);
        if (!btn && currentLanguage == "Russian") {
            // Для русской раскладки ищем соответствие
            for (auto it = russianLayout.constBegin(); it != russianLayout.constEnd(); ++it) {
                if (it.value() == text.toLower()) {
                    btn = keyButtons.value(it.key());
                    break;
                }
            }
        }
        if (!btn && currentLanguage == "English") {
            for (auto it = englishLayout.constBegin(); it != englishLayout.constEnd(); ++it) {
                if (it.value() == text.toLower()) {
                    btn = keyButtons.value(it.key());
                    break;
                }
            }
        }
    }

    if (btn) {
        QString originalStyle = btn->styleSheet();
        btn->setStyleSheet(correct ?
            "background-color: #00aa44; color: white; border: 2px solid #00ff88; border-radius: 8px;" :
            "background-color: #aa2222; color: white; border: 2px solid #ff4444; border-radius: 8px;");
        QTimer::singleShot(150, [btn, originalStyle]() {
            btn->setStyleSheet(originalStyle);
        });
    }

    QWidget::keyPressEvent(event);
}

QString KeyboardTrainer::currentChar() const
{
    if (currentPosition >= fullText.length()) return QString();
    return QString(fullText[currentPosition]);
}

void KeyboardTrainer::highlightKey(const QString& key, bool correct)
{
    Q_UNUSED(key);
    Q_UNUSED(correct);
}



void KeyboardTrainer::updateStats()
{
    qint64 elapsed = 0;
    if (timer.isValid()) {
        elapsed = timer.elapsed() / 1000;
    }
    if (elapsed < 1) elapsed = 1;

    // WPM: стандартно (все символы / 5) / минуты
    int totalChars = totalCorrectChars + totalIncorrectChars;
    double minutes = elapsed / 60.0;
    wpm = (totalCorrectChars / 5.0) / minutes;

    // Точность
    if (totalChars > 0) {
        accuracy = (totalCorrectChars * 100.0) / totalChars;
    } else {
        accuracy = 100.0;
    }

    // Форматируем время как ММ:СС
    int mins = elapsed / 60;
    int secs = elapsed % 60;
    timerLabel->setText(QString("⏱️ %1:%2").arg(mins, 2, 10, QChar('0')).arg(secs, 2, 10, QChar('0')));
    wpmLabel->setText(QString("⚡ %1 WPM").arg(wpm, 0, 'f', 1));
    accuracyLabel->setText(QString("🎯 %1%").arg(accuracy, 0, 'f', 1));
}

void KeyboardTrainer::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    setFocus();
}