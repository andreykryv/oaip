#pragma once
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QTextEdit>
#include <QLabel>
#include <QGroupBox>
#include <QComboBox>
#include "bstviswidget.h"
#include "arrayheapviswidget.h"
#include "listheapviswidget.h"
#include "hashtableviswidget.h"

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    QTabWidget* m_tabs;

    // BST widgets
    BSTVisWidget* m_bstWidget;
    QLineEdit* m_bstInput;
    QTextEdit* m_bstLog;

    // Array Heap widgets
    ArrayHeapVisWidget* m_arrayHeapWidget;
    QLineEdit* m_arrayHeapInput;
    QTextEdit* m_arrayHeapLog;

    // List Heap widgets
    ListHeapVisWidget* m_listHeapWidget;
    QLineEdit* m_listHeapInput;
    QTextEdit* m_listHeapLog;

    // Hash Table widgets
    HashTableVisWidget* m_hashWidget;
    QLineEdit* m_hashKeyInput;
    QLineEdit* m_hashValInput;
    QTextEdit* m_hashLog;

    void setupBSTTab();
    void setupArrayHeapTab();
    void setupListHeapTab();
    void setupHashTab();

    void logTo(QTextEdit* log, const QString& msg);
};
