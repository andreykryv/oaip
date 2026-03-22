#pragma once
#include "polygon_shape.h"

/**
 * @brief Звезда — наследник PolygonShape.
 * Поддерживает 5-, 6- и 8-конечные звёзды.
 * Задаётся внешним радиусом (R) и внутренним (r).
 */
class StarShape : public PolygonShape
{
    Q_OBJECT
public:
    enum PointCount { Star5 = 5, Star6 = 6, Star8 = 8 };

    explicit StarShape(PointCount pts  = Star5,
                       float outerR    = 80.0f,
                       float innerR    = 35.0f,
                       QObject* parent = nullptr);

    QString typeName() const override;

    PointCount pointCount() const   { return m_points; }
    float outerRadius()    const    { return m_outerR; }
    float innerRadius()    const    { return m_innerR; }

    void setPointCount(PointCount p) { m_points = p; rebuildVertices(); }
    void setOuterRadius(float r)     { m_outerR = r; rebuildVertices(); }
    void setInnerRadius(float r)     { m_innerR = r; rebuildVertices(); }

private:
    PointCount m_points;
    float      m_outerR;
    float      m_innerR;
    void recompute() override;
};
