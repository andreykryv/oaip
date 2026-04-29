#include "mainwindow.h"
#include "keyboardtrainer.h"
#include "stringdemo.h"
#include "bitsetdemo.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QPalette>
#include <QColor>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupGlobalStyles();

    tabs = new QTabWidget;
    tabs->addTab(new KeyboardTrainer, "Клавиатурный тренажёр");
    tabs->addTab(new StringDemo, "Библиотека String");
    tabs->addTab(new BitSetDemo, "BitSet");
    setCentralWidget(tabs);
    setWindowTitle("Лабораторная работа №5");

    loadSettings();
}

void MainWindow::setupGlobalStyles()
{
    // Глобальная тёмная тема с современными цветами
    QString styleSheet = R"(
        QMainWindow {
            background-color: #1a1a2e;
        }

        QTabWidget::pane {
            border: 1px solid #3d3d5c;
            border-radius: 8px;
            background-color: #1a1a2e;
        }

        QTabBar::tab {
            background-color: #2d2d44;
            color: #b0b0c0;
            padding: 10px 20px;
            margin-right: 2px;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            font-weight: bold;
        }

        QTabBar::tab:selected {
            background-color: #3d3d5c;
            color: #ffffff;
        }

        QTabBar::tab:hover:!selected {
            background-color: #4a4a6a;
        }

        QPushButton {
            background-color: #3d3d5c;
            color: #ffffff;
            border: none;
            padding: 8px 16px;
            border-radius: 6px;
            font-weight: bold;
            min-width: 80px;
        }

        QPushButton:hover {
            background-color: #4a4a6a;
        }

        QPushButton:pressed {
            background-color: #2d2d44;
        }

        QPushButton:disabled {
            background-color: #2a2a3a;
            color: #666680;
        }

        QLineEdit, QTextEdit, QComboBox, QSpinBox {
            background-color: #2d2d44;
            color: #ffffff;
            border: 1px solid #3d3d5c;
            border-radius: 6px;
            padding: 6px 10px;
            selection-background-color: #4a4a6a;
        }

        QLineEdit:focus, QTextEdit:focus, QComboBox:focus, QSpinBox:focus {
            border: 1px solid #5a5a8a;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #ffffff;
            margin-right: 8px;
        }

        QLabel {
            color: #e0e0e0;
            background-color: transparent;
        }

        QGroupBox {
            border: 1px solid #3d3d5c;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 10px;
            font-weight: bold;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 6px;
            color: #b0b0c0;
        }

        QToolTip {
            background-color: #2d2d44;
            color: #ffffff;
            border: 1px solid #3d3d5c;
            border-radius: 4px;
            padding: 4px 8px;
        }

        QScrollBar:vertical {
            background-color: #1a1a2e;
            width: 12px;
            border-radius: 6px;
        }

        QScrollBar::handle:vertical {
            background-color: #3d3d5c;
            border-radius: 6px;
            min-height: 20px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: #4a4a6a;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QScrollBar:horizontal {
            background-color: #1a1a2e;
            height: 12px;
            border-radius: 6px;
        }

        QScrollBar::handle:horizontal {
            background-color: #3d3d5c;
            border-radius: 6px;
            min-width: 20px;
        }

        QScrollBar::handle:horizontal:hover {
            background-color: #4a4a6a;
        }

        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }
    )";

    qApp->setStyleSheet(styleSheet);
}

void MainWindow::loadSettings()
{
    QSettings settings;
    resize(settings.value("window/size", QSize(1200, 800)).toSize());
    int tabIndex = settings.value("window/currentTab", 0).toInt();
    tabs->setCurrentIndex(tabIndex);
}

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("window/size", size());
    settings.setValue("window/currentTab", tabs->currentIndex());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}