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
#include <QVector>
#include <QHeaderView>

// ─────────────────────────────────────────────────
//  Структура для хранения информации о кольце
// ─────────────────────────────────────────────────
struct RingInfo {
    QGraphicsRectItem *item;
    int ringIndex;      // индекс кольца (0 - самое маленькое)
    int currentTower;   // текущий стержень (0, 1, 2)
};

// ─────────────────────────────────────────────────
//  Цветовая палитра терминала
// ─────────────────────────────────────────────────
namespace Theme {
    const QString BG_MAIN    = "#0a0a14";
    const QString BG_CARD    = "#12121f";
    const QString BG_INPUT   = "#1a1a2e";
    const QString BG_OUTPUT  = "#080810";
    const QString BORDER     = "#2d2d4a";
    const QString ACCENT     = "#8b5cf6";      // фиолетовый
    const QString ACCENT2    = "#06b6d4";      // циан
    const QString ACCENT3    = "#f472b6";      // розовый
    const QString SUCCESS    = "#10b981";      // зелёный
    const QString WARNING    = "#fbbf24";      // жёлтый
    const QString ERROR_CLR  = "#ef4444";      // красный
    const QString TEXT_PRI   = "#f1f5f9";
    const QString TEXT_SEC   = "#94a3b8";
    const QString TEXT_DIM   = "#475569";
    const QString HIGHLIGHT  = "#c4b5fd";
}

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

private:
    // ── Рекурсивные алгоритмы ──────────────────────
    QString      decToBinRec(long long n);
    long long    ackermann(int m, int n, int &calls);
    long long    reverseNum(long long n);
    int          countDigits(long long n);
    void         hanoi(int n, char from, char to, char via,
                       QStringList &steps, int &stepNo);
    void         runHanoiAnimation(int n, char from, char to, char via);
    void         runHanoiAnimationInternal(int n, char from, char to, char via);
    void         countItems(const QString &path, int &files, int &folders);
    QTreeWidgetItem* buildTree(const QString &path);
    void         drawHanoiTowers(int n);
    void         animateHanoiMove(RingInfo ring, int fromTower, int toTower,
                                  int delayMs = 0);

    // ── Построение вкладок ─────────────────────────
    QWidget* createTask1Widget();
    QWidget* createTask2Widget();
    QWidget* createTask3Widget();
    QWidget* createTask4Widget();
    QWidget* createTask5Widget();

    // ── UI-хелперы ─────────────────────────────────
    void        applyAppStyle();
    QGroupBox*  makeCard(const QString &title);
    QPushButton* makeRunButton(const QString &text = "▶  Запустить");
    QTextEdit*  makeOutput();
    void        printHeader(QTextEdit *out, const QString &title,
                            const QString &task);
    void        printLine(QTextEdit *out, const QString &text,
                          const QString &color = Theme::TEXT_PRI);
    void        printKV(QTextEdit *out, const QString &key,
                        const QString &val,
                        const QString &valColor = Theme::SUCCESS);
    void        printSep(QTextEdit *out);
    void        scrollBottom(QTextEdit *out);
    QString     badge(const QString &text, const QString &color);

    // ── Виджеты задач ──────────────────────────────
    QLineEdit  *t1Input;
    QTextEdit  *t1Out;

    QSpinBox   *t2M, *t2N;
    QTextEdit  *t2Out;

    QLineEdit  *t3Input;
    QTextEdit  *t3Out;

    QSpinBox   *t4Rings;
    QTextEdit  *t4Out;
    QGraphicsView *t4Visualizer;
    QGraphicsScene *t4Scene;
    QVector<RingInfo> t4RingsData;      // данные о кольцах для анимации
    double     t4TowerX[3];             // X-координаты стержней
    double     t4BaseY;                 // Y-координата основания
    double     t4TowerHeight;           // высота стержня
    int        t4CurrentStep;           // текущий шаг анимации
    bool       t4IsAnimating;           // флаг анимации

    QLineEdit  *t5Path;
    QTextEdit  *t5Out;
    QTreeWidget*t5Tree;
};

#endif // MAINWINDOW_H