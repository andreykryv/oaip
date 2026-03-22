#include "shape.h"
#include <QtMath>

Shape::Shape(QObject* parent)
    : QObject(parent)
    , m_center(0, 0, 0)
    , m_color(Qt::blue)
    , m_extrusionDepth(20.0f)
    , m_rotation2D(0.0f)
    , m_scale2D(1.0f)
    , m_animStepsDone(0)
    , m_animStepsTotal(0)
{
    setupTimer();
}

void Shape::setupTimer()
{
    m_animTimer = new QTimer(this);
    m_animTimer->setSingleShot(false);
    connect(m_animTimer, &QTimer::timeout, this, &Shape::onAnimTimerTick);
}

// ── Центр масс ──────────────────────────────────────────────────────────────

void Shape::setCenterOfMass(float x, float y, float z)
{
    m_center = QVector3D(x, y, z);
    emit changed();
}

// ── Трансформации ────────────────────────────────────────────────────────────

void Shape::translate(float dx, float dy, float dz)
{
    m_center += QVector3D(dx, dy, dz);
    emit changed();
}

void Shape::rotateFigure(float angleDeg, QVector2D pivot)
{
    // Вращаем сам центр вокруг pivot в плоскости XY
    float rad = qDegreesToRadians(angleDeg);
    float cx = m_center.x() - pivot.x();
    float cy = m_center.y() - pivot.y();
    float rx = cx * qCos(rad) - cy * qSin(rad);
    float ry = cx * qSin(rad) + cy * qCos(rad);
    m_center.setX(rx + pivot.x());
    m_center.setY(ry + pivot.y());
    m_rotation2D += angleDeg;
    emit changed();
}

void Shape::scaleFigure(float factor, QVector2D pivot)
{
    // Масштабируем центр относительно pivot
    float cx = m_center.x() - pivot.x();
    float cy = m_center.y() - pivot.y();
    m_center.setX(cx * factor + pivot.x());
    m_center.setY(cy * factor + pivot.y());
    m_scale2D *= factor;
    emit changed();
}

// ── Мировые точки ───────────────────────────────────────────────────────────

QVector<QVector2D> Shape::worldOutlinePoints() const
{
    float rad = qDegreesToRadians(m_rotation2D);
    float cosA = qCos(rad);
    float sinA = qSin(rad);

    QVector<QVector2D> pts;
    for (const QVector2D& lp : localOutlinePoints()) {
        // Масштаб
        float sx = lp.x() * m_scale2D;
        float sy = lp.y() * m_scale2D;
        // Поворот
        float wx = sx * cosA - sy * sinA + m_center.x();
        float wy = sx * sinA + sy * cosA + m_center.y();
        pts.append(QVector2D(wx, wy));
    }
    return pts;
}

// ── Анимации ─────────────────────────────────────────────────────────────────

void Shape::stopAnimation()
{
    m_animTimer->stop();
}

void Shape::animatedTranslate(float dx, float dy, float dz,
                               int durationMs, int steps)
{
    stopAnimation();
    float stepDx = dx / steps;
    float stepDy = dy / steps;
    float stepDz = dz / steps;
    m_animStepsDone = 0;
    m_animStepsTotal = steps;
    m_animStep = [=]() {
        translate(stepDx, stepDy, stepDz);
    };
    m_animTimer->setInterval(durationMs / steps);
    m_animTimer->start();
}

void Shape::animatedRotate(float angleDeg, QVector2D pivot,
                            int durationMs, int steps)
{
    stopAnimation();
    float stepAngle = angleDeg / steps;
    m_animStepsDone = 0;
    m_animStepsTotal = steps;
    m_animStep = [=]() {
        rotateFigure(stepAngle, pivot);
    };
    m_animTimer->setInterval(durationMs / steps);
    m_animTimer->start();
}

void Shape::animatedScale(float factor, QVector2D pivot,
                           int durationMs, int steps)
{
    stopAnimation();
    // factor^(1/steps) за каждый шаг
    float stepFactor = qPow(factor, 1.0f / steps);
    m_animStepsDone = 0;
    m_animStepsTotal = steps;
    m_animStep = [=]() {
        scaleFigure(stepFactor, pivot);
    };
    m_animTimer->setInterval(durationMs / steps);
    m_animTimer->start();
}

void Shape::onAnimTimerTick()
{
    if (m_animStepsDone >= m_animStepsTotal) {
        m_animTimer->stop();
        emit transformFinished();
        return;
    }
    if (m_animStep) m_animStep();
    ++m_animStepsDone;
    emit transformStep();
}
