#ifndef KEYBOARDTRAINER_H
#define KEYBOARDTRAINER_H

#include <QWidget>
#include <QElapsedTimer>
#include <QTimer>
#include <QMap>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QFile>
#include <QScrollArea>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QVector>
#include <random>
#include <QPainter>
#include <QKeyEvent>
#include <QScrollBar>

class KeyboardTrainer : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardTrainer(QWidget *parent = nullptr);

private slots:
    void onLanguageChanged(int index);
    void loadTextFromFile();
    void updateStats();
    void restartTraining();
    void onKeyPressed(const QString& key, bool correct);
    void updateCursorBlink();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUI();
    void generateText();
    void updateDisplayedText();
    void highlightKey(const QString& key, bool correct);
    QString currentChar() const;
    QString getKeyForChar(const QChar& ch) const;
    void animateButton(QPushButton* btn, bool correct);
    void initLayouts();
    QString getCurrentLineText() const;
    int getCurrentLineStartIndex() const;
    int getCurrentLineEndIndex() const;
    
    // Раскладки
    QMap<QString, QString> englishLayout; // кнопка -> символ
    QMap<QString, QString> russianLayout;
    QMap<QChar, QString> charToKeyEng;  // символ -> кнопка
    QMap<QChar, QString> charToKeyRus;
    QMap<QString, QPushButton*> keyButtons;
    QStringList englishWords;
    QStringList russianWords;

    // Текущее состояние
    QString currentLanguage = "English";
    QString fullText;  // Полный текст для ввода
    int currentPosition = 0;  // Позиция в полном тексте
    int totalCorrectChars = 0;
    int totalIncorrectChars = 0;
    QElapsedTimer timer;
    QTimer *statsTimer;
    QTimer *cursorTimer;
    double wpm = 0.0;
    double accuracy = 100.0;
    bool cursorVisible = true;
    int cursorBlinkCounter = 0;

    QLabel *textDisplay;
    QLabel *timerLabel;
    QLabel *wpmLabel;
    QLabel *accuracyLabel;
    QLabel *progressLabel;
    QComboBox *languageCombo;
    QPushButton *loadFileBtn;
    QPushButton *restartBtn;
    QWidget *keyboardWidget;
    QScrollArea *textScrollArea;
    QWidget *textDisplayWidget;  // Кастомный виджет для отображения текста
    
    // Размеры для отрисовки
    int charWidth = 11;
    int charHeight = 28;
    int lineHeight = 36;
    int maxCharsPerLine = 90;
    
    std::mt19937 rng;

    // Для анимаций
    QMap<QString, QPropertyAnimation*> buttonAnimations;
};

#endif // KEYBOARDTRAINER_H