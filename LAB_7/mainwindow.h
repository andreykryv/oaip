#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QSplitter>
#include <QStatusBar>
#include <QTimer>
#include <QRandomGenerator>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QComboBox>
#include <QLineEdit>
#include <QScrollBar>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>

#include "task1_queue.h"
#include "task2_bst.h"
#include "task3_hash.h"
#include "task4_openaddr.h"
#include "task5_deque.h"
#include "animwidget.h"

// ══ Объявления виджетов-задач ════════════════════════════════════
class Task1Widget;
class Task2Widget;
class Task3Widget;
class Task4Widget;
class Task5Widget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QTabWidget* tabs_;

    QWidget* buildHeaderWidget();
    Task1Widget* task1_;
    Task2Widget* task2_;
    Task3Widget* task3_;
    Task4Widget* task4_;
    Task5Widget* task5_;
};

// ═══════════════════════════════════════════════════════════════════
//  Вспомогательные виджеты
// ═══════════════════════════════════════════════════════════════════

// Виджет для отрисовки двухсвязной очереди / кольца
class QueuePaintWidget : public QWidget {
    Q_OBJECT
public:
    explicit QueuePaintWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumHeight(90);
    }

    void setData(const std::vector<int>& vals,
                 int highlightMax = INT_MIN, int highlightMin = INT_MAX,
                 bool isRing = false, const QString& label = "")
    {
        vals_ = vals; hMax_ = highlightMax; hMin_ = highlightMin;
        isRing_ = isRing; label_ = label;
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override;

private:
    std::vector<int> vals_;
    int hMax_ = INT_MIN, hMin_ = INT_MAX;
    bool isRing_ = false;
    QString label_;
};

// Виджет для BST
class BSTWidget : public QWidget {
    Q_OBJECT
public:
    explicit BSTWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumHeight(280);
    }
    void setTree(BST* tree) { tree_ = tree; update(); }
    void setHighlight(int key) { hlKey_ = key; update(); }
    void clearHighlight() { hlKey_ = INT_MIN; update(); }

protected:
    void paintEvent(QPaintEvent*) override;
private:
    BST* tree_ = nullptr;
    int  hlKey_ = INT_MIN;
    void drawNode(QPainter& p, BSTNode* n, double x, double y, double dx, int level);
};

// Виджет хеш-таблицы стеков
class HashTableWidget : public QWidget {
    Q_OBJECT
public:
    explicit HashTableWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumHeight(220);
    }
    void setTable(HashTableStacks* ht, int highlightStack = -1) {
        ht_ = ht; hl_ = highlightStack; update();
    }
protected:
    void paintEvent(QPaintEvent*) override;
private:
    HashTableStacks* ht_ = nullptr;
    int hl_ = -1;
};

// Виджет открытой адресации
class OpenAddrWidget : public QWidget {
    Q_OBJECT
public:
    explicit OpenAddrWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumHeight(120);
    }
    void setTable(UniversalHashTable* ht) { ht_ = ht; update(); }
protected:
    void paintEvent(QPaintEvent*) override;
private:
    UniversalHashTable* ht_ = nullptr;
};

// Виджет блочного deque
class DequeWidget : public QWidget {
    Q_OBJECT
public:
    explicit DequeWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumHeight(140);
    }
    void setDeque(BlockDeque<int>* dq) { dq_ = dq; update(); }
protected:
    void paintEvent(QPaintEvent*) override;
private:
    BlockDeque<int>* dq_ = nullptr;
};

// ═══════════════════════════════════════════════════════════════════
//  Task 1
// ═══════════════════════════════════════════════════════════════════
class Task1Widget : public QWidget {
    Q_OBJECT
public:
    explicit Task1Widget(QWidget* parent = nullptr);
private slots:
    void onGenerate();
    void onBuildRings();
    void onPushBack();
    void onPushFront();
    void onPopFront();
    void onPopBack();
private:
    void refresh();
    DoublyQueue<int> queue_;
    std::unique_ptr<RingResult7> rings_;
    QueuePaintWidget* mainQView_;
    QueuePaintWidget* ring1View_;
    QueuePaintWidget* ring2View_;
    QTextEdit*        log_;
    QSpinBox*         countSpin_;
    QSpinBox*         valSpin_;
    QLabel*           infoLabel_;
    void logLine(const QString& s, const QString& col = "#c8c8e8");
};

// ═══════════════════════════════════════════════════════════════════
//  Task 2
// ═══════════════════════════════════════════════════════════════════
class Task2Widget : public QWidget {
    Q_OBJECT
public:
    explicit Task2Widget(QWidget* parent = nullptr);
private slots:
    void onGenerate();
    void onBalance();
    void onInsert();
    void onSearch();
    void onRemove();
    void onCountChars();
    void onInorder();
    void onPreorder();
    void onPostorder();
private:
    BST tree_;
    BSTWidget*  treeView_;
    QTextEdit*  log_;
    QSpinBox*   keySpin_;
    QLineEdit*  dataEdit_;
    QLabel*     resultLabel_;
    void logLine(const QString& s, const QString& col = "#c8c8e8");
};

// ═══════════════════════════════════════════════════════════════════
//  Task 3
// ═══════════════════════════════════════════════════════════════════
class Task3Widget : public QWidget {
    Q_OBJECT
public:
    explicit Task3Widget(QWidget* parent = nullptr);
private slots:
    void onGenerate();
    void onFindMinStack();
    void onInsert();
    void onRemove();
    void onSearch();
private:
    HashTableStacks ht_{11};
    HashTableWidget* htView_;
    QTextEdit*       log_;
    QSpinBox*        keySpin_;
    QLabel*          resultLabel_;
    void logLine(const QString& s, const QString& col = "#c8c8e8");
    void refreshView(int hl = -1);
};

// ═══════════════════════════════════════════════════════════════════
//  Task 4
// ═══════════════════════════════════════════════════════════════════
class Task4Widget : public QWidget {
    Q_OBJECT
public:
    explicit Task4Widget(QWidget* parent = nullptr);
private slots:
    void onGenerate();
    void onInsert();
    void onSearch();
    void onRemove();
    void onRunBenchmark();
private:
    std::unique_ptr<UniversalHashTable> ht_;
    OpenAddrWidget* htView_;
    QTextEdit*      log_;
    QSpinBox*       sizeSpin_;
    QSpinBox*       keySpin_;
    QLabel*         benchLabel_;
    // Benchmark chart area
    QWidget*        chartWidget_;
    std::vector<SearchBenchmark> benchResults_;
    void logLine(const QString& s, const QString& col = "#c8c8e8");
    void rebuildHT();
    void updateChart();
};

// Виджет для отрисовки графика
class BenchChart : public QWidget {
    Q_OBJECT
public:
    explicit BenchChart(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumHeight(200);
    }
    void setData(const std::vector<SearchBenchmark>& d) { data_ = d; update(); }
protected:
    void paintEvent(QPaintEvent*) override;
private:
    std::vector<SearchBenchmark> data_;
};

// ═══════════════════════════════════════════════════════════════════
//  Task 5
// ═══════════════════════════════════════════════════════════════════
class Task5Widget : public QWidget {
    Q_OBJECT
public:
    explicit Task5Widget(QWidget* parent = nullptr);
private slots:
    void onPushBack();
    void onPushFront();
    void onPopBack();
    void onPopFront();
    void onClear();
    void onRandomFill();
    void onRandomAccess();
private:
    BlockDeque<int> dq_;
    DequeWidget*  dqView_;
    QTextEdit*    log_;
    QSpinBox*     valSpin_;
    QSpinBox*     idxSpin_;
    QLabel*       statsLabel_;
    void refresh();
    void logLine(const QString& s, const QString& col = "#c8c8e8");
};
