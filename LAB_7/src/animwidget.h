#pragma once
#include <QWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QTimer>
#include <QPainter>
#include <QColor>
#include <QEasingCurve>
#include <QAbstractAnimation>

// Плавное появление виджета
inline void fadeIn(QWidget* w, int ms = 250) {
    auto* eff  = new QGraphicsOpacityEffect(w);
    w->setGraphicsEffect(eff);
    auto* anim = new QPropertyAnimation(eff, "opacity", w);
    anim->setDuration(ms);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

// Плавное скольжение по Y
inline void slideIn(QWidget* w, int fromY, int ms = 350) {
    auto* anim = new QPropertyAnimation(w, "pos", w);
    anim->setDuration(ms);
    anim->setEasingCurve(QEasingCurve::OutBack);
    QPoint target = w->pos();
    anim->setStartValue(QPoint(target.x(), fromY));
    anim->setEndValue(target);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

// Цветовая палитра
namespace Pal {
    const QColor BG       { 8,   8,  18};
    const QColor BG2      {15,  15,  30};
    const QColor BG3      {22,  22,  45};
    const QColor ACCENT   {124, 77, 255};
    const QColor ACCENT2  { 0, 200, 180};
    const QColor ACCENT3  {255, 100, 150};
    const QColor SUCCESS  { 0, 220, 120};
    const QColor WARNING  {255, 180,  40};
    const QColor ERROR_C  {255,  70,  70};
    const QColor TEXT1    {230, 230, 250};
    const QColor TEXT2    {150, 150, 180};
    const QColor TEXT3    { 80,  80, 110};
    const QColor BORDER   { 40,  40,  70};
    const QColor NODE_BG  { 30,  30,  60};
}

// Глобальный stylesheet
inline QString globalStyle() {
    return R"(
QMainWindow, QWidget {
    background: #08080f;
    color: #e6e6f5;
    font-family: 'JetBrains Mono', 'Cascadia Code', 'Consolas', monospace;
    font-size: 13px;
}
QTabWidget::pane { border: 1px solid #28284a; background: #0b0b1a; border-radius:8px; }
QTabBar::tab {
    background: #111120; color: #8888aa; border:none;
    padding: 12px 22px; font-weight:600; font-size:13px;
    border-right: 1px solid #1e1e38;
}
QTabBar::tab:selected { background: #0b0b1a; color: #c8c8ff; border-bottom:3px solid #7c4dff; }
QTabBar::tab:hover:!selected { background: #15152a; color: #aaaacc; }
QGroupBox {
    background: #0f0f22; border:1px solid #28284a; border-radius:10px;
    margin-top:14px; padding:14px;
    font-weight:700; color:#9090cc;
}
QGroupBox::title { subcontrol-origin:margin; left:14px; top:-8px;
    padding:0 6px; background:#0f0f22; color:#7c4dff; }
QLineEdit, QSpinBox, QComboBox {
    background: #14142a; color:#e6e6f5;
    border:1px solid #2e2e50; border-radius:6px;
    padding:7px 11px; font-size:13px;
    selection-background-color:#7c4dff;
}
QLineEdit:focus, QSpinBox:focus { border:1px solid #7c4dff; }
QPushButton {
    background: #7c4dff; color:#fff; border:none;
    border-radius:7px; padding:9px 22px;
    font-weight:700; font-size:13px;
}
QPushButton:hover { background:#9068ff; }
QPushButton:pressed { background:#5a30cc; }
QPushButton#sec {
    background: #1e1e38; color:#c8c8ff; border:1px solid #3a3a60;
}
QPushButton#sec:hover { background:#2a2a50; }
QTextEdit, QListWidget {
    background:#0a0a18; color:#c8c8e8;
    border:1px solid #28284a; border-radius:8px;
    font-family:'JetBrains Mono','Consolas',monospace; font-size:12px;
    selection-background-color:#7c4dff;
}
QScrollBar:vertical { background:#0f0f22; width:8px; border-radius:4px; }
QScrollBar::handle:vertical { background:#3a3a60; border-radius:4px; min-height:20px; }
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0; }
QLabel { color:#c8c8e8; background:transparent; }
QSplitter::handle { background:#28284a; width:2px; }
QStatusBar { background:#0b0b1a; color:#6060a0; border-top:1px solid #28284a; }
    )";
}