#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QLabel>
#include <QWidget>
#include <QScrollArea>
#include <QGroupBox>
#include <QComboBox>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

// ─────────────────────────────────────────────────────────
//  SortVisualizer — гистограмма для визуализации сортировки
// ─────────────────────────────────────────────────────────
class SortVisualizer : public QWidget {
    Q_OBJECT
public:
    explicit SortVisualizer(QWidget* parent = nullptr);
    void setData(const std::vector<int>& data);
    void setHighlight(int index);
    void setRange(int minVal, int maxVal);
    void setBarColor(const QColor& color);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<int> m_data;
    int m_highlight = -1;
    int m_minVal = 0;
    int m_maxVal = 100;
    QColor m_barColor = QColor("#58a6ff");
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override { delete[] t1_sorted; }

private slots:
    void onTask1Generate();
    void onTask1Sort();
    void onTask1Search();

    void onTask2Sort();
    void onTask2Binpow();

    void onTask3Run();

    void onTask4Demo();

    void onTask5Demo();

private:
    QTabWidget* m_tabs = nullptr;

    // Task 1
    QLineEdit*  t1_input   = nullptr;
    QSpinBox*   t1_size    = nullptr;
    QLineEdit*  t1_search  = nullptr;
    QTextEdit*  t1_out     = nullptr;
    SortVisualizer* t1_viz = nullptr;
    int* t1_sorted = nullptr;
    int  t1_n      = 0;

    // Task 2
    QLineEdit*  t2_input   = nullptr;
    QLineEdit*  t2_search  = nullptr;
    QLineEdit*  t2_mod     = nullptr;
    QLineEdit*  t2_base    = nullptr;
    QLineEdit*  t2_exp     = nullptr;
    QLineEdit*  t2_pmod    = nullptr;
    QTextEdit*  t2_out     = nullptr;

    // Task 3
    QLineEdit*  t3_input   = nullptr;
    QTextEdit*  t3_out     = nullptr;

    // Task 4
    QTextEdit*  t4_out     = nullptr;
    SortVisualizer* t4_viz = nullptr;

    // Task 5
    QTextEdit*    t5_out   = nullptr;
    QTableWidget* t5_tbl1  = nullptr;
    QTableWidget* t5_tbl2  = nullptr;

    // Builder helpers
    QWidget* buildTask1();
    QWidget* buildTask2();
    QWidget* buildTask3();
    QWidget* buildTask4();
    QWidget* buildTask5();

    // Shared styling
    void applyStyle();
    static QPushButton* makeBtn(const QString& text, const QString& color);
    static QGroupBox*   makeGroup(const QString& title);
    static QTextEdit*   makeConsole();
    static QString      htmlArr(const std::vector<int>& v, int highlight=-1,
                                const QString& label={});
    static QString      htmlArrD(const std::vector<double>& v);
    static QString      badge(const QString& text, const QString& bg,
                              const QString& fg = "#FFFFFF");
};