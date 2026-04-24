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

// ─────────────────────────────────────────────────
//  Цветовая палитра терминала
// ─────────────────────────────────────────────────
namespace Theme {
    const QString BG_MAIN    = "#0d0d1a";
    const QString BG_CARD    = "#131328";
    const QString BG_INPUT   = "#1a1a30";
    const QString BG_OUTPUT  = "#0a0a16";
    const QString BORDER     = "#2a2a50";
    const QString ACCENT     = "#7c3aed";      // фиолетовый
    const QString ACCENT2    = "#06b6d4";      // циан
    const QString SUCCESS    = "#10b981";      // зелёный
    const QString WARNING    = "#f59e0b";      // жёлтый
    const QString ERROR_CLR  = "#ef4444";      // красный
    const QString TEXT_PRI   = "#e2e8f0";
    const QString TEXT_SEC   = "#94a3b8";
    const QString TEXT_DIM   = "#475569";
    const QString HIGHLIGHT  = "#a78bfa";
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
    void         countItems(const QString &path, int &files, int &folders);
    QTreeWidgetItem* buildTree(const QString &path);

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

    QLineEdit  *t5Path;
    QTextEdit  *t5Out;
    QTreeWidget*t5Tree;
};

#endif // MAINWINDOW_H
