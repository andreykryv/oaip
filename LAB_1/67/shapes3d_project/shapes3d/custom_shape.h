#pragma once
#include "polygon_shape.h"

/**
 * @brief Стрелка — произвольная фигура, не являющаяся разновидностью
 * стандартных многоугольников.
 *
 * Задаётся шириной хвоста (tailWidth), шириной наконечника (headWidth),
 * общей длиной (length) и относительной позицией наконечника (headRatio).
 */
class ArrowShape : public PolygonShape
{
    Q_OBJECT
public:
    explicit ArrowShape(float length   = 120.0f,
                        float tailWidth = 25.0f,
                        float headWidth = 60.0f,
                        float headRatio = 0.45f,
                        QObject* parent = nullptr);

    QString typeName() const override { return "Arrow"; }

    float length()    const { return m_length; }
    float tailWidth() const { return m_tailW; }
    float headWidth() const { return m_headW; }
    float headRatio() const { return m_headRatio; }

    void setLength(float v)    { m_length = v;     rebuildVertices(); }
    void setTailWidth(float v) { m_tailW  = v;     rebuildVertices(); }
    void setHeadWidth(float v) { m_headW  = v;     rebuildVertices(); }
    void setHeadRatio(float v) { m_headRatio = v;  rebuildVertices(); }

private:
    float m_length;
    float m_tailW;
    float m_headW;
    float m_headRatio;
    void recompute() override;
};
