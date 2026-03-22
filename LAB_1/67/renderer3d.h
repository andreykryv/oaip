#pragma once
#include <QWidget>
#include <QVector>
#include <QSharedPointer>
#include "shape.h"

/**
 * @brief Виджет для 3D-рендеринга фигур с помощью QPainter.
 *
 * Реализует перспективную проекцию вручную. Фигуры отрисовываются как
 * вытянутые (экструдированные) призмы. Поддерживается:
 *  - Вращение камеры мышью (drag)
 *  - Масштабирование колёсиком
 *  - Отображение осей координат
 *  - Подсветка выбранной фигуры
 */
class Renderer3D : public QWidget
{
    Q_OBJECT
public:
    explicit Renderer3D(QWidget* parent = nullptr);

    void addShape(Shape* shape);
    void removeShape(Shape* shape);
    void clearShapes();

    void setSelectedIndex(int i) { m_selectedIndex = i; update(); }
    int  selectedIndex() const   { return m_selectedIndex; }

    Shape* selectedShape();
    QVector<Shape*>& shapes()    { return m_shapes; }

    // Параметры камеры
    float azimuth()   const { return m_azimuth; }
    float elevation() const { return m_elevation; }
    float zoom()      const { return m_zoom; }

signals:
    void shapeClicked(int index);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*)   override;
    void mouseMoveEvent(QMouseEvent*)    override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*)        override;

private:
    QVector<Shape*> m_shapes;
    int             m_selectedIndex;

    // Camera
    float m_azimuth;    // горизонтальный угол (°)
    float m_elevation;  // вертикальный угол (°)
    float m_zoom;       // масштаб сцены

    // Mouse drag
    bool  m_dragging;
    QPoint m_lastMousePos;

    // ── Рендеринг ──────────────────────────────────────────────────────────

    /** Перспективная проекция 3D-точки на экран */
    QPointF project(float x, float y, float z) const;

    /** Отрисовка одной фигуры-призмы */
    void drawShape(QPainter& p, Shape* shape, bool selected);

    /** Отрисовка осей координат */
    void drawAxes(QPainter& p);

    /** Заполненный многоугольник на экране */
    void fillPolygon(QPainter& p, const QVector<QVector2D>& pts,
                     float z, QColor color);

    /** Боковые грани призмы */
    void drawSides(QPainter& p, const QVector<QVector2D>& pts,
                   float zBot, float zTop, QColor color);
};
