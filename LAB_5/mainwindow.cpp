#include "mainwindow.h"
#include "keyboardtrainer.h"
#include "stringdemo.h"
#include "bitsetdemo.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QCloseEvent>
#include <QPalette>
#include <QStatusBar>           // добавлено

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupGlobalStyles();

    // ── Центральный виджет ────────────────────────────────────
    QWidget *central = new QWidget;
    QVBoxLayout *root = new QVBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Шапка ────────────────────────────────────────────────
    root->addWidget(buildHeader());

    // ── Вкладки ──────────────────────────────────────────────
    tabs = new QTabWidget;
    tabs->setObjectName("mainTabs");

    tabs->addTab(new KeyboardTrainer, "  ⌨  Клавиатурный тренажёр  ");
    tabs->addTab(new StringDemo,      "  🔤  Библиотека String       ");
    tabs->addTab(new BitSetDemo,      "  🔢  BitSet                  ");

    root->addWidget(tabs);

    // ── Статус-бар ────────────────────────────────────────────
    statusBar()->showMessage("  LAB_5  ·  Qt " QT_VERSION_STR "  ·  C++17");
    statusBar()->setStyleSheet(
        QString("background:%1; color:%2; border-top:1px solid %3;")
        .arg(Theme::BG_CARD, Theme::TEXT_DIM, Theme::BORDER));

    setCentralWidget(central);
    setWindowTitle("Лабораторная работа №5  ·  Строки, BitSet, Тренажёр");

    loadSettings();
}

// ─── Шапка ────────────────────────────────────────────────
QWidget* MainWindow::buildHeader()
{
    QFrame *header = new QFrame;
    header->setFixedHeight(56);
    header->setObjectName("appHeader");

    QHBoxLayout *lay = new QHBoxLayout(header);
    lay->setContentsMargins(20, 0, 20, 0);

    // Логотип
    QLabel *logo = new QLabel("<span style='font-size:22px;'>💻</span>");
    lay->addWidget(logo);

    // Название
    QLabel *title = new QLabel(
        QString("<span style='color:%1;font-size:16px;font-weight:700;"
                "font-family:%2;letter-spacing:1px;'>LAB_5</span>"
                "<span style='color:%3;font-size:13px;'>"
                "  /  Строки  /  BitSet  /  Тренажёр</span>")
        .arg(Theme::NEON_CYAN, Theme::FONT_MONO, Theme::TEXT_SEC));
    lay->addWidget(title);
    lay->addStretch();

    // Бейдж версии
    QLabel *ver = new QLabel(
        QString("<span style='color:%1;background:%2;border:1px solid %3;"
                "border-radius:4px;padding:2px 10px;font-size:11px;"
                "font-family:%4;'>Qt5 · C++17</span>")
        .arg(Theme::NEON_GREEN, Theme::BG_SURFACE, Theme::BORDER_LIT, Theme::FONT_MONO));
    lay->addWidget(ver);

    return header;
}

// ─── Глобальные стили ─────────────────────────────────────
void MainWindow::setupGlobalStyles()
{
    qApp->setStyleSheet(QString(R"(
        * { font-family: %1; }

        QMainWindow, QWidget {
            background: %2;
            color: %3;
        }

        QFrame#appHeader {
            background: %4;
            border-bottom: 1px solid %5;
        }

        /* Вкладки */
        QTabWidget#mainTabs::pane {
            border: none;
            background: %2;
        }
        QTabWidget#mainTabs QTabBar::tab {
            background: %4;
            color: %6;
            border: none;
            border-right: 1px solid %5;
            border-top: 3px solid transparent;
            padding: 12px 18px;
            font-size: 13px;
            font-weight: 600;
            min-width: 120px;
        }
        QTabWidget#mainTabs QTabBar::tab:selected {
            background: %2;
            color: %7;
            border-top: 3px solid %8;
        }
        QTabWidget#mainTabs QTabBar::tab:hover:!selected {
            background: %9;
            color: %3;
        }

        /* Группы */
        QGroupBox {
            background: %4;
            border: 1px solid %5;
            border-radius: 10px;
            margin-top: 18px;
            padding: 14px 16px 16px 16px;
            font-size: 12px;
            font-weight: 700;
            color: %6;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 14px;
            top: -9px;
            padding: 0 8px;
            color: %7;
            background: %4;
        }

        /* Поля ввода */
        QLineEdit, QSpinBox, QDoubleSpinBox, QComboBox {
            background: %10;
            color: %3;
            border: 1px solid %5;
            border-radius: 7px;
            padding: 8px 12px;
            font-size: 14px;
            font-family: %11;
            selection-background-color: %8;
        }
        QLineEdit:focus, QSpinBox:focus, QComboBox:focus {
            border: 1px solid %8;
            background: %9;
        }

        /* Кнопки */
        QPushButton {
            background: %9;
            color: %3;
            border: 1px solid %5;
            padding: 8px 18px;
            border-radius: 7px;
            font-weight: 600;
            font-size: 13px;
        }
        QPushButton:hover  { background: %12; border-color: %8; color: %7; }
        QPushButton:pressed { background: %5; }
        QPushButton:disabled { color: %13; border-color: %5; }

        /* Текстовый вывод */
        QTextEdit {
            background: %14;
            color: %3;
            border: 1px solid %5;
            border-radius: 8px;
            font-family: %11;
            font-size: 13px;
            padding: 10px;
            selection-background-color: %8;
        }

        /* Скроллы */
        QScrollBar:vertical {
            background: %4;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: %12;
            border-radius: 4px;
            min-height: 24px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }

        QScrollBar:horizontal {
            background: %4;
            height: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:horizontal {
            background: %12;
            border-radius: 4px;
            min-width: 24px;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }

        /* Метки */
        QLabel { color: %3; background: transparent; }

        /* SpinBox стрелки */
        QSpinBox::up-button, QSpinBox::down-button {
            background: %5; border-radius: 3px; width: 16px;
        }
        QComboBox::drop-down { background: %5; border-radius: 3px; width: 20px; }

        /* Статус */
        QStatusBar { background: %4; color: %6; font-size: 11px; }
    )").arg(
        Theme::FONT_UI,         // %1  font-family
        Theme::BG_MAIN,         // %2  BG_MAIN
        Theme::TEXT_PRI,        // %3  TEXT_PRI
        Theme::BG_CARD,         // %4  BG_CARD
        Theme::BORDER,          // %5  BORDER
        Theme::TEXT_SEC,        // %6  TEXT_SEC
        Theme::NEON_CYAN,       // %7  NEON_CYAN
        Theme::NEON_GREEN,      // %8  NEON_GREEN
        Theme::BG_SURFACE,      // %9  BG_SURFACE
        Theme::BG_INPUT,        // %10 BG_INPUT
        Theme::FONT_MONO,       // %11 FONT_MONO
        Theme::BORDER_LIT,      // %12 BORDER_LIT
        Theme::TEXT_DIM,        // %13 TEXT_DIM
        Theme::BG_DEEP          // %14 BG_DEEP
    ));
}

void MainWindow::loadSettings()
{
    QSettings s("University", "Lab5");
    resize(s.value("size", QSize(1280, 860)).toSize());
    tabs->setCurrentIndex(s.value("tab", 0).toInt());
}

void MainWindow::saveSettings()
{
    QSettings s("University", "Lab5");
    s.setValue("size", size());
    s.setValue("tab", tabs->currentIndex());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}