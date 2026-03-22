#pragma once
#include "shape.h"

/**
 * @brief Абстрактный многоугольник — промежуточный уровень иерархии.
 *
 * Хранит список локальных вершин и предоставляет общие расчёты
 * площади (по формуле Гаусса) и периметра.
 * Конкретные подклассы либо задают вершины напрямую, либо вычисляют их
 * из собственных параметров (стороны, радиусы и т.д.).
 */
class PolygonShape : public Shape
{
    Q_OBJECT
public:
    explicit PolygonShape(QObject* parent = nullptr);

    // Площадь по формуле Гаусса (shoelace)
    float area()      const override;
    // Периметр как сумма длин сторон
    float perimeter() const override;

    QVector<QVector2D> localOutlinePoints() const override { return m_vertices; }

    // Прямой доступ к вершинам (для треугольника, ромба и т.д.)
    void  setVertices(const QVector<QVector2D>& v);
    const QVector<QVector2D>& vertices() const { return m_vertices; }
    void  setVertex(int i, QVector2D v);

protected:
    QVector<QVector2D> m_vertices; // локальные координаты вершин
    void rebuildVertices();        // переопределяется в подклассах для пересчёта
    virtual void recompute() {}    // вызывается после изменения параметров фигуры
};
