#include "keyboardwidget.h"
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QMargins>

KeyboardWidget::KeyboardWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(160);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    buildLayout();
}

void KeyboardWidget::buildLayout()
{
    m_keys.clear();

    // Строки клавиш (4 ряда + пробел)
    QStringList rows = {
        "1234567890-=",
        "qwertyuiop[]",
        "asdfghjkl;'",
        "zxcvbnm,./"
    };

    // Коэффициенты ширины и отступов будут пересчитаны при resize
    Q_UNUSED(rows);
    // Реальное построение идёт в resizeEvent, потому что нужны размеры виджета
}

void KeyboardWidget::resizeEvent(QResizeEvent *)
{
    m_keys.clear();
    QStringList rows = {
        "1234567890-=",
        "qwertyuiop[]",
        "asdfghjkl;'",
        "zxcvbnm,./"
    };

    qreal margin = 4.0;
    qreal keySpacing = 3.0;
    qreal totalWidth = width() - 2 * margin;
    // Размеры рядов с разной шириной (доли)
    QList<int> counts = {13, 12, 11, 10}; // количество клавиш в ряду
    qreal y = margin;
    qreal rowHeight = (height() - 2 * margin) / 4.5; // 4 ряда + пробел (0.5 ряда)

    // Первый ряд, второй, третий, четвертый
    for (int row = 0; row < 4; ++row) {
        QString chars = rows[row];
        int n = counts[row];
        qreal keyWidth = (totalWidth - (n - 1) * keySpacing) / n;
        qreal x = margin;
        for (int col = 0; col < chars.size(); ++col) {
            QChar ch = chars.at(col);
            QRectF rect(x, y, keyWidth, rowHeight);
            m_keys.append({ch, rect, false});
            x += keyWidth + keySpacing;
        }
        y += rowHeight + keySpacing;
    }

    // Пробел
    qreal spaceWidth = totalWidth * 0.5;
    qreal spaceX = margin + (totalWidth - spaceWidth) / 2.0;
    m_keys.append({QChar(' '), QRectF(spaceX, y, spaceWidth, rowHeight * 0.8), false});
}

void KeyboardWidget::setActiveKey(QChar key)
{
    // Сброс старой подсветки
    for (auto &k : m_keys)
        k.active = false;

    QChar lower = key.toLower();
    // Ищем клавишу
    for (auto &k : m_keys) {
        if (k.character == lower) {
            k.active = true;
            break;
        }
    }
    m_activeKey = lower;
    update();
}

void KeyboardWidget::resetHighlight()
{
    for (auto &k : m_keys)
        k.active = false;
    m_activeKey = QChar();
    update();
}

void KeyboardWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#1a1a2e"));

    QFont font("Consolas", 12, QFont::Bold);
    painter.setFont(font);

    for (const auto &key : m_keys) {
        QRectF r = key.rect;
        // Цвет клавиши
        QColor fill = key.active ? QColor("#00ff88") : QColor("#3d3d5c");
        QColor border = QColor("#5a5a8a");
        QColor textColor = key.active ? Qt::black : Qt::white;

        // Рисуем фон
        painter.setPen(QPen(border, 1));
        painter.setBrush(fill);
        painter.drawRoundedRect(r, 4, 4);

        // Текст клавиши
        painter.setPen(textColor);
        QString label = key.character;
        if (key.character == ' ')
            label = "Space";
        painter.drawText(r, Qt::AlignCenter, label);
    }
}