#include "mainwindow.h"
#include "keyboardtrainer.h"
#include "stringdemo.h"
#include "bitsetdemo.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto *tabs = new QTabWidget;
    tabs->addTab(new KeyboardTrainer, "Клавиатурный тренажёр");
    tabs->addTab(new StringDemo, "Библиотека String");
    tabs->addTab(new BitSetDemo, "BitSet");
    setCentralWidget(tabs);
    setWindowTitle("Лабораторная работа №5");
    resize(1200, 800);
    setStyleSheet("QMainWindow { background-color: #1e1e1e; } QTabWidget::pane { border: 1px solid #444; }");
}