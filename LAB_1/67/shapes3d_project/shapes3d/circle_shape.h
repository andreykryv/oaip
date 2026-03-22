#pragma once
#include "shape.h"

/**
 * @brief Круг — непосредственный наследник Shape.
 * Аппроксимируется N-угольником для рендеринга.
 */
class CircleShape : public Shape
{
    Q_OBJECT
public:
    explicit CircleShape(float radius = 50.0f, QObject* parent = nullptr);

    float area()      const override;
    float perimeter() const override;
    QString typeName() const override { return "Circle"; }

    QVector<QVector2D> localOutlinePoints() const override;

    float radius() const        { return m_radius; }
    void  setRadius(float r)    { m_radius = r; emit changed(); }

    int   segments() const      { return m_segments; }
    void  setSegments(int s)    { m_segments = s; emit changed(); }

private:
    float m_radius;
    int   m_segments; // количество сегментов аппроксимации
};
