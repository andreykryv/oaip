#ifndef STRINGDEMO_H
#define STRINGDEMO_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QScrollArea>
#include <QHBoxLayout>          // добавлено
#include "stringlib/stringlib.h"

// ─── Виджет визуализации строки как ячеек ─────────────────
class StringVisualizerWidget : public QFrame
{
    Q_OBJECT
public:
    explicit StringVisualizerWidget(const QString &label, QWidget *parent = nullptr);
    void setString(const QString &s);
    void clear();

private:
    QLabel  *m_label;
    QWidget *m_cells;
    QHBoxLayout *m_cellLayout;    // замена на конкретный тип

    QLabel* makeLabel(const QString &t);  // добавлено объявление
};

// ─── Основной демо-виджет ─────────────────────────────────
class StringDemo : public QWidget
{
    Q_OBJECT
public:
    explicit StringDemo(QWidget *parent = nullptr);

private slots:
    void performOperation();
    void iterateString();
    void onOperationChanged(int idx);

private:
    void setupDescriptions();
    void logResult(const QString &op, const QString &result,
                   const QString &color = "#00ff88");
    QLabel* makeLabel(const QString &t);   // добавлено объявление

    QLineEdit *input1;
    QLineEdit *input2;
    QLineEdit *countEdit;
    QComboBox *opCombo;
    QTextEdit *output;
    QLabel    *descLabel;

    StringVisualizerWidget *vis1;
    StringVisualizerWidget *vis2;
    StringVisualizerWidget *visR;

    QMap<QString, QString> m_descriptions;
};
#endif