#ifndef KEYBOARDTRAINER_H
#define KEYBOARDTRAINER_H

#include <QWidget>
#include <QElapsedTimer>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QList>
#include <QRandomGenerator>
#include <QSettings>
#include <QProgressBar>
#include <QFrame>

class GameWidget;
class KeyboardHintWidget;

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
        QColor color;          // цвет слова (зависит от длины/сложности)
    };

private slots:
    void onLanguageChanged(int index);
    void restartTraining();
    void loadTextFromFile();
    void updateStats();
    void gameLoop();
    void spawnWord();
    void onDifficultyChanged(int value);

private:
    void setupUI();
    void checkInput();
    void gameOver();
    void savePersonalBest();
    void loadPersonalBest();
    void updateLivesDisplay();
    void updateKeyboardHint();
    QString pickRandomWord();           // убран const

    QString currentLanguage = "English";
    QStringList englishWords;
    QStringList russianWords;
    QRandomGenerator rng;

    QList<Word> words;
    QElapsedTimer elapsedTimer;
    QTimer *gameTimer  = nullptr;
    QTimer *spawnTimer = nullptr;
    QTimer *statsTimer = nullptr;

   int   totalCorrectChars = 0;
int   totalMissedChars  = 0;
QString m_prevInput;  
    int   correctWords      = 0;
    int   missedWords       = 0;
    int   lives             = 5;
    int   streak            = 0;
    double wpm              = 0.0;
    double accuracy         = 100.0;
    double personalBestWPM  = 0.0;
    bool  isGameOver        = false;

    int  difficultyLevel    = 1;   // 0=Easy, 1=Normal, 2=Hard, 3=Insane
    static constexpr int TICK_INTERVAL = 16;  // ~60 fps

    // Скорость и интервал спауна по уровню сложности
const qreal SPEEDS[4]   = {0.21, 0.42, 0.7, 1.12};
   const int SPAWNS[4]   = {3120, 2340, 1560, 1040};
    const int   MAX_LIVES[4]= {7, 5, 3, 2};

    // ── Виджеты ────────────────────────────────────────────
    QLabel        *timerLabel;
    QLabel        *wpmLabel;
    QLabel        *accuracyLabel;
    QLabel        *scoreLabel;
    QLabel        *streakLabel;
    QLabel        *bestLabel;
    QLabel        *livesLabel;
    QLabel        *gameOverLabel = nullptr;
    QComboBox     *languageCombo;
    QComboBox     *diffCombo;
    QPushButton   *restartBtn;
    QPushButton   *loadFileBtn;
    QLineEdit     *inputField;
    GameWidget    *gameField   = nullptr;
    KeyboardHintWidget *kbHint = nullptr;
    QFrame        *statsBar;
};

// ─── Виджет игрового поля ─────────────────────────────────
class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent, QList<KeyboardTrainer::Word> *words);
    void setGameOver(bool v) { m_gameOver = v; update(); }
    void setCurrentInput(const QString &s) { m_input = s; update(); }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QList<KeyboardTrainer::Word> *m_words;
    bool m_gameOver = false;
    QString m_input;
};

// ─── Виджет подсказки клавиатуры ─────────────────────────
class KeyboardHintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardHintWidget(QWidget *parent = nullptr);
    void setNextChar(QChar c);
    void clearHint();
    void setLanguage(const QString &lang);   // новая функция

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

private:
    struct Key { QChar ch; QRectF rect; bool active = false; };
    void buildLayout();
    QStringList rowsForLanguage() const;     // новая вспомогательная функция

    QList<Key> m_keys;
    QChar m_active;
    QString m_language = "English";          // по умолчанию
};

#endif // KEYBOARDTRAINER_H