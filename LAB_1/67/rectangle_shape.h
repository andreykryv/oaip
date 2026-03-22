#pragma once
#include "polygon_shape.h"

/**
 * @brief Прямоугольник — наследник PolygonShape.
 * Задаётся шириной и высотой; центр масс в начале локальных координат.
 */
class RectangleShape : public PolygonShape
{
    Q_OBJECT
public:
    explicit RectangleShape(float width = 100.0f, float height = 60.0f,
                            QObject* parent = nullptr);

    QString typeName() const override { return "Rectangle"; }

    virtual float width()  const { return m_width; }
    virtual float height() const { return m_height; }

    virtual void setWidth(float w);
    virtual void setHeight(float h);
    virtual void setSize(float w, float h);

protected:
    float m_width;
    float m_height;
    void recompute() override;
};
