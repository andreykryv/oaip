#ifndef KEYBOARDTRAINER_H
#define KEYBOARDTRAINER_H

#include <QWidget>
#include <QElapsedTimer>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QList>
#include <QRandomGenerator>

class GameWidget;

class KeyboardTrainer : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardTrainer(QWidget *parent = nullptr);

    struct Word {
        QString text;
        qreal x = 0;
        qreal y = 0;
        bool highlighted = false;
    };

private slots:
    void onLanguageChanged(int index);
    void restartTraining();
    void loadTextFromFile();
    void updateStats();
    void gameLoop();
    void spawnWord();

private:
    void setupUI();
    void checkInput();

    QString currentLanguage = "English";
    QStringList englishWords;
    QStringList russianWords;
    QRandomGenerator rng;

    QList<Word> words;
    QElapsedTimer elapsedTimer;
    QTimer *gameTimer = nullptr;
    QTimer *spawnTimer = nullptr;
    QTimer *statsTimer = nullptr;

    int totalCorrectChars = 0;
    int totalMissedChars = 0;
    int correctWords = 0;
    int missedWords = 0;
    double wpm = 0.0;
    double accuracy = 100.0;

    QLabel *timerLabel;
    QLabel *wpmLabel;
    QLabel *accuracyLabel;
    QLabel *scoreLabel;
    QComboBox *languageCombo;
    QPushButton *restartBtn;
    QPushButton *loadFileBtn;
    QLineEdit *inputField;
    GameWidget *gameField = nullptr;

    static constexpr int TICK_INTERVAL = 30;     // ms
    static constexpr int SPAWN_INTERVAL = 1800;  // ms
    static constexpr qreal WORD_SPEED = 1.2;     // pixels per tick
};

#endif // KEYBOARDTRAINER_H