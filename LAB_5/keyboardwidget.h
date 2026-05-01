#ifndef KEYBOARDWIDGET_H
#define KEYBOARDWIDGET_H

#include <QWidget>
#include <QMap>
#include <QChar>

class KeyboardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit KeyboardWidget(QWidget *parent = nullptr);

    // Подсветить одну клавишу (следующий ожидаемый символ)
    void setActiveKey(QChar key);
    // Сбросить всю подсветку
    void resetHighlight();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    struct Key {
        QChar character;
        QRectF rect;
        bool active = false;
    };

    void buildLayout();
    QList<Key> m_keys;
    QChar m_activeKey;
};

#endif // KEYBOARDWIDGET_H