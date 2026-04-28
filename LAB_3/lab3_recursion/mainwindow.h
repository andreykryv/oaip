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
#include <QPainterPath>
#include <QPixmap>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsDropShadowEffect>
#include <QVariantAnimation>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QEasingCurve>
#include <QVector>
#include <QList>
#include <QMap>
#include <QHeaderView>
#include <QDesktopServices>
#include <QUrl>

// ─────────────────────────────────────────────────
//  Один ход в задаче «Ханойская башня»
// ─────────────────────────────────────────────────
struct HanoiMove {
    int ringNum;   // 1 = наименьшее, n = наибольшее
    int fromTower; // 0 = A, 1 = B, 2 = C
    int toTower;
};

// ─────────────────────────────────────────────────
//  Цветовая палитра
// ─────────────────────────────────────────────────
namespace Theme {
    const QString BG_MAIN   = "#0a0a14";
    const QString BG_CARD   = "#12121f";
    const QString BG_INPUT  = "#1a1a2e";
    const QString BG_OUTPUT = "#080810";
    const QString BORDER    = "#2d2d4a";
    const QString ACCENT    = "#8b5cf6";
    const QString ACCENT2   = "#06b6d4";
    const QString ACCENT3   = "#f472b6";
    const QString SUCCESS   = "#10b981";
    const QString WARNING   = "#fbbf24";
    const QString ERROR_CLR = "#ef4444";
    const QString TEXT_PRI  = "#f1f5f9";
    const QString TEXT_SEC  = "#94a3b8";
    const QString TEXT_DIM  = "#475569";
    const QString HIGHLIGHT = "#c4b5fd";
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

    // ── Задание 4: Ханойская башня ─────────────────
    void   collectHanoiMoves(int n, int from, int to, int via);
    void   drawHanoiInitial(int n);
    void   animateNextHanoiMove(int generation);
    double hanoiRingW(int ringNum) const;
    double hanoiRingY(int stackPos) const;

    // ── Задание 5: Файловая система ────────────────
    void             countItems(const QString &path, int &files, int &folders);
    QTreeWidgetItem* buildTree(const QString &path);

    // ── Построение вкладок ─────────────────────────
    QWidget* createTask1Widget();
    QWidget* createTask2Widget();
    QWidget* createTask3Widget();
    QWidget* createTask4Widget();
    QWidget* createTask5Widget();

    // ── UI-хелперы ─────────────────────────────────
    void         applyAppStyle();
    QGroupBox*   makeCard(const QString &title);
    QPushButton* makeRunButton(const QString &text = "▶  Запустить");
    QTextEdit*   makeOutput();
    void         printHeader(QTextEdit *out, const QString &title,
                             const QString &task);
    void         printLine(QTextEdit *out, const QString &text,
                           const QString &color = Theme::TEXT_PRI);
    void         printKV(QTextEdit *out, const QString &key,
                         const QString &val,
                         const QString &valColor = Theme::SUCCESS);
    void         printSep(QTextEdit *out);
    void         scrollBottom(QTextEdit *out);
    QString      badge(const QString &text, const QString &color);

    // ── Виджеты задачи 1 ───────────────────────────
    QLineEdit  *t1Input;
    QTextEdit  *t1Out;

    // ── Виджеты задачи 2 ───────────────────────────
    QSpinBox   *t2M, *t2N;
    QTextEdit  *t2Out;

    // ── Виджеты задачи 3 ───────────────────────────
    QLineEdit  *t3Input;
    QTextEdit  *t3Out;

    // ── Виджеты задачи 4 ───────────────────────────
    QSpinBox          *t4Rings   {nullptr};
    QTextEdit         *t4Out     {nullptr};
    QGraphicsView     *t4View    {nullptr};
    QGraphicsScene    *t4Scene   {nullptr};

    // Данные анимации
    QVector<HanoiMove>           m_moves;
    int                          m_moveIdx      {0};
    int                          m_hanoiN       {0};
    int                          m_generation   {0};   // для отмены старой анимации
    QVector<QList<int>>          m_stacks;              // [башня] → список (низ→верх)
    QMap<int, QGraphicsPathItem*> m_ringItems;           // ringNum → item
    QSequentialAnimationGroup   *m_activeAnim  {nullptr};

    // ── Виджеты задачи 5 ───────────────────────────
    QLineEdit  *t5Path;
    QTextEdit  *t5Out;
    QTreeWidget*t5Tree;
};

#endif // MAINWINDOW_H