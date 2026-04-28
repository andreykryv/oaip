#ifndef KEYBOARDTRAINER_H
#define KEYBOARDTRAINER_H

#include <QWidget>
#include <QElapsedTimer>
#include <QTimer>
#include <QMap>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTextEdit>
#include <QFile>
#include <vector>
#include <random>

class KeyboardTrainer : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardTrainer(QWidget *parent = nullptr);

private slots:
    void onLanguageChanged(int index);
    void loadTextFromFile();
    void updateStats();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupUI();
    void generateText();
    void updateDisplayedText();
    void highlightKey(const QString& key, bool correct);
    QString currentChar() const;

    // Раскладки
    QMap<QString, QString> englishLayout; // кнопка -> символ
    QMap<QString, QString> russianLayout;
    QMap<QString, QPushButton*> keyButtons;
    QStringList englishWords;
    QStringList russianWords;

    // Текущее состояние
    QString currentLanguage = "English";
    std::vector<QString> targetWords;
    int currentWordIndex = 0;
    int currentCharIndex = 0;
    int totalCorrectChars = 0;
    int totalIncorrectChars = 0;
    QElapsedTimer timer;
    QTimer *statsTimer;
    double wpm = 0.0;

    QLabel *textDisplay;
    QLabel *timerLabel;
    QLabel *wpmLabel;
    QComboBox *languageCombo;
    QPushButton *loadFileBtn;
    QWidget *keyboardWidget;

    std::mt19937 rng;
};

#endif // KEYBOARDTRAINER_H