#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QWidget>
#include <QString>
#include <QStringList>
#include <QStatusBar>
#include <QFrame>
#include <QScrollBar>
#include <QApplication>
#include <QClipboard>
#include <QFont>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QPauseAnimation>
#include <QVector>
#include <QHeaderView>
#include <QComboBox>
#include <QSlider>
#include <QProgressBar>
#include <QSplitter>
#include <QScrollArea>
#include <QDesktopServices>
#include <QUrl>
#include <cmath>
#include <climits>
#include <stdexcept>
#include <QRegularExpression>

// ─────────────────────────────────────────────────
//  Структура для хранения информации о кольце
// ─────────────────────────────────────────────────
struct RingInfo {
    QGraphicsRectItem *item{};
    int ringIndex{0};      // индекс кольца (0 - самое маленькое)
    int currentTower{0};   // текущий стержень (0, 1, 2)
};

// ─────────────────────────────────────────────────
//  Современная цветовая палитра (Dark Theme)
// ─────────────────────────────────────────────────
namespace ModernTheme {
    // Основные цвета фона
    inline const QString BG_PRIMARY     = "#0f0f1a";      // Очень тёмный сине-фиолетовый
    inline const QString BG_SECONDARY   = "#1a1a2e";      // Тёмный карточки
    inline const QString BG_TERTIARY    = "#252542";      // Элементы UI
    inline const QString BG_INPUT       = "#16213e";      // Поля ввода
    inline const QString BG_OUTPUT      = "#0a0a14";      // Консоль вывода
    
    // Акцентные цвета
    inline const QString ACCENT_PRIMARY = "#7c3aed";      // Фиолетовый неон
    inline const QString ACCENT_SECOND  = "#06b6d4";      // Циан
    inline const QString ACCENT_THIRD   = "#f472b6";      // Розовый
    inline const QString ACCENT_GRAD_START = "#8b5cf6";   // Градиент старт
    inline const QString ACCENT_GRAD_END   = "#06b6d4";   // Градиент конец
    
    // Статусные цвета
    inline const QString SUCCESS        = "#10b981";      // Зелёный
    inline const QString WARNING        = "#f59e0b";      // Жёлтый
    inline const QString ERROR_CLR      = "#ef4444";      // Красный
    inline const QString INFO           = "#3b82f6";      // Синий
    
    // Текст
    inline const QString TEXT_PRIMARY   = "#f8fafc";      // Белый
    inline const QString TEXT_SECONDARY = "#94a3b8";      // Серый светлый
    inline const QString TEXT_DIM       = "#64748b";      // Серый тёмный
    inline const QString TEXT_HIGHLIGHT = "#e2e8f0";      // Подсветка
    
    // Границы и эффекты
    inline const QString BORDER         = "#334155";      // Граница
    inline const QString BORDER_LIGHT   = "#475569";      // Светлая граница
    inline const QString SHADOW         = "rgba(0,0,0,0.4)";
}

// ─────────────────────────────────────────────────
//  Главное окно приложения
// ─────────────────────────────────────────────────
class MainWindow : public QMainWindow {
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void runTask1();
    void runTask2();
    void runTask3();
    void runTask4();
    void runTask5();
    void browseFolder();
    void onHanoiStart();

private:
    // ══ Рекурсивные алгоритмы ═══════════════════════
    QString      decToBinRec(long long n);
    long long    ackermann(int m, int n, int &calls);
    long long    reverseNum(long long n);
    int          countDigits(long long n);
    void         hanoi(int n, char from, char to, char via,
                       QStringList &steps, int &stepNo);
    void         countItems(const QString &path, int &files, int &folders);
    QTreeWidgetItem* buildTree(const QString &path);
    
    // ══ Ханойская башня - анимация ═══════════════════
    void setupHanoiScene(int rings);
    void animateHanoiMove(int ringIdx, int fromTower, int toTower);
    void executeHanoiAlgorithm(int n, char from, char to, char via);
    
    // ══ Построение вкладок ═══════════════════════════
    QWidget* createTask1Widget();
    QWidget* createTask2Widget();
    QWidget* createTask3Widget();
    QWidget* createTask4Widget();
    QWidget* createTask5Widget();
    
    // ══ UI-хелперы ═══════════════════════════════════
    void applyModernStyle();
    QGroupBox* createCard(const QString &title, const QString &icon = "");
    QPushButton* createActionButton(const QString &text = "▶ Запустить", 
                                     const QString &type = "primary");
    QTextEdit* createOutputConsole();
    void printHeader(QTextEdit *out, const QString &title, const QString &task);
    void printLine(QTextEdit *out, const QString &text, const QString &color = ModernTheme::TEXT_PRIMARY);
    void printKV(QTextEdit *out, const QString &key, const QString &val,
                 const QString &valColor = ModernTheme::SUCCESS);
    void printSep(QTextEdit *out);
    void scrollBottom(QTextEdit *out);
    QString badge(const QString &text, const QString &color);
    QLabel* createSectionLabel(const QString &text);
    QLineEdit* createInputField(const QString &placeholder = "");
    QSpinBox* createSpinBox(int min, int max, int value);
    
    // ══ Виджеты задач ════════════════════════════════
    QLineEdit  *t1Input{};
    QTextEdit  *t1Out{};

    QSpinBox   *t2M{};
    QSpinBox   *t2N{};
    QTextEdit  *t2Out{};

    QLineEdit  *t3Input{};
    QTextEdit  *t3Out{};

    QSpinBox   *t4Rings{};
    QTextEdit  *t4Out{};
    QGraphicsView *t4Visualizer{};
    QGraphicsScene *t4Scene{};
    QVector<RingInfo> t4RingsData;
    double     t4TowerX[3]{};
    double     t4BaseY{};
    double     t4RingHeight{};
    bool       t4IsAnimating{false};
    int        t4CurrentStep{0};
    QStringList t4MoveQueue;
    QTimer     *t4AnimationTimer{};

    QLineEdit  *t5Path{};
    QTextEdit  *t5Out{};
    QTreeWidget* t5Tree{};
    
    // Общие элементы
    QTabWidget *mainTabs{};
    QStatusBar *statusBarWidget{};
};

#endif // MAINWINDOW_H
