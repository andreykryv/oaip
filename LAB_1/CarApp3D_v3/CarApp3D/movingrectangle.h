#pragma once
#include "shape3d.h"

#include <QObject>
#include <QTimer>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

// ============================================================
// MovingRectangle — РОДИТЕЛЬСКИЙ класс.
// Хранит позицию/ориентацию/масштаб и анимирует через QTimer.
// Дочерние классы используют getModelMatrix() при рендеринге.
// ============================================================
class MovingRectangle : public QObject, public Shape3D
{
    Q_OBJECT
public:
    explicit MovingRectangle(QObject* parent = nullptr);

    // Shape3D
    double  area()      const override;
    double  perimeter() const override;
    QString typeName()  const override { return "MovingRectangle"; }
    void translate(float dx, float dy, float dz)               override;
    void rotateDeg(float angle, float ax, float ay, float az)  override;
    void setScale(float s)                                     override;

    // Animated transforms (emit animStep each tick)
    void animMoveTo (QVector3D target,  int ms = 1800);
    void animRotate (float deg, QVector3D axis, int ms = 1500);
    void animScaleTo(float target,      int ms = 1000);

    // Accessors
    QMatrix4x4  getModelMatrix() const;
    QVector3D   position()       const { return m_pos; }
    float       scaleValue()     const { return m_scale; }
    bool        isAnimating()    const { return m_timer.isActive(); }

    void setDimensions(float w, float h, float d) { m_w=w; m_h=h; m_d=d; }
    float width()  const { return m_w; }
    float height() const { return m_h; }
    float depth()  const { return m_d; }

    void resetTransform();

signals:
    void animStep();
    void animFinished();

private:
    float m_w = 1.72f, m_h = 1.39f, m_d = 4.42f;

    QVector3D   m_pos;
    QQuaternion m_orient;
    float       m_scale = 1.0f;

    enum class AK { None, Move, Rotate, Scale };
    AK    m_ak = AK::None;

    QVector3D m_aStartPos, m_aTargetPos;
    float     m_aStartScl, m_aTargetScl;
    float     m_aAccumDeg, m_aTotalDeg;
    QVector3D m_aAxis;

    int m_aDur = 1800, m_aElapsed = 0;
    static constexpr int kTick = 16;
    QTimer m_timer;

private slots:
    void onTick();
};
